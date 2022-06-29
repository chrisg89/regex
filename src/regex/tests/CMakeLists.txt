

add_executable(regex_tests 
    Regex_tests.cpp
    TokenStream_test.cpp)

TARGET_LINK_LIBRARIES( regex_tests
    catch
    regex_lib
    )

    