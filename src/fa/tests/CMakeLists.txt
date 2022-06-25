

add_executable(fa_tests 
    fa_tests.cpp)

TARGET_LINK_LIBRARIES( fa_tests
    catch
    fa_lib
    )

    