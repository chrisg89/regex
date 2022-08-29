#pragma once

#include "regex_constants.hpp"
#include "regex_traits.hpp"
#include "sub_match.hpp"

#include <iterator>
#include <memory>
#include <string>
#include <vector>

namespace regex
{

template<class BidirIt, class Alloc = std::allocator<sub_match<BidirIt>>>
class match_results
{
public:
    using allocator_type = Alloc;
    using value_type = sub_match<BidirIt>;
    using const_reference = const value_type&;
    using reference = value_type&;
    using container_type = std::vector<value_type, allocator_type>;
    using const_iterator = typename container_type::const_iterator;
    using iterator = const_iterator;
    using difference_type = typename std::iterator_traits<BidirIt>::difference_type;
    using size_type = typename std::allocator_traits<Alloc>::size_type;
    using char_type = typename std::iterator_traits<BidirIt>::value_type;
    using string_type = std::basic_string<char_type>;

    // default ctor
    match_results() 
    : match_results(allocator_type())
    {}

    // parameterized ctor
    match_results( const allocator_type& a )
    : mReady {false}
    , mContainer{a}
    , mUnmatched{}
    , mPrefix{}
    , mSuffix{}
    {}


    // copy ctor
    match_results(const match_results& rhs) = default;
    // move ctor
    match_results(match_results&& rhs) noexcept = default;
    // dtor
    ~match_results() = default;
    // copy assignment
    match_results& operator=( const match_results& other ) = default;
    // move assignment
    match_results& operator=( match_results&& other ) noexcept = default;

    allocator_type get_allocator() const
    {
        return mContainer.get_allocator();
    }

    bool ready() const 
    {
        return mReady;
    }

    bool empty() const
    {
        return mContainer.empty();
    }

    size_type size() const noexcept
    {
        return mContainer.size();
    }

    size_type max_size() const noexcept
    {
        return mContainer.max_size();
    }

    difference_type length( size_type n) const
    {
        if ( n < this->size())
            return mContainer[n].length();
        else
            return mUnmatched.length();
    }

    difference_type position( size_type n ) const
    {
        if ( n < this->size())
            return std::distance(mContainer[0].first, mContainer[n].first);
        else
            return std::distance(mContainer[0].first, mUnmatched.first);
    }

    string_type str( size_type n ) const
    {
        if ( n < this->size())
            return mContainer[n].str();
        else
            return mUnmatched.str();
    }

    const_reference operator[]( size_type n ) const
    {
        if ( n < this->size())
            return mContainer[n];
        else
            return mUnmatched;
    }

    const_reference prefix() const
    {
        return mPrefix;
    }

    const_reference suffix() const
    {
        return mSuffix;
    }

    iterator begin() noexcept
    {
        return mContainer.begin();
    }

    const_iterator begin() const noexcept
    {
        return mContainer.begin();
    }

    const_iterator cbegin() const noexcept
    {
        return mContainer.cbegin();
    }

    iterator end() noexcept
    {
        return mContainer.end();
    }

    const_iterator end() const noexcept
    {
        return mContainer.end();
    }

    const_iterator cend() const noexcept
    {
        return mContainer.cend();
    }


    // Not implemented
    // {{
    /*
    template< class OutputIt >
    OutputIt format( OutputIt out,
                     const char_type* fmt_first,
                     const char_type* fmt_last,
                     std::regex_constants::match_flag_type flags = std::regex_constants::format_default 
                    ) const;


    template< class OutputIt >
    OutputIt format( OutputIt out,
                     const char_type* fmt_first,
                     const char_type* fmt_last,
                     std::regex_constants::match_flag_type flags = std::regex_constants::format_default 
                    ) const;


    template< class OutputIt, class ST, class SA >
    OutputIt format( OutputIt out,
                     const basic_string<char_type,ST,SA>& fmt,
                     std::regex_constants::match_flag_type flags = std::regex_constants::format_default 
                   ) const;

    template< class ST, class SA >
    std::basic_string<char_type,ST,SA>
            format( const std::basic_string<char_type,ST,SA>& fmt,
                    std::regex_constants::match_flag_type flags = std::regex_constants::format_default 
                  ) const;

    string_type format( const char_type* fmt_s,
                        std::regex_constants::match_flag_type flags = std::regex_constants::format_default 
                      ) const;

    */
    // }}

    void swap( match_results<BidirIt,Alloc>& other ) noexcept
    {
        this->mContainer.swap(other.mContainer);
        std::swap(this->mReady, other.mReady);
        std::swap(this->mPrefix, other.mPrefix); //TODO why does this work? sub_match<BidirIt> is not moveable.
        std::swap(this->mSuffix, other.mSuffix); //TODO why does this work? sub_match<BidirIt> is not moveable.
    }

    // non-standard API 
    // {{

    void add_submatch(BidirIt first, BidirIt last, bool matched)
    {
        auto sub = sub_match<BidirIt>();
        sub.first = first;
        sub.second = last;
        sub.matched = matched;
        this->mContainer.push_back(std::move(sub));
    }

    void set_prefix(BidirIt first, BidirIt last, bool matched)
    {
        mPrefix.first = first;
        mPrefix.second = last;
        mPrefix.matched = matched;
    }

    void set_suffix(BidirIt first, BidirIt last, bool matched)
    {
        mSuffix.first = first;
        mSuffix.second = last;
        mSuffix.matched = matched;
    }

    void set_unmatched(BidirIt first, BidirIt last, bool matched)
    {
        mUnmatched.first = first;
        mUnmatched.second = last;
        mUnmatched.matched = matched;
    }

    void set_ready()
    {
        mReady = true;
    }

    // }}

private:

    bool mReady;
    container_type mContainer;
    value_type mUnmatched;
    value_type mPrefix;
    value_type mSuffix;

    template< class _BidirIt, class _Alloc >
    friend bool operator==( match_results<_BidirIt,_Alloc>& lhs, match_results<_BidirIt,_Alloc>& rhs);

    template< class _BidirIt, class _Alloc >
    friend bool operator!=( match_results<_BidirIt,_Alloc>& lhs, match_results<_BidirIt,_Alloc>& rhs);

    template< class _BidirIt, class _Alloc >
    friend void swap( match_results<_BidirIt,_Alloc>& x1, match_results<_BidirIt,_Alloc>& x2 ) noexcept;
};

// Non-member functions

template< class BidirIt, class Alloc >
bool operator==( match_results<BidirIt,Alloc>& lhs, match_results<BidirIt,Alloc>& rhs )
{
    if(lhs.mReady != rhs.mReady)
        return false;
    if(lhs.mContainer != rhs.mContainer)
        return false;
    if(lhs.mPrefix != rhs.mPrefix)
        return false;
    if(lhs.mSuffix != rhs.mSuffix)
        return false;

    return true;
}

template< class BidirIt, class Alloc >
bool operator!=( match_results<BidirIt,Alloc>& lhs, match_results<BidirIt,Alloc>& rhs )
{
    return !(lhs==rhs);
}

template< class BidirIt, class Alloc >
void swap( match_results<BidirIt,Alloc>& x1, match_results<BidirIt,Alloc>& x2 ) noexcept
{
    x1.swap(x2);
}

// Specializations

using cmatch = match_results<const char*>;
using wcmatch =	match_results<const wchar_t*>;
using smatch =match_results<std::string::const_iterator>;
using wsmatch =	match_results<std::wstring::const_iterator>;


} //namespace regex
