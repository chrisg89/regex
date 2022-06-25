#pragma once 

#include <vector>

namespace fa
{

using StateId = int;
using Alphabet = std::vector<char>;
constexpr char kNullState = -1;
constexpr char kEpsilon = 0;

}