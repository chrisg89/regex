# Installation and CMake Integration

This library is CMake friendly and developed with modern CMake best practices in mind. The CMake target `Regex::Regex` is exposed with all usage requirements specified.

## Installing regex from git repository
Assuming you have enough rights, you can install regex like so:

```
$ git clone https://github.com/chrisg89/regex.git
$ cd regex
$ cmake -B ./build -S .
$ sudo cmake --build ./build/ --target install
```
If you do not have superuser rights, you will also need to specify CMAKE_INSTALL_PREFIX when configuring the build, and then modify your calls to find_package accordingly.

A successful installation will exit with the following notice:
```
Install the project...
$ -- Install configuration: ""
$ -- Up-to-date: /usr/local/include
$ -- Installing: /usr/local/include/regex
$ -- Installing: /usr/local/include/regex/Regex.hpp
$ -- Installing: /usr/local/lib/libregex_lib.a
$ -- Installing: /usr/local/lib/cmake/Regex/RegexTargets.cmake
$ -- Installing: /usr/local/lib/cmake/Regex/RegexTargets-noconfig.cmake
$ -- Installing: /usr/local/lib/cmake/Regex/RegexConfig.cmake
$ -- Installing: /usr/local/lib/cmake/Regex/RegexConfigVersion.cmake
```

# Integrating regex into your CMake project via cmake's [find_package](https://cmake.org/cmake/help/latest/command/find_package.html)

Note: This method requires the library to be installed.

```
  find_package(Regex 1.0.0 REQUIRED)
  add_executable(fooExe foo.cpp)
  target_link_libraries(fooExe PRIVATE Regex::Regex)
```

# Integrating regex into your CMake project via [fetch_content](https://cmake.org/cmake/help/latest/module/FetchContent.html)

Note: This method **DOES NOT** require the library to be installed. The library will be build as part of the host project alongside all of its other artifacts.

```
Include(FetchContent)

FetchContent_Declare(
  Regex
  GIT_REPOSITORY https://github.com/chrisg89/regex.git
  GIT_TAG        v1.0.0 # check github for latest tag or commit sha
)

FetchContent_MakeAvailable(Regex)

add_executable(fooExe foo.cpp)
target_link_libraries(fooExe PRIVATE Regex::Regex)
```

# Integrating regex into your CMake project as a subdirectory

Note: This method **DOES NOT** require the library to be installed. The library will be build as part of the host project alongside all of its other artifacts.

Clone the regex repo into your project
```
$ git clone https://github.com/chrisg89/regex.git
```

Then add it to your project as a subdirectory

```
  add_subdirectory(regex)
  add_executable(demo demo.cpp)
  target_link_libraries(demo PRIVATE Regex::Regex)
```