

add_executable(fsm_tests 
    FSM_tests.cpp)

TARGET_LINK_LIBRARIES( fsm_tests
    catch
    fsm_lib
    )

    