#pragma once

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

//#include "juglans/core/filesystem.hpp"

namespace juglans {

class CommonBase {
 public:
  CommonBase(const std::string &name) : demo_name(name) {};
  ~CommonBase() {};

  int run(int argc, char **argv);

 protected:
  virtual void add_args(CLI::App &) {}
  virtual void parse_args() {}
  virtual bool init() { return true; }

 private:
  std::string input_path;
  std::string output_path;
  const std::string demo_name;
};

}  // namespace juglans
