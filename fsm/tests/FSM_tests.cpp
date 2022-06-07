#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "FSM.hpp"


TEST_CASE( "FSM API", "[api]" ) {

    FSM fsm;

    auto& state1 = fsm.addState(true, false);
    auto& state2 = fsm.addState(false, false);
    auto& state3 = fsm.addState(false, false);

    REQUIRE((state1 == fsm.startState())); //TODO sm.startState() error if no start state specified
    REQUIRE((state2 != fsm.startState()));
    REQUIRE((state3 != fsm.startState()));

    REQUIRE((state1 == state1));
    REQUIRE((state1 != state2));
    REQUIRE((state1 != state3));

    REQUIRE( state1.numTransitions() == 0 );
    REQUIRE( state2.numTransitions() == 0 );
    REQUIRE( state3.numTransitions() == 0 );

    state1.addTransitionTo(state2, 'a');
    REQUIRE( state1.numTransitions() == 1 );
    REQUIRE( state2.numTransitions() == 0 );
    REQUIRE( state3.numTransitions() == 0 );

    state3.addTransitionFrom(state2, 'b');
    REQUIRE( state1.numTransitions() == 1 );
    REQUIRE( state2.numTransitions() == 1 );
    REQUIRE( state3.numTransitions() == 0 );

    std::cout << state1 << std::endl;
    std::cout << state2 << std::endl;
    std::cout << state3 << std::endl;
    std::cout << fsm.startState() << std::endl;

    //REQUIRE( fsm.run("ab") );

}