#pragma once 

#include <vector>
#include <set>

//TODO rename fa to fsa???

namespace fa
{

using InputType = int;
using StateId = int;
using Alphabet = std::vector<InputType>;
constexpr StateId kNullState = -1;
constexpr InputType kEpsilon = -1;

}