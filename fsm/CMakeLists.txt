
add_library(fsm_lib
    NFA.cpp
    DFA.cpp
    Bimap.cpp)

TARGET_LINK_LIBRARIES( fsm_lib
    )

target_include_directories(fsm_lib
    PUBLIC
    ./
    ) 

add_subdirectory(tests)