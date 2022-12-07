add_executable(tests
    Alphabet_tests.cpp
    Parser_tests.cpp
    Regex_tests.cpp
    RegexMatch_tests.cpp
    RegexSearch_tests.cpp
    )

TARGET_LINK_LIBRARIES( tests
    PRIVATE
    Catch2::Catch2WithMain
    regex_lib
    )