#ifndef CERRORS_H
#define CERRORS_H

#include <exception>
#include <string>
#include <utility>

class InvalidArgCountException final : public std::exception {
  std::string _msg;

public:
  explicit InvalidArgCountException(std::string  msg) : _msg(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
};

class InvalidArgException final : public std::exception {
  std::string _msg;

public:
  explicit InvalidArgException(std::string msg) : _msg(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
};

class InvalidFlagException final : public std::exception {
  std::string _msg;

public:
  explicit InvalidFlagException(std::string msg) : _msg(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
};

class InvalidIgnoreFileException final : public std::exception {
  std::string _msg;

public:
  explicit InvalidIgnoreFileException(std::string msg) : _msg(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
};

class EmptyArgValueException final : public std::exception {
  std::string _msg;

public:
  explicit EmptyArgValueException(std::string msg) : _msg(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
};

class NoEnvException final : public std::exception {
  std::string _msg;

public:
  explicit NoEnvException(std::string msg) : _msg(std::move(msg)) {}
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
};

#endif //CERRORS_H