#include <filesystem>
#include <format>
#include <fstream>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <unordered_map>
#include "git_initializer/constants.h"
#include "git_initializer/utils.h"
#include "git_initializer/helper.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

std::pair<bool, std::string> Utils::has_correct_arg_count(const std::string &arg_name, const int arg_count) noexcept {
  if (arg_name == ADD_ARG) {
    if (arg_count < 3) {
      return {false, std::format("Too few arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    if (arg_count > 4) {
      return {false, std::format("Too many arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    return {true, ""};
  }
  if (arg_name == INIT_ARG) {
    if (arg_count > 3) {
      return {false, std::format("Too many arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    return {true, ""};
  }
  if (arg_name == LIST_IGNORES_ARG || arg_name == LIST_AVAILABLE_CMDS_ARG) {
    if (arg_count > 2) {
      return {false, std::format("Too many arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    return {true, ""};
  }

  return {false, std::format("Invalid argument: {}", arg_name)};
}

std::pair<bool, std::string> Utils::has_empty_arg_value(const std::string &arg_name,
                                                        const std::string &arg_value) noexcept {
  if (arg_value.empty()) {
    return {true, std::format("{} cannot have empty value", arg_name)};
  }

  return {false, ""};
}

std::pair<bool, std::string> Utils::invalid_arg_name(const std::string &arg_name) noexcept {
  for (const std::string &arg: VALID_ARGS) {
    if (arg_name == arg) {
      return {true, ""};
    }
  }

  return {false, std::format("Invalid argument: {}", arg_name)};
}

std::pair<bool, std::string> Utils::invalid_flag_name(const std::string &flag_name) noexcept {
  for (const std::string &flag: VALID_FLAGS) {
    if (flag_name == flag) {
      return {true, ""};
    }
  }

  return {false, std::format("Invalid flag: {}", flag_name)};
}

std::pair<bool, std::string> Utils::env_exists() noexcept {
  auto [env_exists, env] = Helper::check_env_existence();

  if (!env_exists) {
    return {env_exists, std::format("{} not set", env)};
  }

  return {env_exists, ""};
}

void Utils::create_readme_and_gitignore(const std::string &ign_file_content) noexcept {
  std::string dir = std::filesystem::current_path().string();
  dir = Helper::sanitize_dir_name(dir);

  Helper::log("Creating .gitignore file....", LogLevel::Info);
  Helper::add_delay();

  std::ofstream git_ignore_file(IGN_FILE_NAME);
  if (!git_ignore_file.good()) {
    Helper::log("Failed to create .gitignore file in " + dir, LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log(".gitignore file created in " + dir, LogLevel::Success);
  Helper::add_delay();

  std::ofstream readme_file(README_FILE_NAME);
  if (!readme_file.good()) {
    Helper::log("Failed to create readme file in " + dir, LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log("README.md file created in " + dir, LogLevel::Success);
  Helper::add_delay();

  git_ignore_file << ign_file_content;
  readme_file << "# " + dir + "\n";

  git_ignore_file.close();
  readme_file.close();
}

Json::Value Utils::read_ignores_json() noexcept {
  std::string ign_json_file_path = Helper::get_static_file_path(IGN_JSON_FILE_NAME);
  std::ifstream ign_json_file(ign_json_file_path);

  if (!ign_json_file.good()) {
    Helper::log("An internal error occurred", LogLevel::Internal);
    Helper::exit_gracefully();
  }

  Json::Value ignores_json;
  Json::Reader ignores_json_reader;

  ignores_json_reader.parse(ign_json_file, ignores_json);
  ign_json_file.close();

  return ignores_json;
}

std::vector<std::string> Utils::read_valid_ignores_txt() noexcept {
  std::string ign_txt_file_path = Helper::get_static_file_path(VALID_IGN_LIST_FILE_NAME);
  std::ifstream ign_txt_file(ign_txt_file_path);

  if (!ign_txt_file.good()) {
    Helper::log("An internal error occurred", LogLevel::Internal);
    Helper::exit_gracefully();
  }

  std::vector<std::string> valid_ign_files;
  std::string words;

  while (ign_txt_file >> words) {
    valid_ign_files.push_back(words);
  }

  ign_txt_file.close();
  return valid_ign_files;
}

void Utils::read_help_msg_txt() noexcept {
  std::string help_msg_txt_file_path = Helper::get_static_file_path(HELP_MSG_FILE_NAME);
  std::ifstream help_msg_file(help_msg_txt_file_path);

  if (!help_msg_file.good()) {
    Helper::log("An internal error occurred", LogLevel::Internal);
    Helper::exit_gracefully();
  }

  std::string line;
  while (std::getline(help_msg_file, line)) {
    std::cout << line << std::endl;
  }

  help_msg_file.close();
}

void Utils::create_remote_repo() noexcept {
  Helper::log("Creating remote_repo...", LogLevel::Info);
  Helper::add_delay();

  std::string repo_name = Helper::sanitize_dir_name(std::filesystem::current_path().string());
  std::string git_key = getenv(GIT_KEY.c_str()), git_user = getenv(GIT_USER.c_str());

  httplib::Client client(GIT_API_BASE_URL);
  const httplib::Headers headers = {
      {GIT_CONTENT_TYPE_HEADER_NAME, GIT_CONTENT_TYPE_HEADER_VALUE},
      {GIT_API_ACCEPT_HEADER_NAME, GIT_API_ACCEPT_HEADER_VALUE},
      {GIT_API_AUTH_HEADER_NAME, std::format("{} {}", GIT_API_AUTH_HEADER_VALUE, git_key)},
      {GIT_API_VERSION_HEADER_NAME, GIT_API_VERSION_HEADER_VALUE}};
  std::unordered_map<std::string, std::string> req_data = {
      {"name", repo_name},
      {"private", std::to_string(false)},
      {"homepage", std::format("{}/{}/{}", GIT_HOME_BASE_URL, git_user, repo_name)}};

  Json::Value req_json_data;
  for (const auto &[key, value]: req_data) {
    req_json_data[key] = value;
  }

  Json::StreamWriterBuilder writer;
  std::string req_body = Json::writeString(writer, req_json_data);

  httplib::Result res = client.Post("/user/repos", headers, req_body.c_str(), GIT_CONTENT_TYPE_HEADER_VALUE);
  if (!res || res->status != 201) {
    Helper::log("Failed to create remote repository", LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log(std::format("{} created successfully", repo_name), LogLevel::Success);
}

void Utils::run_git_commands() noexcept {
  std::string dir_name = Helper::sanitize_dir_name(std::filesystem::current_path().string());
  std::string git_user = getenv(GIT_USER.c_str());

  Helper::log("Running git commands....", LogLevel::Info);
  Helper::add_delay();

  Helper::log("Initializing the local repository....", LogLevel::Info);
  Helper::add_delay();

  int git_init_res_code = std::system("git init -b master");
  if (git_init_res_code != 0) {
    Helper::log("Failed to initialize the local repository", LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log("Initialized the local repository successfully", LogLevel::Success);
  Helper::add_delay();
  Helper::log("Adding remote origin", LogLevel::Info);
  Helper::add_delay();

  int git_add_origin_res_code =
      std::system(std::format("git remote add origin {}/{}/{}.git", GIT_HOME_BASE_URL, git_user, dir_name).c_str());
  if (git_add_origin_res_code != 0) {
    Helper::log("Failed to add remote origin", LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log("Remote origin added successfully", LogLevel::Success);
  Helper::add_delay();
}
