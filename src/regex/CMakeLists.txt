
add_library(regex_lib
    dummy.cpp
    )

TARGET_LINK_LIBRARIES( regex_lib
    regex_detail_lib
    )

target_include_directories(regex_lib
    PUBLIC
    ./
    ) 

add_subdirectory(detail)
add_subdirectory(tests)