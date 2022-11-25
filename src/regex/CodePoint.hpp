
#pragma once

#include <utility>

namespace regex
{

using CodePoint = char32_t;
using CodePointInterval = std::pair<CodePoint, CodePoint>;

constexpr CodePoint kCodePointMin = 0x0000'0000U;
constexpr CodePoint kCodePointMax = 0x0010'FFFFU;

constexpr CodePoint kEOF = 0xFFFF'FFFFU; 

//TODO Add calculation logic here for Codepoints?

}