
#pragma once

#include "TokenStream.hpp"
#include "DFABuilder.hpp"

#include <string>

namespace regex
{

using DFA = fa::DFA;

class Regex
{

public:
    Regex();

    void compile(std::string regex);

    bool match(std::string string);

    //TODO with match results
    //TODO should there be DFA builder and DFA Runner classes?
    bool search(std::string string);


private:
    DFA mDFA;

};

bool isValidRegex(TokenStream regex);

} //namespace regex