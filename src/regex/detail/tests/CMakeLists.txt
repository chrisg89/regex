
add_library(regex_detail_tests
    Regex_tests.cpp
    TokenStream_test.cpp
    ThompsonConstruction_test.cpp
    )

TARGET_LINK_LIBRARIES(regex_detail_tests
    catch
    regex_detail_lib
    )

target_include_directories(regex_detail_tests
    PUBLIC
    ./
    ) 
    