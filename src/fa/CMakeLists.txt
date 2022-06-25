
add_library(fa_lib
    NFA.cpp
    DFA.cpp
    Bimap.cpp)

TARGET_LINK_LIBRARIES( fa_lib
    )

target_include_directories(fa_lib
    PUBLIC
    ./
    ) 

add_subdirectory(tests)