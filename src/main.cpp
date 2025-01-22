#include <format>
#include <git_initializer/cmd/add.h>
#include <git_initializer/cmd/init.h>
#include <git_initializer/helper.h>
#include <git_initializer/utils.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    Utils::read_help_msg_txt();
    return 0;
  }

  if (auto [is_corr, msg] = Utils::has_correct_arg_count(argv[1], argc); !is_corr) {
    Helper::log(msg, constants::LogLevel::Error);
    Helper::exit_ungracefully();
  }

  if (argv[1] == constants::INIT_ARG) {
    init_arg(argc, argv);
    return 0;
  }
  if (argv[1] == constants::ADD_ARG) {
    add_arg(argc, argv);
    return 0;
  }
  // if (argv[1] == constants::LIST_IGNORES_ARG ) {
  //   ArgParser::parse_list_valid_ignore_files_arg();
  //   return 0;
  // }

  return 1;
}
