#include "juglans/core/profiler.hpp"

#include <map>
#include <vector>
#define MAX_FUNC_NUM 128

namespace juglans {
const int ProfilerManager::threshold_num = 0x7ffffff0;
std::map<std::string, Profiler> ProfilerManager::profilers;
std::vector<std::string> ProfilerManager::orders(MAX_FUNC_NUM);
int ProfilerManager::total_order = 0;

std::string Profiler::trace_str(time_t total_avg) {
  std::ostringstream out;
  out << empty_prefix << tag;
  std::string prefix(out.str());

  out.str("");
  out.precision(2);
  double this_avg = get_avg();
  out << std::left << std::setw(40) << prefix << " avg_cost:" << std::fixed << std::right
      << std::setw(8) << this_avg << " ms"
      << "   max_cost:" << std::fixed << std::right << std::setw(8) << max_cost << " ms"
      << "   ratio:" << std::fixed << std::right << std::setw(8) << 100 * (this_avg / total_avg)
      << "%";
  return out.str();
}

void ProfilerManager::dump_trace_str() {
  if (total_order == 0) return;
  int total_cnt = profilers[orders[0]].get_trace_cnt();
  if (total_cnt == 0) return;

  time_t total_avg = profilers[orders[0]].get_avg();
  for (int i = 0; i < total_order; ++i) { LOG_INFO(profilers[orders[i]].trace_str(total_avg)); }
  LOG_INFO("Total Frames {}", total_cnt);
}

}  // namespace juglans