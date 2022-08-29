#pragma once

#include "basic_regex.hpp"
#include "match_results.hpp"


namespace regex
{

// Overload #1
template< class BidirIt, class Alloc, class CharT, class Traits >
bool regex_match( BidirIt first, 
                  BidirIt last,
                  match_results<BidirIt,Alloc>& m,
                  const basic_regex<CharT,Traits>& e,
                  regex_constants::match_flag_type flags = regex_constants::match_default )
{
    m = match_results<BidirIt,Alloc>();

    if (e.mRegexImpl.match(first, last))
    {
        m.add_submatch(first, last, true);
        m.set_prefix(first, first, false);
        m.set_suffix(last, last, false);
        m.set_ready();
        return true;
    }
    else
    {
        m.set_ready();
        return false;
    }

}

// Overload #2
template< class BidirIt, class CharT, class Traits >
bool regex_match( BidirIt first, 
                  BidirIt last,
                  const basic_regex<CharT,Traits>& e,
                  regex_constants::match_flag_type flags = regex_constants::match_default)
{
    match_results<BidirIt> dummy;
    return regex_match(first, last, dummy, e, flags);
}

// Overload #3
template< class CharT, class Alloc, class Traits >
bool regex_match( const CharT* str,
                  match_results<const CharT*,Alloc>& m,
                  const basic_regex<CharT,Traits>& e,
                  regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return regex_match(str, str + Traits::length(str), m, e, flags);
}

// Overload #4
template< class STraits, class SAlloc, class Alloc, class CharT, class Traits >
bool regex_match( const std::basic_string<CharT,STraits,SAlloc>& s,
                  match_results<typename std::basic_string<CharT,STraits,SAlloc>::const_iterator,Alloc>& m,
                  const basic_regex<CharT,Traits>& e,
                  regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return regex_match(s.begin(), s.end(), m, e, flags);
}

// Overload #5
template< class CharT, class Traits >
bool regex_match( const CharT* str,
                  const basic_regex<CharT,Traits>& e,
                  regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return regex_match(str, str + Traits::length(str), e, flags);
}

// Overload #6
template< class STraits, class SAlloc, class CharT, class Traits >
bool regex_match( const std::basic_string<CharT, STraits, SAlloc>& s,
                  const basic_regex<CharT,Traits>& e,
                  regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return regex_match(s.begin(), s.end(), e, flags);
}


// Explicitly deleted overload
template< class STraits, class SAlloc, class Alloc, class CharT, class Traits >
bool regex_match( const std::basic_string<CharT,STraits,SAlloc>&&,
                  match_results<typename std::basic_string<CharT,STraits,SAlloc>::const_iterator,Alloc>&,
                  const basic_regex<CharT,Traits>&,
                  regex_constants::match_flag_type flags = regex_constants::match_default ) = delete;

} //namespace regex
