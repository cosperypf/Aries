
#include <cstddef>

#include "juglans/interface.hpp"
#include "interface.hpp"
#include "juglans/core/log.hpp"

namespace juglans {

int Interface::Hello() {
  LOG_INFO("hello world!");
  return 0;
};

}  // namespace juglans::inference