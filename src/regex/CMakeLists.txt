
add_library(regex_lib
    Regex.cpp
    Utf8Iterator.cpp
    Parser.cpp
    Alphabet.cpp
    )

TARGET_LINK_LIBRARIES( regex_lib
    automata_lib
    )

target_include_directories(regex_lib
    PUBLIC
    ./
    )