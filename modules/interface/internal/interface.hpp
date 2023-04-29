#pragma once

#include <cstdint>
#include <mutex>
#include <thread>


namespace juglans {

constexpr int SomeInt = 0;

class Interface {
 public:
  Interface(){};
  ~Interface(){};

  int Hello();

 private:
};

}  // namespace juglans
