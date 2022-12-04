#include "Regex.hpp"
#include "Parser.hpp"

#include <algorithm>
#include <cassert>

#include "Utf8Iterator.hpp"

namespace regex
{

using parser::Parser;

Regex::Regex(const std::string& pattern)
: Regex{Parser(pattern).parse()}
{}

Regex::Regex(const ast::AST& ast)
: mAlphabet{ast.makeAlphabet()}
, mDFA{ast.makeNFA(mAlphabet).toDFA()}
{}

automata::InputType Regex::findInAlphabet(CodePoint input)
{
    const auto within = [input](CodePointInterval interval)
    { 
        return input>= interval.first && input <= interval.second; 
    };

    auto begin = mAlphabet.begin();
    auto end = mAlphabet.end();
    auto result = std::find_if(begin, end, within);
    assert(result != end);
    return std::distance(begin, result);
}

bool Regex::match(const std::string& target)
{
    auto state = mDFA.getStartState();

    for( Utf8Iterator it = target.cbegin(); it != target.cend(); ++it )
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

bool Regex::search(const std::string& target)
{
    //TODO broken
    /*
    for(auto result : mDFA.search(target))
    {
        std::cout << result << std::endl;
    }
    */

    return true; //todo
}

} //namespace regex