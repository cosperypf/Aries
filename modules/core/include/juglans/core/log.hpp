#pragma once

#ifdef JUGLANS_DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif
#include <spdlog/spdlog.h>

namespace juglans {

enum class LogType {
  Console,
  Production,
  None,
};

class LoggerInstance {
 public:
  static std::shared_ptr<spdlog::logger> &logger() { return instance().logger_; }

  static void SetupLogger(LogType type = LogType::Console) { instance().setup(type); }

 private:
  LoggerInstance() { setup(LogType::Console); }

  ~LoggerInstance() {}

  static LoggerInstance &instance() {
    static LoggerInstance instance_;
    return instance_;
  }

  void setup(LogType type);

  std::shared_ptr<spdlog::logger> logger_;
  LogType type_ = LogType::None;
};

}  // namespace juglans

#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(juglans::LoggerInstance::logger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(juglans::LoggerInstance::logger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(juglans::LoggerInstance::logger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(juglans::LoggerInstance::logger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(juglans::LoggerInstance::logger(), __VA_ARGS__)
