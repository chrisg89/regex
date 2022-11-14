
#pragma once

#include "CodePoint.hpp"

#include <vector>

namespace regex
{

using Alphabet = std::vector<CodePointInterval>;

void DisjoinOverlap(Alphabet& alphabet, CodePoint min, CodePoint max);
void negate(Alphabet& alphabet);

}