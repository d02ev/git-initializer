#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include "git_initializer/constants.h"

class Helper {
public:
  Helper() = default;

  ~Helper() = default;

  static bool check_env_config_existence(constants::EnvType env_type) noexcept;

  static std::string sanitize_dir_name(const std::string &dir_name) noexcept;

  static void log(const std::string &err_msg, constants::LogLevel log_level) noexcept;

  static void add_delay(int delay_ms = 1000) noexcept;

  static std::string get_static_file_path(const std::string &filename) noexcept;

  static void exit_gracefully() noexcept;

  static void exit_ungracefully() noexcept;

  static std::vector<std::string> split_str(const std::string &s, char delim) noexcept;

  static bool to_bool(const std::string &str) noexcept;
};

#endif // HELPER_H
