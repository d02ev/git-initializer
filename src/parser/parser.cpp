#include <format>
#include <fstream>
#include <git_initializer/helper.h>
#include <git_initializer/parser/parser.h>
#include <git_initializer/utils.h>
#include <iostream>
#include <json/value.h>
#include <json/writer.h>
#include <vector>

void ArgParser::parse_add_arg(const std::string &git_key, const std::string &git_user) noexcept {
  auto [git_key_empty, git_key_msg] = Utils::has_empty_arg_value(constants::GIT_KEY, git_key);
  auto [git_user_empty, git_user_msg] = Utils::has_empty_arg_value(constants::GIT_USER, git_user);

  // if both --key and --user supplied
  if (!git_key_empty && !git_user_empty) {
    Helper::log(std::format("Checking if {} already exists in the user profile....", constants::GIT_KEY_AND_USER),
                constants::LogLevel::Info);
    Helper::add_delay();

    if (bool env_exists = Helper::check_env_config_existence(constants::EnvType::GitKeyAndUser); env_exists) {
      Helper::log(std::format("{} already exist in the user profile", constants::GIT_KEY_AND_USER),
                  constants::LogLevel::Info);
      Helper::exit_gracefully();
    }

    Helper::log(std::format("Adding {} to the user profile....", constants::GIT_KEY_AND_USER),
                constants::LogLevel::Info);
    Helper::add_delay();

    Utils::export_env_config(constants::EnvType::GitKeyAndUser, git_key, git_user);
  }
  // if only --key supplied
  else if (!git_key_empty) {
    Helper::log(std::format("Checking if {} already exists in the user profile....", constants::GIT_KEY),
                constants::LogLevel::Info);
    Helper::add_delay();

    if (bool env_exists = Helper::check_env_config_existence(constants::EnvType::GitKey); env_exists) {
      Helper::log(std::format("{} already exist in the user profile", constants::GIT_KEY), constants::LogLevel::Info);
      Helper::exit_gracefully();
    }

    Helper::log(std::format("Adding {} to the user profile....", constants::GIT_KEY), constants::LogLevel::Info);
    Helper::add_delay();

    Utils::export_env_config(constants::EnvType::GitKey, git_key, git_user);
  }
  // if only --user supplied
  else if (!git_user_empty) {
    Helper::log(std::format("Checking if {} exists in the user profile....", constants::GIT_USER),
                constants::LogLevel::Info);
    Helper::add_delay();

    if (bool env_exists = Helper::check_env_config_existence(constants::EnvType::GitUser); env_exists) {
      Helper::log(std::format("{} already exist in the user profile", constants::GIT_USER), constants::LogLevel::Info);
      Helper::exit_gracefully();
    }

    Helper::log(std::format("Adding {} to the user profile....", constants::GIT_USER), constants::LogLevel::Info);
    Helper::add_delay();

    Utils::export_env_config(constants::EnvType::GitUser, git_key, git_user);
  }
}

void ArgParser::parse_init_arg(const std::string &ign_arg_val) noexcept {
  Helper::log(
      std::format("Checking if {} and {} are added to the user profile....", constants::GIT_KEY, constants::GIT_USER),
      constants::LogLevel::Info);
  Helper::add_delay();

  if (auto [env_exists, env] = Utils::env_exists(); env_exists) {
    std::string ign_files;
    if (!ign_arg_val.empty()) {
      // TODO: Implement `trim()` to remove trailing whitespaces
      ign_files = ign_arg_val;
    } else {
      Helper::log(std::format("No ignore files specified - defaulting to: {}", constants::DEFAULT_IGN_FILES),
                  constants::LogLevel::Info);
      ign_files = constants::DEFAULT_IGN_FILES;
    }

    std::vector<std::string> ign_file_names = Helper::split_str(ign_files, ',');
    std::string ign_file_content;
    Json::Value ign_files_json = Utils::read_ignores_json();

    for (const std::string &file_name : ign_file_names) {
      if (ign_files_json[file_name].isNull()) {
        Helper::log(std::format("Invalid ignore file: {}", file_name), constants::LogLevel::Error);
        Helper::log("Please run: git-initializer lsig to see available ignore files", constants::LogLevel::Info);
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

    Helper::log("Done!", constants::LogLevel::Success);
  } else {
    Helper::log(std::format("{} not set", env), constants::LogLevel::Error);
    Helper::log(constants::ENV_ADD_HELPER_MSG, constants::LogLevel::Info);
    Helper::exit_gracefully();
  }
}

void ArgParser::parse_list_valid_ignore_files_arg() noexcept {
  Helper::log("Here\'s a list of valid ignore files: ", constants::LogLevel::Info);

  std::vector<std::string> valid_ign_files = Utils::read_valid_ignores_txt();

  for (const std::string &file_name : valid_ign_files) {
    std::cout << file_name << std::endl;
  }
}
