
add_library(regex_lib
    CharacterClassStream.cpp
    DFABuilder.cpp
    Regex.cpp
    ThompsonConstruction.cpp
    TokenStream.cpp
    Utf8Iterator.cpp
    Parser.cpp
    Alphabet.cpp
    )

TARGET_LINK_LIBRARIES( regex_lib
    fa_lib
    )

target_include_directories(regex_lib
    PUBLIC
    ./
    ) 

add_subdirectory(tests)