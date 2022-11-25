#include "catch.hpp"
#include "Regex.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>



namespace regex{

namespace {

std::string utf8Encode(CodePoint codePoint)
{
    std::string utf8;

    if (codePoint <= 0x7F) 
    {
        // 1-byte unicode
        utf8.append(1, codePoint);

    }
    else if (codePoint <= 0x07FF) 
    {
        // 2-byte unicode
        utf8.append(1, (((codePoint >> 6) & 0x1F) | 0xC0));
        utf8.append(1, (((codePoint >> 0) & 0x3F) | 0x80));
    }
    else if (codePoint <= 0xFFFF) 
    {
        // 3-byte unicode
        utf8.append(1, (((codePoint >> 12) & 0x0F) | 0xE0));
        utf8.append(1, (((codePoint >>  6) & 0x3F) | 0x80));
        utf8.append(1, (((codePoint >>  0) & 0x3F) | 0x80));
    }
    else if (codePoint <= 0x10FFFF) 
    {
        // 4-byte unicode
        utf8.append(1, (((codePoint >> 18) & 0x07) | 0xF0));
        utf8.append(1, (((codePoint >> 12) & 0x3F) | 0x80));
        utf8.append(1, (((codePoint >>  6) & 0x3F) | 0x80));
        utf8.append(1, (((codePoint >>  0) & 0x3F) | 0x80));
    }
    else 
    { 
        // TODO throw?
    }

    return utf8;
}


SCENARIO("Regex Match") 
{
    {
        auto regex = Regex("a");
        REQUIRE(!regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(!regex.match("aa"));
    }

    {
        
        auto regex = Regex("a|b");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("ab"));
        REQUIRE(!regex.match("ba"));
        REQUIRE(!regex.match(""));
    }

    {
        
        auto regex = Regex("ab");
        REQUIRE(regex.match("ab"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("ba"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("abb"));
        REQUIRE(!regex.match("aba"));
        REQUIRE(!regex.match(""));
    }

    {
        
        auto regex = Regex("a*");
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
    }

    {
        
        auto regex = Regex("a+");
        REQUIRE(!regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
    }

    {
        
        auto regex = Regex("a?");
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaa"));
    }

    {
        
        auto regex = Regex("a*(b|c)");
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("ab"));
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("aaab"));

        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("ac"));
        REQUIRE(regex.match("aac"));
        REQUIRE(regex.match("aaac"));

        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaa"));

        REQUIRE(!regex.match("aba"));
        REQUIRE(!regex.match("abc"));
        REQUIRE(!regex.match("aca"));
        REQUIRE(!regex.match("acb"));
    }

    {
        
        auto regex = Regex("((ab)|(cd))*e");
        REQUIRE(regex.match("e"));
        REQUIRE(regex.match("abe"));
        REQUIRE(regex.match("cde"));
        REQUIRE(regex.match("ababe"));
        REQUIRE(regex.match("cdcde"));
        REQUIRE(regex.match("abcde"));
        REQUIRE(regex.match("cdabe"));
    }

    {
        
        auto regex = Regex("A\\|\\|B");
        REQUIRE(regex.match("A||B"));
    }

    {
        
        auto regex = Regex("A\\*B");
        REQUIRE(regex.match("A*B"));
    }

    {
        
        auto regex = Regex("\\(abc\\)");
        REQUIRE(regex.match("(abc)"));
    }

    {
        
        auto regex = Regex("while|if|void|break");
        REQUIRE(regex.match("if"));
        REQUIRE(regex.match("while"));
        REQUIRE(regex.match("void"));
        REQUIRE(regex.match("break"));

    }

    //  ANY (.)
    {
        
        auto regex = Regex("a.*b");
        REQUIRE(regex.match("a123b"));
        REQUIRE(regex.match("a456b"));
    }

    //  Digit (\d)
    {
        
        auto regex = Regex("a\\db");
        REQUIRE(regex.match("a0b"));
        REQUIRE(regex.match("a1b"));
        REQUIRE(regex.match("a2b"));
        REQUIRE(regex.match("a4b"));
        REQUIRE(regex.match("a5b"));
        REQUIRE(regex.match("a6b"));
        REQUIRE(regex.match("a7b"));
        REQUIRE(regex.match("a8b"));
        REQUIRE(regex.match("a9b"));
        REQUIRE(!regex.match("aab"));
    }

    //  NonDigit (\D)
    {
        
        auto regex = Regex("a\\Db");
        REQUIRE(!regex.match("a0b"));
        REQUIRE(!regex.match("a1b"));
        REQUIRE(!regex.match("a2b"));
        REQUIRE(!regex.match("a4b"));
        REQUIRE(!regex.match("a5b"));
        REQUIRE(!regex.match("a6b"));
        REQUIRE(!regex.match("a7b"));
        REQUIRE(!regex.match("a8b"));
        REQUIRE(!regex.match("a9b"));
        REQUIRE(regex.match("aab"));
    }

        //  Whitespace (\s)
    {
        
        auto regex = Regex("a\\sb");
        REQUIRE(regex.match("a b"));
        REQUIRE(regex.match("a\tb"));
        REQUIRE(regex.match("a\nb"));
        REQUIRE(regex.match("a\vb"));
        REQUIRE(regex.match("a\fb"));
        REQUIRE(regex.match("a\rb"));
        REQUIRE(!regex.match("aab"));
    }

    //  Whitespace (\s)
    {
        
        auto regex = Regex("a\\Sb");
        REQUIRE(!regex.match("a b"));
        REQUIRE(!regex.match("a\tb"));
        REQUIRE(!regex.match("a\nb"));
        REQUIRE(!regex.match("a\vb"));
        REQUIRE(!regex.match("a\fb"));
        REQUIRE(!regex.match("a\rb"));
        REQUIRE(regex.match("aab"));
    }

    //  Word Character (\w)
    {
        
        auto regex = Regex("a\\wb");
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("a1b"));
        REQUIRE(regex.match("a_b"));
        REQUIRE(!regex.match("a\vb"));
        REQUIRE(!regex.match("a.b"));
        REQUIRE(!regex.match("a+b"));
    }

    //  Non Word Character (\W)
    {
        
        auto regex = Regex("a\\Wb");
        REQUIRE(!regex.match("aab"));
        REQUIRE(!regex.match("a1b"));
        REQUIRE(!regex.match("a_b"));
        REQUIRE(regex.match("a\vb"));
        REQUIRE(regex.match("a.b"));
        REQUIRE(regex.match("a+b"));
    }

    //  Newline
    {
        
        auto regex = Regex("\\n");
        REQUIRE(regex.match("\n"));
    }

    //  FormFeed
    {
        
        auto regex = Regex("\\f");
        REQUIRE(regex.match("\f"));
    }

    //  CarriageReturn
    {
        
        auto regex = Regex("\\r");
        REQUIRE(regex.match("\r"));
    }

    //  Horizontal Tab
    {
        
        auto regex = Regex("\\t");
        REQUIRE(regex.match("\t"));
    }

    //  Vertical Tab
    {
        
        auto regex = Regex("\\v");
        REQUIRE(regex.match("\v"));
    }

    //  Realistic tests using dates
    {
        
        auto regex = Regex("\\d\\d-(\\d\\d|JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC)-\\d\\d(\\d\\d)?");
        REQUIRE(regex.match("12-12-22"));
        REQUIRE(regex.match("12-12-2022"));
        REQUIRE(regex.match("12-OCT-2022"));
    }
    
    //  Unicode characters
    {
        // 「こんにちは世界」 - Hello World! (Kanji)
        const char utf8String[] = {'\xE3', '\x80', '\x8C', '\xE3', '\x81', '\x93', '\xE3', '\x82', '\x93', '\xE3', '\x81', '\xAB', '\xE3', '\x81', '\xA1', '\xE3', '\x81', '\xAF', '\xE4', '\xB8', '\x96', '\xE7', '\x95', '\x8C', '\xE3', '\x80', '\x8D', '\x00'};
        std::string string = utf8String;
        
        auto regex = Regex(string);
        REQUIRE(regex.match(utf8String));
        REQUIRE(!regex.match("hello world!"));
    }

    //Character class
    {
        
        auto regex = Regex("[abc]");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(!regex.match("["));
        REQUIRE(!regex.match("]"));
    }

    //Character class
    {
        
        auto regex = Regex("[a-c]");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(!regex.match("["));
        REQUIRE(!regex.match("]"));
    }

    //Character class
    {
        
        auto regex = Regex("[a-c1-3]");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(!regex.match("["));
        REQUIRE(!regex.match("]"));
    }

    //Character class
    {
        
        auto regex = Regex("[-]");
        REQUIRE(regex.match("-"));
    }

    //Character class
    {
        
        auto regex = Regex("[a-]");
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));
    }

    //Character class
    {
        
        auto regex = Regex("[ab-]");
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
    }

    //Character class
    {
        
        auto regex = Regex("[^-]");
        REQUIRE(!regex.match("-"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
    }

    //Character class
    {
        
        auto regex = Regex("[^-a-c]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        
        auto regex = Regex("[^\\-a-c]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        
        auto regex = Regex("[^a-c\\-]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        
        auto regex = Regex("[^-a]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(regex.match("b"));
    }

    //Character class
    {
        
        auto regex = Regex("[-a]");
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));
        REQUIRE(!regex.match("b"));
    }

    //Character class
    {
        
        auto regex = Regex("[b-b]");
        REQUIRE(regex.match("b"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        
        auto regex = Regex("[^b-b]");
        REQUIRE(!regex.match("b"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("c"));
    }




    //Character class with shorthand
    {
        
        auto regex = Regex("[\\d]");
        REQUIRE(regex.match("0"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(regex.match("4"));
        REQUIRE(regex.match("5"));
        REQUIRE(regex.match("6"));
        REQUIRE(regex.match("7"));
        REQUIRE(regex.match("8"));
        REQUIRE(regex.match("9"));
    }
}

#if 0
SCENARIO("Shorthand \\d matches [0-9]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\d]");

    if( i >= '0' && i <= '9')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}


SCENARIO("Shorthand \\D matches [^0-9]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\D]");

    if( i >= '0' && i <= '9')
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
}


SCENARIO("Shorthand \\s matches [\r\n\t\f\v ]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\s]");

    if( i == '\r' || i == '\n' || i == '\t' || i == '\f' || i == '\v' || i == ' ')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}


SCENARIO("Shorthand \\S matches [^\\r\\n\\t\\f\\v ]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\S]");

    if( i == '\r' || i == '\n' || i == '\t' || i == '\f' || i == '\v' || i == ' ')
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
}

SCENARIO("Shorthand \\w matches [a-zA-Z0-9_]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\w]");

    if( (i >= 'a' && i <= 'z') || 
        (i >= 'A' && i <= 'Z') || 
        (i >= '0' && i <= '9') ||  
        (i == '_'))
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}


SCENARIO("Shorthand \\W matches [^a-zA-Z0-9_]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\W]");

    if( (i >= 'a' && i <= 'z') || 
        (i >= 'A' && i <= 'Z') || 
        (i >= '0' && i <= '9') ||  
        (i == '_'))
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\n matches new line") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\n]");

    if( i == '\n')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\f matches form feed") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\f]");

    if( i == '\f')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\r matches carriage return") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\r]");

    if( i == '\r')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\t matches horizontal tab") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\t]");

    if( i == '\t')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\v matches vertical tab") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\v]");

    if( i == '\v')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\a matches bell") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\a]");

    if( i == '\a')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape \\b matches backspace") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\b]");

    if( i == '\b')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

//TODO below has error
SCENARIO("Character escape \\b matches escape") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\b]");

    if( i == 0x1B)
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape for meta-character '\\'") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\\\]");

    if( i == '\\')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape for meta-character '^'") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\^]");

    if( i == '^')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape for meta-character '['") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\[]");

    if( i == '[')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape for meta-character ']'") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\]]");

    if( i == ']')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}

SCENARIO("Character escape for meta-character '-'") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    
    auto regex = Regex("[\\-]");

    if( i == '-')
    {
        REQUIRE(regex.match(utf8Encode(i)));
    }
    else
    {
        REQUIRE(!regex.match(utf8Encode(i)));
    }
}


//TODO this test should be done outside of character class if code is common
SCENARIO("Character by codepoint using syntax \\uXXXXXX") 
{
    SECTION("1 digit Codepoint is rejected")
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\uF]"), std::runtime_error);
    }

    SECTION("2 digit Codepoint is rejected")
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\uFF]"), std::runtime_error);
    }

    SECTION("3 digit Codepoint is rejected")
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\uFFF]"), std::runtime_error);
    }

    SECTION("4 digit Codepoint is rejected")
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\uFFFF]"), std::runtime_error);
    }

    SECTION("5 digit Codepoint is rejected")
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\uFFFFF]"), std::runtime_error);
    }

    SECTION("Codepoint 0x000000 is rejected") //TODO should this code point be supported? 
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\u000000]"), std::runtime_error);
    }

    SECTION("Codepoints 0x110000 to 0xFFFFFF are rejected")
    {
        auto i = GENERATE(range(0x110000, 0xFFFFFF+1));

        std::ostringstream ss;
        ss << "[\\u" << std::setfill('0') << std::setw(6) << std::hex << i << "]";
        auto exp = ss.str();
        
        
        REQUIRE_THROWS_AS(auto regex = Regex(exp), std::runtime_error);
    }

    SECTION("Codepoints 0x000001 to 0x10FFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0x10FFFF+1));

        std::ostringstream ss;
        ss << "[\\u" << std::setfill('0') << std::setw(6) << std::hex << i << "]";

        auto exp = ss.str();
        auto target = utf8Encode(i);

        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }
}


//TODO this test should be done outside of character class if code is common
SCENARIO("Character by codepoint using syntax \\u{XXXXXX}") 
{
    SECTION("Codepoint 0x000000 is rejected") //TODO should this code point be supported? 
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\u{000000}]"), std::runtime_error);
    }

    SECTION("Invalid syntax: brackets contains non-hexidecimal digit")
    {
        std::string input = GENERATE(
            "[\\u{X00000}]",
            "[\\u{0X0000}]",
            "[\\u{00X000}]",
            "[\\u{000X00}]",
            "[\\u{0000X0}]",
            "[\\u{00000X}]"
        );
        
        REQUIRE_THROWS_AS(auto regex = Regex(input), std::runtime_error);
    }

    SECTION("Invalid syntax: brackets contain more than 6 hexidecimal digits")
    {
        
        REQUIRE_THROWS_AS(auto regex = Regex("[\\u{1234567}]"), std::runtime_error);
    }

    SECTION("Codepoints 0x110000 to 0xFFFFFF are rejected")
    {
        auto i = GENERATE(range(0x110000, 0xFFFFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(6) << std::hex << i << "}]";
        auto exp = ss.str();
        
        
        REQUIRE_THROWS_AS(auto regex = Regex(exp), std::runtime_error);
    }

    SECTION("1 digit Codepoints 0x1-0xF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(1) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("2 digit Codepoints 0x01-0xFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(2) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("3 digit Codepoints 0x001-0xFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(3) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("4 digit Codepoints 0x0001-0xFFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(4) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("5 digit Codepoints 0x00001-0xFFFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFFFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(5) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("6 digit Codepoints 0x000001-0x10FFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0x10FFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(6) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        
        auto regex = Regex(exp);
        REQUIRE(regex.match(target));
    }
}
#endif


} // namespace 
} // namespace regex