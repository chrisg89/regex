#include "Regex.hpp" 
#include "Parser.hpp"

#include <cassert> //todo try to replace with excpetion everywhere

#include "Utf8Iterator.hpp"

namespace regex
{

Regex::Regex()
: mDFA{{}} //TODO: create default constructor?
{}

void Regex::compile(std::string regex)
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

    auto current = mDFA.getFirst();

    Utf8Iterator utf8It = string.cbegin();

    while( utf8It != string.end() ) //TODO chnage to for loop
    {
        //Translate CP to int
        auto input = findInAlphabet(*utf8It);
        current = mDFA.step(current, input);
        
        if(mDFA.isDeadState(current))
        {
            break;
        }

        utf8It++;
    }

    return mDFA.isFinalState(current);
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