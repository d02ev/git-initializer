#include "git_initializer/parser.h"
#include <format>
#include <fstream>
#include <iostream>
#include <json/value.h>
#include <vector>

#include <json/writer.h>

#include "git_initializer/helper.h"
#include "git_initializer/utils.h"

void ArgParser::parse_add_arg(const std::string &git_key, const std::string &git_user) noexcept {
  auto [git_key_empty, git_key_msg] = Utils::has_empty_arg_value(GIT_KEY, git_key);
  auto [git_user_empty, git_user_msg] = Utils::has_empty_arg_value(GIT_USER, git_user);

  if (!git_key_empty && !git_user_empty) {
    Helper::log(std::format("Checking if {} and {} already exist in the user profile....", GIT_KEY, GIT_USER),
                LogLevel::Info);
    Helper::add_delay();

    if (auto [env_exists, env] = Helper::check_env_existence(); env_exists) {
      Helper::log(std::format("{} already exist in the user profile", env), LogLevel::Info);
      Helper::exit_gracefully();
    }

    Helper::log(std::format("Adding {} and {} to the user profile....", GIT_KEY, GIT_USER), LogLevel::Info);
    Helper::add_delay();

    const std::string home_dir = getenv("HOME");
    const std::string profile_path = home_dir + "/.bashrc";

    std::ofstream file(profile_path, std::ios::out);
    if (!file.is_open()) {
      Helper::log(std::format("Failed to open file at: {}", profile_path), LogLevel::Error);
      Helper::exit_gracefully();
    }

    file << std::format("\nexport {}={}\nexport {}={}", GIT_KEY, git_key, GIT_USER, git_user);
    file.close();

    Helper::add_delay();
    Helper::log(std::format("{} and {} successfully added to user profile", GIT_KEY, GIT_USER), LogLevel::Success);
    Helper::log("To apply changes immediately, run: ", LogLevel::Info);
    Helper::log(std::format("source {}", profile_path), LogLevel::Info);
  }
}

void ArgParser::parse_init_arg(const std::string &ign_arg_val) noexcept {
  Helper::log(std::format("Checking if {} and {} are added to the user profile....", GIT_KEY, GIT_USER),
              LogLevel::Info);
  Helper::add_delay();

  if (auto [env_exists, env] = Utils::env_exists(); env_exists) {
    std::string ign_files;
    if (!ign_arg_val.empty()) {
      // TODO: Implement `trim()` to remove trailing whitespaces
      ign_files = ign_arg_val;
    } else {
      Helper::log(std::format("No ignore files specified - defaulting to: {}", DEFAULT_IGN_FILES), LogLevel::Info);
      ign_files = DEFAULT_IGN_FILES;
    }

    std::vector<std::string> ign_file_names = Helper::split_str(ign_files, ',');
    std::string ign_file_content;
    Json::Value ign_files_json = Utils::read_ignores_json();

    for (const std::string &file_name: ign_file_names) {
      if (ign_files_json[file_name].isNull()) {
        Helper::log(std::format("Invalid ignore file: {}", file_name), LogLevel::Error);
        Helper::log("Please run: git-initializer lsig to see available ignore files", LogLevel::Info);
        Helper::exit_gracefully();
      }

      ign_file_content += ign_files_json[file_name]["contents"].asString();
    }

    Utils::create_readme_and_gitignore(ign_file_content);
    Helper::add_delay();

    Utils::create_remote_repo();
    Helper::add_delay();

    Utils::run_git_commands();
    Helper::add_delay();

    Helper::log("Done!", LogLevel::Success);
  } else {
    Helper::log(std::format("{} not set", env), LogLevel::Error);
    Helper::log(ENV_ADD_HELPER_MSG, LogLevel::Info);
    Helper::exit_gracefully();
  }
}

void ArgParser::parse_list_valid_ignore_files_arg() noexcept {
  Helper::log("Here\'s a list of valid ignore files: ", LogLevel::Info);

  std::vector<std::string> valid_ign_files = Utils::read_valid_ignores_txt();

  for (const std::string &file_name: valid_ign_files) {
    std::cout << file_name << std::endl;
  }
}
