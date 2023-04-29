#pragma once

#include <cstdint>

namespace juglans {

using time_t = double;

time_t tick();

void waitMS(uint32_t milliseconds);

}  // namespace juglans
