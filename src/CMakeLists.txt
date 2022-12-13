add_library(regex_lib
    ./automata/NFA.cpp
    ./automata/DFA.cpp
    ./automata/Bimap.cpp
    ./regex/Regex.cpp
    ./regex/Utf8Iterator.cpp
    ./regex/Parser.cpp
    ./regex/Alphabet.cpp
    )

target_include_directories(regex_lib
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PRIVATE
        ./automata
        ./regex
    )