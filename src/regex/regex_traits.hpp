#pragma once
#include <string>
#include <locale>
#include <regex>

namespace regex
{

template< class CharT >
class regex_traits
{
public:
    using char_type = CharT;
    using string_type = std::basic_string<CharT>;
    using locale_type = std::locale;
    // using char_class_type = Not Implemented

    regex_traits()
    : mLocale{}
    {}

    static std::size_t length(const char_type* p)
    {
        return std::char_traits<CharT>::length(p);
    }

	CharT translate(CharT c) const
    {
        return c;
    }

	CharT translate_nocase(CharT c) const
    {
        return std::use_facet<std::ctype<CharT>>(getloc()).tolower(c);
    }

    // Not implemented
    // {{
    /*
	
    template< class ForwardIt >
    string_type transform( ForwardIt first, ForwardIt last) const;
	
	template< class ForwardIt >
    string_type transform_primary( ForwardIt first, ForwardIt last ) const;
	
    template< class ForwardIt >
    string_type lookup_collatename( ForwardIt first, ForwardIt last ) const;

    template< class ForwardIt >
    char_class_type lookup_classname( ForwardIt first, ForwardIt last, bool icase = false ) const;
        
    bool isctype( CharT c, char_class_type f ) const;

    */
    // }}



    int value( CharT ch, int radix ) const
    {
        std::basic_istringstream<CharT> istream(string_type(1, ch));
        long value;

        if (radix == 8)
            istream >> std::oct;
        else if (radix == 16)
            istream >> std::hex;
        istream >> value;
        return istream.fail() ? -1 : value;
    }


    locale_type imbue( locale_type loc )
    {
        std::swap(mLocale, loc);
        return loc;
    }

    locale_type getloc() const
    {
        return mLocale;
    }

private:
    locale_type mLocale;

};

} //namespace regex
