#include <git_initializer/cmd/init.h>
#include <git_initializer/helper.h>
#include <git_initializer/parser/parser.h>
#include <vector>

void init_arg(int argc, char **argv) noexcept {
  if (argc == 2) {
    std::string ign_file_names = "";
    ArgParser::parse_init_arg(ign_file_names);
    return;
  }

  std::vector<std::string> ign_flag_vals = Helper::split_str(argv[2], '=');
  ArgParser::parse_init_arg(ign_flag_vals.at(1));
}
