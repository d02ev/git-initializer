#ifndef HELPER_H
#define HELPER_H

#include "git_initializer/constants.h"

class Helper {
  public:
    Helper() = default;
    ~Helper() = default;

    static std::pair<bool, std::string> check_env_existence();
    static std::string sanitize_dir_name(const std::string& dir_name);
    static void log(const std::string& msg, LogLevel log_level);
    static void add_delay(int delay_ms = 1000);
    static std::string get_static_file_path(const std::string& filename);
    static void exit_gracefully();
};

#endif //HELPER_H
