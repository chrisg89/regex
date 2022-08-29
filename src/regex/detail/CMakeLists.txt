
add_library(regex_detail_lib
    DFABuilder.cpp
    Regex.cpp
    ThompsonConstruction.cpp
    TokenStream.cpp
    )

TARGET_LINK_LIBRARIES(regex_detail_lib
    fa_lib
    )

target_include_directories(regex_detail_lib
    PUBLIC
    ./
    ) 

add_subdirectory(tests)