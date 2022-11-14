
#pragma once

#include <utility>

namespace regex
{

using CodePoint = char32_t; //TODO
using CodePointInterval = std::pair<CodePoint, CodePoint>;   //TODO make custom POD?

constexpr CodePoint kCodePointMin = 0x0000'0000U;  //TODO
constexpr CodePoint kCodePointMax = 0x0010'FFFFU; //TODO

//TODO Add calculation logic here for Codepoints?

}