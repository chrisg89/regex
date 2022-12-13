add_executable(tests
    Alphabet_tests.cpp
    Parser_tests.cpp
    RegexMatch_tests.cpp
    )

target_link_libraries( tests
    PRIVATE
    Catch2::Catch2
    regex_lib
    )

# Allow tests access to private header not exposed by the library
target_include_directories( tests 
    PRIVATE $<TARGET_PROPERTY:regex_lib,INCLUDE_DIRECTORIES>
    )

catch_discover_tests(tests)