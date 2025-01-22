#include <git_initializer/cmd/add.h>
#include <git_initializer/helper.h>
#include <git_initializer/parser/parser.h>
#include <string>
#include <vector>

void add_arg(int argc, char *argv[]) noexcept {
  // either --key or --user
  if (argc == 3) {
    const std::vector<std::string> flag_split = Helper::split_str(argv[2], '=');
    const std::string &flag_name = flag_split.at(0);
    const std::string &flag_value = flag_split.at(1);
    if (flag_name == constants::KEY_FLAG) {
      ArgParser::parse_add_arg(flag_value, "");
    }
    if (flag_name == constants::USER_FLAG) {
      ArgParser::parse_add_arg("", flag_value);
    }
  }
  // both --key and --user
  if (argc == 4) {
    const std::vector<std::string> key_flag_split = Helper::split_str(argv[2], '=');
    const std::vector<std::string> user_flag_split = Helper::split_str(argv[3], '=');
    const std::string &key_flag_value = key_flag_split.at(1);
    const std::string &user_flag_value = user_flag_split.at(1);

    ArgParser::parse_add_arg(key_flag_value, user_flag_value);
  }
}
