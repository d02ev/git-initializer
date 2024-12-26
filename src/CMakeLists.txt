add_executable(git-initializer main.cpp constants.cpp
        utils.cpp
        helper.cpp
        parser.cpp)

target_link_libraries(git-initializer PRIVATE JsonCpp::JsonCpp httplib::httplib OpenSSL::SSL OpenSSL::Crypto)