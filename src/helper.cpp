#include "git_initializer/helper.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "git_initializer/constants.h"


bool Helper::check_env_config_existence(constants::EnvType env_type) noexcept {
  const char *git_key_env = getenv(constants::GIT_KEY.c_str()), *git_user_env = getenv(constants::GIT_USER.c_str());
  if (env_type == constants::EnvType::GitKeyAndUser) {
    return git_key_env != nullptr && git_user_env != nullptr;
  }
  if (env_type == constants::EnvType::GitUser) {
    return git_user_env != nullptr;
  }
  if (env_type == constants::EnvType::GitKey) {
    return git_key_env != nullptr;
  }

  return false;
}

std::string Helper::sanitize_dir_name(const std::string &dir_name) noexcept {
  const int dir_name_size = dir_name.length();
  int i = dir_name_size - 1;
  std::string sanitized_dir_name;

  while (i >= 0 && dir_name.at(i) != '/') {
    sanitized_dir_name.push_back(dir_name.at(i--));
  }

  return sanitized_dir_name;
}

void Helper::log(const std::string &err_msg, const constants::LogLevel log_level) noexcept {
  std::string colored_msg;

  switch (log_level) {
    case constants::LogLevel::Error:
      colored_msg = constants::ERROR_CLR + err_msg + constants::RESET;
      break;
    case constants::LogLevel::Info:
      colored_msg = constants::INFO_CLR + err_msg + constants::RESET;
      break;
    case constants::LogLevel::Success:
      colored_msg = constants::SUCCESS_CLR + err_msg + constants::RESET;
      break;
    case constants::LogLevel::Internal:
      colored_msg = constants::ERROR_CLR + err_msg + constants::RESET;
      break;
  }

  std::cout << colored_msg << std::endl;
}

void Helper::add_delay(const int delay_ms) noexcept {
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

void Helper::exit_gracefully() noexcept {
  log("Attempting to exit gracefully....", constants::LogLevel::Info);
  add_delay();
  exit(EXIT_FAILURE);
}

void Helper::exit_ungracefully() noexcept { exit(EXIT_FAILURE); }

std::vector<std::string> Helper::split_str(const std::string &s, const char delim) noexcept {
  std::vector<std::string> splits;
  std::string split_str;

  for (int i = 0; i < s.size(); ++i) {
    if (s.at(i) == delim || i == s.size() - 1) {
      if (i == s.size() - 1) {
        split_str.push_back(s.at(i));
      }
      splits.emplace_back(split_str);
      split_str = "";
    } else {
      split_str.push_back(s.at(i));
    }
  }

  return splits;
}

std::string Helper::get_static_file_path(const std::string &filename) noexcept {
  std::string home_dir = getenv("HOME");
  return std::format("{}/{}/{}", home_dir, constants::STATIC_FILE_DIR_NAME_ON_INSTALL, filename);
}

bool Helper::to_bool(const std::string &str) noexcept { return std::stoi(str) != 0; }
