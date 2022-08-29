#pragma once

#include <cstdint>



namespace regex_constants
{

typedef uint32_t syntax_option_type;
constexpr syntax_option_type icase = 1;
constexpr syntax_option_type nosubs = 2;
constexpr syntax_option_type optimize = 4;
constexpr syntax_option_type collate = 8;
constexpr syntax_option_type ECMAScript = 16;
constexpr syntax_option_type basic = 32;
constexpr syntax_option_type extended = 64;
constexpr syntax_option_type awk = 128;
constexpr syntax_option_type grep = 256;
constexpr syntax_option_type egrep = 512;

typedef uint32_t match_flag_type;
constexpr match_flag_type match_default = {};
constexpr match_flag_type match_not_bol = 1;
constexpr match_flag_type match_not_eol = 2;
constexpr match_flag_type match_not_bow = 4;
constexpr match_flag_type match_not_eow = 8;
constexpr match_flag_type match_any = 16;
constexpr match_flag_type match_not_null = 32;
constexpr match_flag_type match_continuous = 64;
constexpr match_flag_type match_prev_avail = 128;

} //namespace regex_constants