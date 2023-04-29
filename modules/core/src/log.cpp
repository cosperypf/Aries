#include "juglans/core/log.hpp"

#include <cstdlib>

#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace juglans {

void LoggerInstance::setup(LogType type) {
  if (logger_ && type_ == type) return;
  type_ = type;
  logger_.reset();
  spdlog::drop_all();
  if (type == LogType::None)
    logger_ = spdlog::null_logger_mt("null");
  else if (type == LogType::Production) {
#ifdef __ANDROID__
    logger_ = spdlog::android_logger_mt("android", "Juglans");
    logger_->set_pattern("%v");
#else
#ifdef _WIN32
    char *env_ptr = nullptr;
    std::string folder;
    if (_dupenv_s(&env_ptr, nullptr, "APPDATA") == 0 && env_ptr) {
      folder = std::string(env_ptr) + "\\Juglans\\";
      free(env_ptr);
    }
#else
    static const std::string folder = std::string(getenv("HOME")) + "/.config/Juglans/";
#endif
    if (folder != "") {
      logger_ = spdlog::rotating_logger_mt("file", folder + "Juglans.log", 1024 * 1024, 3);
      logger_->set_pattern("%L%Y-%m-%d %H:%M:%S.%f [%t] %v");
    }
#endif
  }
  if (!logger_) {
    logger_ = spdlog::stdout_color_mt("console");
    logger_->set_pattern("%^%L%Y%m%d %H%M%S.%f [%t %s:%# %!] %v%$");
  }
#ifdef JUGLANS_DEBUG
  logger_->set_level(spdlog::level::level_enum::debug);
#endif
}

}  // namespace juglans
