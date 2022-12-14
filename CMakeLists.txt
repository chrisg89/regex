cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    Regex
    VERSION 0.1
    DESCRIPTION "Regex library based on DFA"
    LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(FetchContent)

    # Catch2 for unit testing
    FetchContent_Declare(
        Catch2
        SYSTEM ON # catch2 header is added as system header (cmake 3.25)
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

    if(ENABLE_COVERAGE)

        # find required tools
        find_program(LCOV lcov REQUIRED)
        find_program(GENHTML genhtml REQUIRED)
    
        # add coverage target
        # only contents of the '/src' directory will be included in the coverage report
        add_custom_target(
        coverage
        COMMAND ${LCOV} --capture --no-external --directory ${CMAKE_BINARY_DIR}/src --base-directory ${PROJECT_SOURCE_DIR}/src --output-file coverage.info
        COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

    endif()


endif()

# The compiled library code is here
add_subdirectory(src)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Regex)

install(
    DIRECTORY include/ 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS regex_lib
    EXPORT regex-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT regex-targets
    FILE RegexTargets.cmake
    NAMESPACE Regex::
    DESTINATION ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RegexConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/RegexConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RegexConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RegexConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/RegexConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree

export(
    EXPORT regex-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/RegexTargets.cmake
    NAMESPACE Regex::
)