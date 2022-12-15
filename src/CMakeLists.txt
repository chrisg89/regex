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

# For consistent use across find_package() and fetch_content()
# https://www.youtube.com/watch?v=bsXLMQ6WgIk&t=3126s
add_library(Regex::Regex ALIAS regex_lib)

#This is required so that the exported target has the name Regex
set_target_properties(regex_lib PROPERTIES EXPORT_NAME Regex)