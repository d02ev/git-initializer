#ifndef PARSER_H
#define PARSER_H

#include <string>

class ArgParser {
public:
  static void parse_add_arg(const std::string &git_key, const std::string &git_user) noexcept;
  static void parse_init_arg(std::string &ign_arg_val) noexcept;
  static void parse_list_valid_ignore_files_arg() noexcept;
};

#endif // PARSER_H
