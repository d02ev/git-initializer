#include "git_initializer/constants.h"

const std::string GIT_USER = "GIT_USER";
const std::string GIT_KEY = "GIT_KEY";
const std::string LIST_IGNORES_ARG = "lsig";
const std::string LIST_AVAILABLE_CMDS_ARG = "lsc";
const std::string IGN_FLAG = "--ig";
const std::string USER_FLAG = "--user";
const std::string KEY_FLAG = "--key";
const std::string INIT_ARG = "init";
const std::string ADD_ARG = "add";

const std::string GIT_API_BASE_URL = "https://api.github.com";
const std::string GIT_HOME_BASE_URL = "https://github.com";
const std::string GIT_API_AUTH_HEADER_NAME = "Authorization";
const std::string GIT_API_AUTH_HEADER_VALUE = "Bearer";
const std::string GIT_API_ACCEPT_HEADER_NAME = "Accept";
const std::string GIT_API_ACCEPT_HEADER_VALUE = "application/vnd.github+json";
const std::string GIT_API_VERSION_HEADER_NAME = "X-GitHub-Api-Version";
const std::string GIT_API_VERSION_HEADER_VALUE = "2022-11-28";
const std::string GIT_CONTENT_TYPE_HEADER_NAME = "Content-Type";
const std::string GIT_CONTENT_TYPE_HEADER_VALUE = "application/json";

const std::string ERROR_CLR = "\033[31m";
const std::string INFO_CLR = "\033[34m";
const std::string SUCCESS_CLR = "\033[32m";
const std::string RESET = "\033[0m";

const std::string DEFAULT_IGN_FILES = "dotenv,visualstudiocode,vs,linux";
const std::string IGN_FLAG_DEFAULT = "dotenv,visualstudiocode,vs,linux";
const std::string IGN_FILE_NAME = ".gitignore";
const std::string README_FILE_NAME = "README.md";
const std::string STATIC_FILE_DIR_NAME = ".git-initializer";
const std::string IGN_JSON_FILE_NAME = "ignores.json";
const std::string VALID_IGN_LIST_FILE_NAME = "valid_ignores_list.txt";
const std::string HELP_MSG_FILE_NAME = "help_msg.txt";

const std::string VALID_ARGS[4] = {"lsig", "lsc", "init", "add"};
const std::string VALID_FLAGS[3] = {"--ig", "--key", "--user"};

const std::string ENV_ADD_HELPER_MSG =
    "Please run: git-initializer add --key=<key> --user=<user> to add <key> and <user> before initializing a new repository";
