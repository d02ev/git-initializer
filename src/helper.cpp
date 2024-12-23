#include "git_initializer/helper.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


std::pair<bool, std::string> Helper::check_env_existence() noexcept {
  std::string git_key_env = getenv(GIT_KEY.c_str());
  std::string git_user_env = getenv(GIT_USER.c_str());

  if (git_key_env.empty() && git_user_env.empty()) {
    return {false, std::format("{} and {}", GIT_KEY, GIT_USER)};
  }
  if (git_key_env.empty()) {
    return {false, GIT_KEY};
  }
  if (git_user_env.empty()) {
    return {false, GIT_USER};
  }

  return {true, std::format("{} and {}", GIT_KEY, GIT_USER)};
}

std::string Helper::sanitize_dir_name(const std::string &dir_name) noexcept {
  std::stringstream stream(dir_name);
  std::string temp;

  while (std::getline(stream, temp, '/')) {
  }
  return temp;
}

void Helper::log(const std::string &err_msg, const LogLevel log_level) noexcept {
  std::string colored_msg;

  switch (log_level) {
    case LogLevel::Error:
      colored_msg = ERROR_CLR + err_msg + RESET;
      break;
    case LogLevel::Info:
      colored_msg = INFO_CLR + err_msg + RESET;
      break;
    case LogLevel::Success:
      colored_msg = SUCCESS_CLR + err_msg + RESET;
      break;
    case LogLevel::Internal:
      colored_msg = ERROR_CLR + err_msg + RESET;
      break;
  }

  std::cout << colored_msg << std::endl;
}

void Helper::add_delay(const int delay_ms) noexcept {
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

void Helper::exit_gracefully() noexcept {
  log("Attempting to exit gracefully....", LogLevel::Info);
  add_delay();
  exit(EXIT_FAILURE);
}

void Helper::exit_ungracefully() noexcept { exit(EXIT_FAILURE); }

std::vector<std::string> Helper::split_str(const std::string &s, char delim) noexcept {
  std::stringstream stream(s);
  std::string temp;
  std::vector<std::string> splits;

  while (std::getline(stream, temp, delim)) {
    splits.push_back(temp);
  }

  return splits;
}

std::string Helper::get_static_file_path(const std::string &filename) noexcept {
  std::string home_dir = getenv("HOME");
  return std::format("{}/{}/{}", home_dir, STATIC_FILE_DIR_NAME, filename);
}
