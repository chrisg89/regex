
add_library(regex_lib
    Regex.cpp)

TARGET_LINK_LIBRARIES( regex_lib
    fa_lib
    )

target_include_directories(regex_lib
    PUBLIC
    ./
    ) 

add_subdirectory(tests)