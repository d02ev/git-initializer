#include <format>
#include <iostream>
#include <vector>
#include "git_initializer/helper.h"
#include "git_initializer/parser.h"
#include "git_initializer/utils.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    Utils::read_help_msg_txt();
    return 0;
  }

  if (auto [is_corr, msg] = Utils::has_correct_arg_count(argv[1], argc); !is_corr) {
    Helper::log(msg, LogLevel::Error);
    Helper::exit_ungracefully();
  }

  if (argv[1] == INIT_ARG) {
    if (argc == 2) {
      ArgParser::parse_init_arg("");
      return 0;
    }

    std::vector<std::string> ign_flag_vals = Helper::split_str(argv[2], '=');
    ArgParser::parse_init_arg(ign_flag_vals.at(1));

    return 0;
  }
  if (argv[1] == ADD_ARG) {
    std::vector<std::string> key_flag_vals = Helper::split_str(argv[2], '=');
    std::vector<std::string> user_flag_vals = Helper::split_str(argv[3], '=');
    ArgParser::parse_add_arg(key_flag_vals.at(1), user_flag_vals.at(1));

    return 0;
  }
  if (argv[1] == LIST_IGNORES_ARG) {
    ArgParser::parse_list_valid_ignore_files_arg();
    return 0;
  }

  return 1;
}
