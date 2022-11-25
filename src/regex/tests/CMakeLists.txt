

add_executable(regex_tests 
    Regex_tests.cpp
    Parser_test.cpp
    ThompsonConstruction_test.cpp
    )

TARGET_LINK_LIBRARIES( regex_tests
    catch
    regex_lib
    )

    