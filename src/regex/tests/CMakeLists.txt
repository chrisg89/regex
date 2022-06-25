

add_executable(regex_tests 
    Regex_tests.cpp)

TARGET_LINK_LIBRARIES( regex_tests
    catch
    regex_lib
    )

    