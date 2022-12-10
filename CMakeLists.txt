cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  Regex
  VERSION 0.1
  DESCRIPTION "Regex library based on DFA"
  LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(FetchContent)

  # Catch2 for unit testing
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9 
  )
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)

  # Valgrind for dynamic analysis
  set(MEMORYCHECK_COMMAND_OPTIONS "-q --tool=memcheck --leak-check=yes --errors-for-leak-kinds=all --num-callers=25 --track-origins=yes --show-reachable=yes --error-exitcode=1")
  
  include(CTest)
  include(Catch)

  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()

  # Docs only available if this is the main app
  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

# The compiled library code is here
add_subdirectory(src)


