#ifndef CUSTOM_ERRORS_H
#define CUSTOM_ERRORS_H

#include <exception>
#include <string>

namespace custom_errors {
  class ConfigAlreadyExistsException : public std::exception {
    std::string _message;

  public:
    explicit ConfigAlreadyExistsException(const std::string &msg) : _message(msg) {};

    const char *what() const noexcept override { return _message.c_str(); }
  };

  class ConfigNotFoundException final : public std::exception {
    std::string _message;

  public:
    explicit ConfigNotFoundException(const std::string &msg) : _message(msg) {};

    const char *what() const noexcept override { return _message.c_str(); }
  };

  class ConfigEmptyException final : public std::exception {
    std::string _message;

  public:
    explicit ConfigEmptyException(const std::string &msg) : _message(msg) {};

    const char *what() const noexcept override { return _message.c_str(); }
  };

  class InternalException final : public std::exception {
    std::string _message;

  public:
    explicit InternalException(const std::string &msg) : _message(msg) {};

    const char *what() const noexcept override { return _message.c_str(); }
  };
}; // namespace custom_errors

#endif // CUSTOM_ERRORS_H
