/*
warning: this profiler is not multithread safe
*/

#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "log.hpp"
#include "time.hpp"

#define STRING(x) #x

namespace juglans {

class Profiler {
 public:
  Profiler(const std::string &tag_, int intent_ = 0) : tag(tag_), empty_prefix(2 * intent_, ' ') {}

  Profiler() : empty_prefix("") {}

  ~Profiler() {}

  std::string trace_str(time_t total_avg);

  inline void trace(time_t cost_ms) {
    ++trace_cnt;
    sum_cost += cost_ms;
    max_cost = max_cost > cost_ms ? max_cost : cost_ms;
  }

  inline void reset() {
    trace_cnt = 0;
    sum_cost = 0.0;
    max_cost = 0.0;
  }

  inline int get_trace_cnt() { return trace_cnt; }

  inline time_t get_avg() {
    double denominator = trace_cnt < 1 ? 1 : trace_cnt;
    return sum_cost / denominator;
  }

 private:
  std::string tag;
  const std::string empty_prefix;
  int trace_cnt = 0;
  time_t sum_cost = 0.0;
  time_t max_cost = 0.0;
};

class ProfilerManager {
 public:
  ProfilerManager(const ProfilerManager &signal) = delete;
  const ProfilerManager &operator=(const ProfilerManager &signal) = delete;

  void dump_trace_str();

  static ProfilerManager &GetInstance() {
    static ProfilerManager m_SingleInstance;
    return m_SingleInstance;
  }

  inline void add_func(const std::string &func_name, int indentation) {
    if (profilers.count(func_name) == 0) {
      add_order(func_name);
      profilers.emplace(func_name, Profiler(func_name, indentation));
    }
  }

  inline void trace(const std::string &tag, time_t cost_ms) {
    if (tag == orders[0] && profilers[tag].get_trace_cnt() >= threshold_num) reset();
    profilers[tag].trace(cost_ms);
  }

  inline void reset() {
    for (auto &[k, profiler] : profilers) profiler.reset();
  }

  inline void add_order(const std::string &func_name) { orders[total_order++] = func_name; }

 private:
  ProfilerManager() {}

  static const int threshold_num;
  static std::map<std::string, Profiler> profilers;
  static std::vector<std::string> orders;
  static int total_order;
};

class CostTrace {
 public:
  CostTrace(std::string tag_, int indentation_ = 0) : tag(tag_) {
    ProfilerManager::GetInstance().add_func(tag, indentation_);
    start = tick();
  }

  ~CostTrace() {
    auto cost = 1000 * (tick() - start);
    ProfilerManager::GetInstance().trace(tag, cost);
  }

 private:
  std::string tag;
  time_t start;
};

#define FUNC_COST_TRACE(FUNC_NAME, INDENT, FUNC) \
  do {                                           \
    CostTrace(FUNC_NAME, INDENT);                \
    FUNC;                                        \
  } while (0)

}  // end namespace juglans
