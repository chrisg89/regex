#pragma once

#include <iterator>
#include <string>
#include <utility>

namespace regex
{

template<class BidirIt>
class sub_match : public std::pair<BidirIt, BidirIt>
{
    using Super = std::pair<BidirIt, BidirIt>;
public:

    using iterator = BidirIt;
    using value_type = typename std::iterator_traits<BidirIt>::value_type;
    using difference_type = typename std::iterator_traits<BidirIt>::difference_type;
    using string_type = std::basic_string<value_type>;

    // default ctor
    sub_match()
        : Super{}
        , matched{false}
    {}

    // Copy and move
    // These are not described in the specification
    // but a reference implementation supported them
    // hence, I added them as well. They are very useful.
    sub_match(const sub_match& rhs) = default;
    sub_match(sub_match&& rhs) noexcept = default;
    sub_match& operator=( const sub_match& other ) = default;
    sub_match& operator=( sub_match&& other ) noexcept = default;

    // TODO: sub_match inherits from std::pair<BidirIt, BidirIt>, although it 
    // cannot be treated as a std::pair object because member functions such 
    // as swap and assignment will not work as expected. Should delete these?

    // Indicates if this match was successful
    bool matched;

    difference_type length() const
    {
        if (matched)
            return std::distance(Super::first, Super::second);
        else
            return difference_type{};
    }

    operator string_type() const
    {
        if (matched)
            return string_type{Super::first, Super::second};
        else
            return string_type{};
    }

    string_type str() const
    {
        if (matched)
            return string_type{Super::first, Super::second};
        else
            return string_type{};
    }

    int compare( const sub_match& m) const
    {
        return str().compare(m.str()); //TODO optimize.
    }

    int compare( const string_type& s) const
    {
        return str().compare(s); //TODO optimize
    }

    int compare( const value_type* c) const
    {
        return str().compare(c); //TODO optimize
    }

private:
};

//Direct comparison

template< class BidirIt >
bool operator==( const sub_match<BidirIt>& lhs,
                 const sub_match<BidirIt>& rhs )
{
    if (lhs.compare(rhs) == 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator!=( const sub_match<BidirIt>& lhs,
                 const sub_match<BidirIt>& rhs )
{
    if (lhs.compare(rhs) != 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<( const sub_match<BidirIt>& lhs,
                 const sub_match<BidirIt>& rhs )
{
    if (lhs.compare(rhs) < 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<=( const sub_match<BidirIt>& lhs,
                 const sub_match<BidirIt>& rhs )
{
    if (lhs.compare(rhs) <= 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>( const sub_match<BidirIt>& lhs,
                 const sub_match<BidirIt>& rhs )
{
    if (lhs.compare(rhs) > 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>=( const sub_match<BidirIt>& lhs,
                 const sub_match<BidirIt>& rhs )
{
    if (lhs.compare(rhs) >= 0)
        return true;

    return false;
}

//sub_match and std::basic_string

template< class BidirIt, class Traits, class Alloc >
bool operator==( const sub_match<BidirIt>& sm,
                 const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& rhs )
{
    if (sm.compare(rhs) == 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator!=( const sub_match<BidirIt>& sm,
                 const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& st )
{
    if (sm.compare(st) != 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator<( const sub_match<BidirIt>& sm,
                const std::basic_string<
                    typename std::iterator_traits<BidirIt>::value_type,
                    Traits,Alloc>& st )
{
    if (sm.compare(st) < 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator<=( const sub_match<BidirIt>& sm,
                 const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& st )
{
    if (sm.compare(st) <= 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator>( const sub_match<BidirIt>& sm,
                const std::basic_string<
                    typename std::iterator_traits<BidirIt>::value_type,
                    Traits,Alloc>& st )
{
    if (sm.compare(st) > 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator>=( const sub_match<BidirIt>& sm,
                 const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& st )
{
    if (sm.compare(st) >= 0)
        return true;

    return false;
}


//std::basic_string and sub_match

template< class BidirIt, class Traits, class Alloc >
bool operator==( const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& st,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(st) == 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator!=( const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& st,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(st) != 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator<( const std::basic_string<
                    typename std::iterator_traits<BidirIt>::value_type,
                    Traits,Alloc>& st,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(st) > 0)
        return true;

    return false;
}


template< class BidirIt, class Traits, class Alloc >
bool operator<=( const std::basic_string<
                    typename std::iterator_traits<BidirIt>::value_type,
                    Traits,Alloc>& st,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(st) >= 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator>( const std::basic_string<
                    typename std::iterator_traits<BidirIt>::value_type,
                    Traits,Alloc>& st,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(st) < 0)
        return true;

    return false;
}

template< class BidirIt, class Traits, class Alloc >
bool operator>=( const std::basic_string<
                     typename std::iterator_traits<BidirIt>::value_type,
                     Traits,Alloc>& st,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(st) <= 0)
        return true;

    return false;
}

//sub_match and std::iterator_traits<BidirIt>::value_type*

template< class BidirIt >
bool operator==( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type* s )
{
    if (sm.compare(s) == 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator!=( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type* s )
{
    if (sm.compare(s) != 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<( const sub_match<BidirIt>& sm,
                const typename std::iterator_traits<BidirIt>::value_type* s )
{
    if (sm.compare(s) < 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<=( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type* s )
{
    if (sm.compare(s) <= 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>( const sub_match<BidirIt>& sm,
                const typename std::iterator_traits<BidirIt>::value_type* s )
{
    if (sm.compare(s) > 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>=( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type* s )
{
    if (sm.compare(s) >= 0)
        return true;

    return false;
}

// std::iterator_traits<BidirIt>::value_type* and sub_match

template< class BidirIt >
bool operator==( const typename std::iterator_traits<BidirIt>::value_type* s,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(s) == 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator!=( const typename std::iterator_traits<BidirIt>::value_type* s,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(s) != 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<( const typename std::iterator_traits<BidirIt>::value_type* s,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(s) > 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<=( const typename std::iterator_traits<BidirIt>::value_type* s,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(s) >= 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>( const typename std::iterator_traits<BidirIt>::value_type* s,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(s) < 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>=( const typename std::iterator_traits<BidirIt>::value_type* s,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(s) <= 0)
        return true;

    return false;
}

// sub_match and std::iterator_traits<BidirIt>::value_type


template< class BidirIt >
bool operator==( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type& ch )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) == 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator!=( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type& ch )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) != 0)
        return true;

    return false;
}


template< class BidirIt >
bool operator<( const sub_match<BidirIt>& sm,
                const typename std::iterator_traits<BidirIt>::value_type& ch )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) < 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<=( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type& ch )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) <= 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>( const sub_match<BidirIt>& sm,
                const typename std::iterator_traits<BidirIt>::value_type& ch )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) > 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>=( const sub_match<BidirIt>& sm,
                 const typename std::iterator_traits<BidirIt>::value_type& ch )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) >= 0)
        return true;

    return false;
}


// std::iterator_traits<BidirIt>::value_type and sub_match

template< class BidirIt >
bool operator==( const typename std::iterator_traits<BidirIt>::value_type& ch,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) == 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator!=( const typename std::iterator_traits<BidirIt>::value_type& ch,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) != 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<( const typename std::iterator_traits<BidirIt>::value_type& ch,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) > 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator<=( const typename std::iterator_traits<BidirIt>::value_type& ch,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) >= 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>( const typename std::iterator_traits<BidirIt>::value_type& ch,
                const sub_match<BidirIt>& sm )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) < 0)
        return true;

    return false;
}

template< class BidirIt >
bool operator>=( const typename std::iterator_traits<BidirIt>::value_type& ch,
                 const sub_match<BidirIt>& sm )
{
    if (sm.compare(typename sub_match<BidirIt>::string_type(1, ch)) <= 0)
        return true;

    return false;
}

template< class CharT, class Traits, class BidirIt >
std::basic_ostream<CharT,Traits>&
    operator<<( std::basic_ostream<CharT,Traits>& os,
                const sub_match<BidirIt>& m )
{
    os << m.str();
    return os;
}


using csub_match = sub_match<const char*>;
using wcsub_match = sub_match<const wchar_t*>;
using ssub_match = sub_match<std::string::const_iterator>;
using wssub_match = sub_match<std::wstring::const_iterator>;

} //namespace regex
