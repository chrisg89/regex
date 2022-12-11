#pragma once

#include <cstdint>
#include <set>
#include <vector>

namespace automata
{

using InputType = int;
using StateId = unsigned int;
using Alphabet = std::vector<InputType>;
constexpr InputType kEpsilon = -1;

}