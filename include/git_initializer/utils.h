#ifndef UTILS_H
#define UTILS_H

#include <json/value.h>
#include <string>

class Utils {
public:
  Utils() = default;

  ~Utils() = default;

  static std::pair<bool, std::string> has_correct_arg_count(const std::string &arg_name, int arg_count) noexcept;

  static std::pair<bool, std::string> has_empty_arg_value(const std::string &arg_name,
                                                          const std::string &arg_value) noexcept;

  static std::pair<bool, std::string> invalid_arg_name(const std::string &arg_name) noexcept;

  static std::pair<bool, std::string> invalid_flag_name(const std::string &flag_name) noexcept;

  static bool invalid_ignore_file_value(const std::string &arg_value) noexcept(false);

  static std::pair<bool, std::string> env_exists() noexcept;

  static Json::Value read_ignores_json() noexcept;

  static void create_readme_and_gitignore(const std::string &ign_file_content) noexcept;

  static std::vector<std::string> read_valid_ignores_txt() noexcept;

  static void read_help_msg_txt() noexcept;

  static void create_remote_repo() noexcept;

  static void run_git_commands() noexcept;
};

#endif // UTILS_H
