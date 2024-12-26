#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

extern const std::string GIT_USER;
extern const std::string GIT_KEY;
extern const std::string LIST_IGNORES_ARG;
extern const std::string LIST_AVAILABLE_CMDS_ARG;
extern const std::string IGN_FLAG;
extern const std::string USER_FLAG;
extern const std::string KEY_FLAG;
extern const std::string INIT_ARG;
extern const std::string ADD_ARG;

extern const std::string GIT_API_BASE_URL;
extern const std::string GIT_HOME_BASE_URL;
extern const std::string GIT_API_AUTH_HEADER_NAME;
extern const std::string GIT_API_AUTH_HEADER_VALUE;
extern const std::string GIT_API_ACCEPT_HEADER_NAME;
extern const std::string GIT_API_ACCEPT_HEADER_VALUE;
extern const std::string GIT_API_VERSION_HEADER_NAME;
extern const std::string GIT_API_VERSION_HEADER_VALUE;
extern const std::string GIT_CONTENT_TYPE_HEADER_NAME;
extern const std::string GIT_CONTENT_TYPE_HEADER_VALUE;

extern const std::string ERROR_CLR;
extern const std::string INFO_CLR;
extern const std::string SUCCESS_CLR;
extern const std::string RESET;

extern const std::string DEFAULT_IGN_FILES;
extern const std::string IGN_FLAG_DEFAULT;
extern const std::string IGN_FILE_NAME;
extern const std::string README_FILE_NAME;
extern const std::string STATIC_FILE_DIR_NAME;
extern const std::string IGN_JSON_FILE_NAME;
extern const std::string VALID_IGN_LIST_FILE_NAME;
extern const std::string HELP_MSG_FILE_NAME;

extern const std::string VALID_ARGS[4];
extern const std::string VALID_FLAGS[3];

extern const std::string ENV_ADD_HELPER_MSG;

enum class LogLevel {
  Error,
  Info,
  Internal,
  Success
};
#endif //CONSTANTS_H