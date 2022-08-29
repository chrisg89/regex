#pragma once

#include "regex_constants.hpp"
#include "regex_traits.hpp"
#include "Regex.hpp"

#include <string>

namespace regex
{

//forward declaration for friend (avoids circular header dependencies)
template<class BidirIt, class Alloc>
class match_results;

//TODO lots to implement in this file

template <class CharT, class Traits = regex_traits<CharT>> 
class basic_regex
{
public:

    using value_type = CharT;
    using traits_type = Traits;
    using string_type = typename Traits::string_type;
    using locale_type = typename Traits::locale_type;
    using flag_type = regex_constants::syntax_option_type;

    static constexpr flag_type icase = regex_constants::icase;
    static constexpr flag_type nosubs = regex_constants::nosubs;
    static constexpr flag_type optimize = regex_constants::optimize;
    static constexpr flag_type collate = regex_constants::collate;
    static constexpr flag_type ECMAScript = regex_constants::ECMAScript;
    static constexpr flag_type basic = regex_constants::basic;
    static constexpr flag_type extended = regex_constants::extended;
    static constexpr flag_type awk = regex_constants::awk;
    static constexpr flag_type grep = regex_constants::grep;
    static constexpr flag_type egrep = regex_constants::egrep;

    basic_regex()
    : basic_regex((CharT *)nullptr, (CharT *)nullptr)
    {}

    explicit basic_regex( const CharT* s, flag_type f = regex_constants::ECMAScript )
    : basic_regex(s, s + Traits::length(s), f)
    {}

    basic_regex( const CharT* s, std::size_t count,flag_type f = regex_constants::ECMAScript )
    : basic_regex(s, s+count, f)
    {}

    basic_regex( const basic_regex& other ) = default;
    basic_regex( basic_regex&& other ) noexcept = default;

    template< class ST, class SA >
    explicit basic_regex( const std::basic_string<CharT,ST,SA>& str,flag_type f = regex_constants::ECMAScript )
    : basic_regex(str.begin(), str.end(), f)
    {}

    template< class ForwardIt >
    basic_regex( ForwardIt first, ForwardIt last,flag_type f = regex_constants::ECMAScript )
    : mRegexImpl(first, last)
    , mTraits{}
    , mMarkCount{0}
    , mFlags{f}
    {}

    basic_regex( std::initializer_list<CharT> il,flag_type f = regex_constants::ECMAScript )
    : basic_regex(il.begin(), il.end(), f)
    {}

    ~basic_regex() = default;

    basic_regex& operator=( const basic_regex& other ) = default; //TODO should this use assign?
    basic_regex& operator=( basic_regex&& other ) noexcept = default; //TODO should this use assign?
    
    basic_regex& operator=( const CharT* ptr )
    {
        return assign(ptr);
    }

    basic_regex& operator=( std::initializer_list<CharT> il )
    {
        return assign(il);
    }

    template< class ST, class SA >
    basic_regex& operator=( const std::basic_string<CharT,ST,SA>& p )
    {
        return assign(p);
    }

    basic_regex& assign( const basic_regex& other )
    {
        mMarkCount = other.mMarkCount;
        mFlags = other.mFlags;
        mRegexImpl = other.mRegexImpl;
        return *this;
    }

    basic_regex& assign( basic_regex&& other ) noexcept
    {
        mMarkCount = std::move(other.mMarkCount);
        mFlags = std::move(other.mFlags);
        mRegexImpl = std::move(other.mRegexImpl);
        return *this;
    }

    basic_regex& assign( const CharT* s, flag_type f = regex_constants::ECMAScript)
    {
        return assign(s, s + Traits::length(s),f);
    }

    basic_regex& assign( const CharT* ptr, size_t count, flag_type f = regex_constants::ECMAScript)
    {
        return assign(ptr, ptr+count,f);
    }

    template< class ST, class SA >
    basic_regex& assign( const std::basic_string<CharT,ST,SA>& str, flag_type f = regex_constants::ECMAScript)
    {
        return assign(str.begin(), str.end(),f);
    }

    template< class InputIt >
    basic_regex& assign( InputIt first, InputIt last, flag_type f = regex_constants::ECMAScript)
    {
        mFlags = f;
        mMarkCount = 0;
        mRegexImpl = Regex(first, last);
        return *this;
    }

    basic_regex& assign( std::initializer_list<CharT> il, flag_type f = regex_constants::ECMAScript)
    {
        return assign(il.begin(), il.end(), f);
    }

    unsigned mark_count() const
    {
        return mMarkCount;
    }

    flag_type flags() const
    {
        return mFlags;
    }

    locale_type getloc() const
    {
        return mTraits.getloc();
    }

    locale_type imbue( locale_type loc )
    {
        return mTraits.imbue(loc);
    }

    //TODO: implement this
    void swap( basic_regex& other ) noexcept;

private:

    template< class BidirIt, class Alloc, class _CharT, class _Traits >
    friend bool regex_match( BidirIt first, 
                             BidirIt last,
                             match_results<BidirIt,Alloc>& m,
                             const basic_regex<_CharT,_Traits>& e,
                             regex_constants::match_flag_type flags);

    flag_type mFlags;
    traits_type mTraits;
    unsigned mMarkCount;
    Regex mRegexImpl;  //TODO move Regex into detail and rename

};

//TODO: implement this
//template< class CharT, class Traits >
//void swap( basic_regex<CharT,Traits> &lhs, basic_regex<CharT,Traits> &rhs ) noexcept;

using regex = basic_regex<char>;
using wregex = basic_regex<wchar_t>;

} //namespace regex

