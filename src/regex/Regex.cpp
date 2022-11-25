#include "Regex.hpp" 
#include "Parser.hpp"

#include <cassert> //todo try to replace with excpetion everywhere

#include "Utf8Iterator.hpp"

namespace regex
{

Regex::Regex(const std::string& regex)
: mDFA{}
{
    auto parser = parser::Parser(regex);
    auto ast = parser.parse();
    mAlphabet = ast.makeAlphabet();
    auto nfa = ast.makeNFA(mAlphabet); 
    mDFA = nfa.toDFA();
}

fa::InputType Regex::findInAlphabet(CodePoint input)
{
    int index = 0;
    for(auto& x : mAlphabet)
    {
        if(input>= x.first && input <= x.second)
        {
            return index;
        }
        index++;
    }

    assert(false); //TODO throw exception //TODO restructure this function
    return 0;
}

bool Regex::match(const std::string string)
{
    auto state = mDFA.getFirst();

    for( Utf8Iterator it = string.cbegin(); it != string.cend(); it++ )
    {
        // lookup the codepoint in the alphabet
        auto input = findInAlphabet(*it);

        // advance the DFA
        state = mDFA.step(state, input);
        
        // exit on a dead state
        if(mDFA.isDeadState(state))
        {
            break;
        }
    }

    return mDFA.isFinalState(state);
}

bool Regex::search(std::string string)
{
    //TODO broken
    /*
    for(auto result : mDFA.search(string))
    {
        std::cout << result << std::endl;
    }
    */

    return true; //todo
}

} //namespace regex