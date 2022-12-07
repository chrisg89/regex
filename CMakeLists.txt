# Works with 3.14 and tested through 3.25
cmake_minimum_required(VERSION 3.14...3.25)

# Project name and a few useful settings. Other commands can pick up the results
project(
  Regex
  VERSION 0.1
  DESCRIPTION "Regex library based on DFA"
  LANGUAGES CXX)


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # FetchContent added in CMake 3.11, downloads during the configure step
  # FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.1
  )
  FetchContent_MakeAvailable(Catch2)

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
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


