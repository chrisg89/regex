

add_executable(regex_tests
    Alphabet_tests.cpp
    Parser_tests.cpp
    Regex_tests.cpp
    RegexMatch_tests.cpp
    RegexSearch_tests.cpp
    )

TARGET_LINK_LIBRARIES( regex_tests
    catch
    regex_lib
    )

    