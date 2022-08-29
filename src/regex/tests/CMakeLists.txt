

add_executable(regex_tests 
    basic_regex_tests.cpp
    match_results_tests.cpp
    regex_constants_tests.cpp
    regex_match_tests.cpp
    regex_traits_tests.cpp
    sub_match_tests.cpp
    )

TARGET_LINK_LIBRARIES( regex_tests
    catch
    regex_lib
    regex_detail_tests
    )

    