cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_BUILD_TYPE Debug)

# set the project name
project(Regex)

add_subdirectory(src/automata)
add_subdirectory(src/regex)
add_subdirectory(src/catch)
 