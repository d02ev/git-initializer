add_executable(git-initializer main.cpp
        utils.cpp
        helper.cpp
        parser/parser.cpp
        cmd/add.cpp
        cmd/init.cpp)

target_link_libraries(git-initializer PRIVATE JsonCpp::JsonCpp httplib::httplib OpenSSL::SSL OpenSSL::Crypto)