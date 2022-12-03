#pragma once 

#include <vector>
#include <set>

namespace automata
{

using InputType = int;
using StateId = int;
using Alphabet = std::vector<InputType>;
constexpr StateId kNullState = -1;
constexpr InputType kEpsilon = -1;

}