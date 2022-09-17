
#pragma once

#include "TokenStream.hpp"
#include "DFABuilder.hpp"

#include <string>

//TODO clean this up
// start range
#include <algorithm>
#include <queue>

using range = std::pair<char32_t, char32_t>; //TODO Range
using ranges = std::vector<range>; //TODO Ranges
constexpr auto min = char32_t(0);
constexpr auto max = char32_t(0x1FFFF);
// end range

namespace regex
{

using DFA = fa::DFA;

void DisjoinOverlap(ranges& ranges, char32_t min, char32_t max);

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