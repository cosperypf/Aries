#include "common.hpp"

#include "juglans/core/log.hpp"


namespace juglans {

int CommonBase::run(int argc, char **argv) {
  // NOTE: use lower case 1-letter options, upper case should be used by demo specific options

  LOG_INFO("Runnint {} success", demo_name);

  CLI::App app{demo_name};
  app.add_option("-i,--input", input_path, "input path");
  app.add_option("-o,--output", output_path, "save folder for results");
  add_args(app);

  CLI11_PARSE(app, argc, argv);
  parse_args();

  init();


  return 0;
}

}  // namespace juglans
