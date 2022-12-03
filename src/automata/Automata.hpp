#pragma once 

#include <set>
#include <vector>

namespace automata
{

using InputType = int;
using StateId = int;
using Alphabet = std::vector<InputType>;
constexpr StateId kNullState = -1;
constexpr InputType kEpsilon = -1;

}