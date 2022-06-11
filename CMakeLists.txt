cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_BUILD_TYPE Debug)

# set the project name
project(Lexer)

# add the executable
add_executable(Lexer 
    main.cpp)

TARGET_LINK_LIBRARIES( Lexer
    )

add_subdirectory(fsm)
add_subdirectory(regex)
add_subdirectory(catch)
 