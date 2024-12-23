#include <format>
#include <iostream>
#include <vector>
#include "git_initializer/helper.h"
#include "git_initializer/parser.h"
#include "git_initializer/utils.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    // TODO: parse the `help_msg.txt` file
    std::cout << "Under construction" << std::endl;
    return 0;
  }

  if (auto [is_corr, msg] = Utils::has_correct_arg_count(argv[1], argc); !is_corr) {
    Helper::log(msg, LogLevel::Error);
    Helper::exit_ungracefully();
  }

  if (argv[1] == INIT_ARG) {
    if (argc == 2) {
      ArgParser::parse_init_arg("");
    }

    std::vector<std::string> ign_flag_vals = Helper::split_str(argv[2], '=');
    ArgParser::parse_init_arg(ign_flag_vals.at(2));
  }
  if (argv[1] == ADD_ARG) {
    std::cout << argv[2] << " " << argv[3] << std::endl;
    std::vector<std::string> key_flag_vals = Helper::split_str(argv[2], '=');
    std::vector<std::string> user_flag_vals = Helper::split_str(argv[3], '=');
    ArgParser::parse_add_arg(key_flag_vals.at(1), user_flag_vals.at(1));
  }
  if (argv[1] == LIST_IGNORES_ARG) {
    ArgParser::parse_list_valid_ignore_files_arg();
  }
}
