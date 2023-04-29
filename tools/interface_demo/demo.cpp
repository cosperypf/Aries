#include <fstream>

#include "juglans/interface.hpp"
#include "juglans/core/log.hpp"

#include "common.hpp"

namespace juglans {

class InterfaceDemo : public CommonBase {
 public:
  InterfaceDemo() : CommonBase("InterfaceDemo")  {};
  ~InterfaceDemo() {};

 protected:
  void add_args(CLI::App &app) override {
    app.add_option("-p,--print", print_params, "print string");
    app.add_option("-t,--type", input_type, "input type")
      ->check(CLI::IsMember({"none", "type1", "type2", "type3"}));
  }

  void parse_args() override {
  }

  bool init() override {
    LOG_INFO("---> print_params: {}", print_params);

    return true;
  }

 private:
  std::string print_params;
  std::string input_type;
};

}  // namespace juglans

int main(int argc, char **argv) {
  juglans::InterfaceDemo demo;
  return demo.run(argc, argv);
}
