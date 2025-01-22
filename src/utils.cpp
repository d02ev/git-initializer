#include <filesystem>
#include <format>
#include <fstream>
#include <git_initializer/constants.h>
#include <git_initializer/helper.h>
#include <git_initializer/utils.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <regex>
#include <unordered_map>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

std::pair<bool, std::string> Utils::has_correct_arg_count(const std::string &arg_name, const int arg_count) noexcept {
  if (arg_name == constants::ADD_ARG) {
    if (arg_count < 3) {
      return {false, std::format("Too few arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    if (arg_count > 4) {
      return {false, std::format("Too many arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    return {true, ""};
  }
  if (arg_name == constants::INIT_ARG) {
    if (arg_count > 3) {
      return {false, std::format("Too many arguments: {} for {}", std::to_string(arg_count), arg_name)};
    }
    return {true, ""};
  }
  if (arg_name == constants::LIST_IGNORES_ARG || arg_name == constants::LIST_AVAILABLE_CMDS_ARG) {
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
  for (const std::string &arg : constants::VALID_ARGS) {
    if (arg_name == arg) {
      return {true, ""};
    }
  }

  return {false, std::format("Invalid argument: {}", arg_name)};
}

std::pair<bool, std::string> Utils::invalid_flag_name(const std::string &flag_name) noexcept {
  for (const std::string &flag : constants::VALID_FLAGS) {
    if (flag_name == flag) {
      return {true, ""};
    }
  }

  return {false, std::format("Invalid flag: {}", flag_name)};
}

std::pair<bool, std::string> Utils::env_exists() noexcept {
  bool env_exists = Helper::check_env_config_existence(constants::EnvType::GitKeyAndUser);

  if (!env_exists) {
    return {env_exists, std::format("{} not set", constants::GIT_KEY_AND_USER)};
  }

  return {env_exists, ""};
}

void Utils::export_env_config(constants::EnvType env_type, const std::string &git_key,
                              const std::string &git_user) noexcept {
  const std::string home_dir = getenv("HOME");
  const std::string git_initializer_profile_path = std::format("{}/{}", home_dir, constants::CONFIG_FILE_NAME);
  std::ofstream file(git_initializer_profile_path, std::ios::out | std::ios::app);

  if (!file.is_open()) {
    Helper::log(std::format("Failed to open file at: {}", git_initializer_profile_path), constants::LogLevel::Error);
    Helper::exit_gracefully();
  }

  if (env_type == constants::EnvType::GitKeyAndUser) {
    file << std::format("\nexport {}={}\nexport {}={}", constants::GIT_KEY, git_key, constants::GIT_USER, git_user);
    file.close();
    Helper::add_delay();
    Helper::log(std::format("{} successfully added", constants::GIT_KEY_AND_USER), constants::LogLevel::Success);
  } else if (env_type == constants::EnvType::GitKey) {
    file << std::format("\nexport {}={}\n", constants::GIT_KEY, git_key);
    file.close();
    Helper::add_delay();
    Helper::log(std::format("{} successfully added", constants::GIT_KEY), constants::LogLevel::Success);

  } else if (env_type == constants::EnvType::GitUser) {

    file << std::format("\nexport {}={}\n", constants::GIT_USER, git_user);
    file.close();
    Helper::add_delay();
    Helper::log(std::format("{} successfully added", constants::GIT_USER), constants::LogLevel::Success);
  }

  int source_gitinitializerconfig = system(std::format(". ~/{}", constants::CONFIG_FILE_NAME).c_str());
  if (source_gitinitializerconfig == -1) {
    Helper::log(std::format("Failed to source {}", constants::CONFIG_FILE_NAME), constants::LogLevel::Error);
    Helper::log(std::format("Run source ~/{}", constants::CONFIG_FILE_NAME), constants::LogLevel::Info);
  }

  file.close();
  Helper::log("To apply changes, add the following line to your .bashrc file: ", constants::LogLevel::Info);
  Helper::log("if [-f ~/.gitinitializerconfig]; then", constants::LogLevel::Info);
  Helper::log(" source ~/.gitinitializerconfig;", constants::LogLevel::Info);
  Helper::log("fi", constants::LogLevel::Info);
  Helper::log("then run source ~/.bashrc to immediately apply the changes", constants::LogLevel::Info);
}

void Utils::create_readme_and_gitignore(const std::string &ign_file_content) noexcept {
  std::string dir = std::filesystem::current_path().string();
  dir = Helper::sanitize_dir_name(dir);

  Helper::log("Creating .gitignore file....", constants::LogLevel::Info);
  Helper::add_delay();

  std::ofstream git_ignore_file(constants::IGN_FILE_NAME);
  if (!git_ignore_file.good()) {
    Helper::log("Failed to create .gitignore file in " + dir, constants::LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log(".gitignore file created in " + dir, constants::LogLevel::Success);
  Helper::add_delay();

  std::ofstream readme_file(constants::README_FILE_NAME);
  if (!readme_file.good()) {
    Helper::log("Failed to create readme file in " + dir, constants::LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log("README.md file created in " + dir, constants::LogLevel::Success);
  Helper::add_delay();

  git_ignore_file << ign_file_content;
  readme_file << "# " + dir + "\n";

  git_ignore_file.close();
  readme_file.close();
}

Json::Value Utils::read_ignores_json() noexcept {
  std::string ign_json_file_path = Helper::get_static_file_path(constants::IGN_JSON_FILE_NAME);
  std::ifstream ign_json_file(ign_json_file_path);

  if (!ign_json_file.good()) {
    Helper::log("An internal error occurred", constants::LogLevel::Internal);
    Helper::exit_gracefully();
  }

  Json::Value ignores_json;
  Json::Reader ignores_json_reader;

  ignores_json_reader.parse(ign_json_file, ignores_json);
  ign_json_file.close();

  return ignores_json;
}

std::vector<std::string> Utils::read_valid_ignores_txt() noexcept {
  std::string ign_txt_file_path = Helper::get_static_file_path(constants::VALID_IGN_LIST_FILE_NAME);
  std::ifstream ign_txt_file(ign_txt_file_path);

  if (!ign_txt_file.good()) {
    Helper::log("An internal error occurred", constants::LogLevel::Internal);
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
  std::string help_msg_txt_file_path = Helper::get_static_file_path(constants::HELP_MSG_FILE_NAME);
  std::ifstream help_msg_file(help_msg_txt_file_path);

  if (!help_msg_file.good()) {
    Helper::log("An internal error occurred", constants::LogLevel::Internal);
    Helper::exit_gracefully();
  }

  std::string line;
  while (std::getline(help_msg_file, line)) {
    std::cout << line << std::endl;
  }

  help_msg_file.close();
}

void Utils::create_remote_repo() noexcept {
  Helper::log("Creating remote_repo...", constants::LogLevel::Info);
  Helper::add_delay();

  std::string repo_name = Helper::sanitize_dir_name(std::filesystem::current_path().string());
  std::string git_key = getenv(constants::GIT_KEY.c_str()), git_user = getenv(constants::GIT_USER.c_str());

  httplib::Client client(constants::GIT_API_BASE_URL);
  const httplib::Headers headers = {
      {constants::GIT_CONTENT_TYPE_HEADER_NAME, constants::GIT_CONTENT_TYPE_HEADER_VALUE},
      {constants::GIT_API_ACCEPT_HEADER_NAME, constants::GIT_API_ACCEPT_HEADER_VALUE},
      {constants::GIT_API_AUTH_HEADER_NAME, std::format("{} {}", constants::GIT_API_AUTH_HEADER_VALUE, git_key)},
      {constants::GIT_API_VERSION_HEADER_NAME, constants::GIT_API_VERSION_HEADER_VALUE}};
  std::unordered_map<std::string, std::string> req_data = {
      {"name", repo_name},
      {"private", std::to_string(false)},
      {"homepage", std::format("{}/{}/{}", constants::GIT_HOME_BASE_URL, git_user, repo_name)}};

  Json::Value req_json_data;
  for (const auto &[key, value] : req_data) {
    if (key == "private")
      req_json_data[key] = Helper::to_bool(value);
    req_json_data[key] = value;
  }

  Json::StreamWriterBuilder writer;
  std::string req_body = Json::writeString(writer, req_json_data);

  httplib::Result res = client.Post("/user/repos", headers, req_body.c_str(), constants::GIT_CONTENT_TYPE_HEADER_VALUE);
  if (!res || res->status != 201) {
    Helper::log("Failed to create remote repository", constants::LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log(std::format("{} created successfully", repo_name), constants::LogLevel::Success);
}

void Utils::run_git_commands() noexcept {
  std::string dir_name = Helper::sanitize_dir_name(std::filesystem::current_path().string());
  std::string git_user = getenv(constants::GIT_USER.c_str());

  Helper::log("Running git commands....", constants::LogLevel::Info);
  Helper::add_delay();

  Helper::log("Initializing the local repository....", constants::LogLevel::Info);
  Helper::add_delay();

  int git_init_res_code = std::system("git init -b master");
  if (git_init_res_code != 0) {
    Helper::log("Failed to initialize the local repository", constants::LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log("Initialized the local repository successfully", constants::LogLevel::Success);
  Helper::add_delay();
  Helper::log("Adding remote origin", constants::LogLevel::Info);
  Helper::add_delay();

  int git_add_origin_res_code = std::system(
      std::format("git remote add origin {}/{}/{}.git", constants::GIT_HOME_BASE_URL, git_user, dir_name).c_str());
  if (git_add_origin_res_code != 0) {
    Helper::log("Failed to add remote origin", constants::LogLevel::Error);
    Helper::exit_gracefully();
  }

  Helper::log("Remote origin added successfully", constants::LogLevel::Success);
  Helper::add_delay();
}

std::string Utils::generate_suggestions(const std::string &invalid_file_name) noexcept {
  std::string suggestions;
  const std::regex pattern(std::format("{}.*", invalid_file_name), std::regex_constants::icase);
  std::vector<std::string> valid_ign_file_names = Helper::load_valid_ign_file_names();

  for (const auto &file_name : valid_ign_file_names) {
    if (std::regex_match(file_name, pattern)) {
      suggestions += file_name + ", ";
    }
  }

  return suggestions;
}
