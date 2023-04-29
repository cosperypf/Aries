#include "juglans/core/time.hpp"

#include <chrono>
#include <thread>

namespace juglans {

time_t tick() {
  std::chrono::duration<time_t> time = std::chrono::steady_clock::now().time_since_epoch();
  return time.count();
}

void waitMS(uint32_t milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

}  // namespace juglans
