
add_library(automata_lib
    NFA.cpp
    DFA.cpp
    Bimap.cpp)

TARGET_LINK_LIBRARIES( automata_lib
    )

target_include_directories(automata_lib
    PUBLIC
    ./
    ) 

add_subdirectory(tests)