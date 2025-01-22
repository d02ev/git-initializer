#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace constants {
  inline const std::string LIST_IGNORES_ARG = "lsig";
  inline const std::string LIST_AVAILABLE_CMDS_ARG = "lsc";
  inline const std::string IGN_FLAG = "--ignore";
  inline const std::string USER_FLAG = "--user";
  inline const std::string KEY_FLAG = "--key";
  inline const std::string INIT_ARG = "init";
  inline const std::string ADD_ARG = "add";

  inline const std::string GIT_USER = "GIT_USER";
  inline const std::string GIT_KEY = "GIT_KEY";
  inline const std::string GIT_KEY_AND_USER = "GIT_KEY and GIT_USER";
  inline const std::string GIT_API_BASE_URL = "https://api.github.com";
  inline const std::string GIT_HOME_BASE_URL = "https://github.com";
  inline const std::string GIT_API_AUTH_HEADER_NAME = "Authorization";
  inline const std::string GIT_API_AUTH_HEADER_VALUE = "Bearer";
  inline const std::string GIT_API_ACCEPT_HEADER_NAME = "Accept";
  inline const std::string GIT_API_ACCEPT_HEADER_VALUE = "application/vnd.github+json";
  inline const std::string GIT_API_VERSION_HEADER_NAME = "X-GitHub-Api-Version";
  inline const std::string GIT_API_VERSION_HEADER_VALUE = "2022-11-28";
  inline const std::string GIT_CONTENT_TYPE_HEADER_NAME = "Content-Type";
  inline const std::string GIT_CONTENT_TYPE_HEADER_VALUE = "application/json";

  inline const std::string ERROR_CLR = "\033[31m";
  inline const std::string INFO_CLR = "\033[34m";
  inline const std::string SUCCESS_CLR = "\033[32m";
  inline const std::string RESET = "\033[0m";

  inline const std::string DEFAULT_IGN_FILES = "visualstudiocode,dotenv,linux";
  inline const std::string IGN_FLAG_DEFAULT = "visualstudiocode,dotenv,linux";
  inline const std::string IGN_FILE_NAME = ".gitignore";
  inline const std::string README_FILE_NAME = "README.md";
  inline const std::string LICENSE_FILE_NAME = "LICENSE.md";
  inline const std::string STATIC_FILE_DIR_NAME_ON_INSTALL = ".git-initializer";
  inline const std::string IGN_JSON_FILE_NAME = "ignores.json";
  inline const std::string VALID_IGN_LIST_FILE_NAME = "valid_ignores_list.txt";
  inline const std::string HELP_MSG_FILE_NAME = "help_msg.txt";
  inline const std::string CONFIG_FILE_NAME = ".gitinitializerconfig";
  inline const std::string VALID_ARGS[4] = {"lsig", "lsc", "init", "add"};
  inline const std::string VALID_FLAGS[3] = {"--ig", "--key", "--user"};
  inline const std::string ENV_ADD_HELPER_MSG = "Please run: git-initializer add --key=<key> --user=<user> to add "
                                                    "<key> and <user> before initializing a new repository";

  enum class LogLevel { Error, Info, Internal, Success };
  enum class EnvType { GitKey, GitUser, GitKeyAndUser };
}; // namespace constants

#endif // CONSTANTS_H
