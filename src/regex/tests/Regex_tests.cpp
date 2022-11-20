#define CATCH_CONFIG_MAIN
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






SCENARIO( "XXX", "[xxxx]" )  
{
    // TODO: unit test this or remove

    {
    auto myRanges = Alphabet{{2,2},{3,5}, {0,1}, {33,44},{33,37}, {31, 40}, {33,39}};
    DisjoinOverlap(myRanges, 0, 255);

    for (auto x : myRanges)
    {
        //std::cout << int(x.first) << " " << int(x.second) << " --- " << std::endl;
    }

    CHECK((myRanges == Alphabet({{0,1}, {2,2}, {3,5}, {6,30}, {31,32}, {33,37}, {38,39}, {40,40}, {41,44}, {45, 255}})));

    /*
    0 1
    2 2
    3 5
    6 30
    31 32
    33 37
    38 39
    40 40
    41 44
    45 255
    */
    }

    {
    auto myRanges = Alphabet{{2,2},{3,5}, {0,1}, {33,44},{33,37}, {31, 40}, {33,39}};
    DisjoinOverlap(myRanges, 0, 255);

    for (auto x : myRanges)
    {
        //std::cout << int(x.first) << " " << int(x.second) << " --- " << std::endl;
    }

    CHECK((myRanges == Alphabet({{0,1}, {2,2}, {3,5}, {6,30}, {31,32}, {33,37}, {38,39}, {40,40}, {41,44}, {45, 255}})));

    /*
    0 1
    2 2
    3 5
    6 30
    31 32
    33 37
    38 39
    40 40
    41 44
    45 255
    */
    }


    


    {
    auto myRanges = Alphabet{{65,65},{124,124}, {124,124}, {66,66}, {255,255}/*,{255,255}*/};
    DisjoinOverlap(myRanges, 0, 255);

    for (auto x : myRanges)
    {
        //std::cout << int(x.first) << " " << int(x.second) << " --- " << std::endl;
    }

    CHECK((myRanges == Alphabet({{0,64}, {65,65}, {66,66}, {67,123}, {124,124}, {125,254}, {255,255}})));

    /*
    0 1
    2 2
    3 5
    6 30
    31 32
    33 37
    38 39
    40 40
    41 44
    45 255
    */
    }
    
}


SCENARIO( "Empty", "[empty]" ) 
{
    {
        auto regex = Regex();
        regex.compile("a");
        REQUIRE(!regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(!regex.match("aa"));
    }

    {
        auto regex = Regex();
        regex.compile("a|b");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("ab"));
        REQUIRE(!regex.match("ba"));
        REQUIRE(!regex.match(""));
    }

    {
        auto regex = Regex();
        regex.compile("ab");
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
        auto regex = Regex();
        regex.compile("a*");
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
    }

    {
        auto regex = Regex();
        regex.compile("a+");
        REQUIRE(!regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
    }

    {
        auto regex = Regex();
        regex.compile("a?");
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaa"));
    }

    {
        auto regex = Regex();
        regex.compile("a*(b|c)");
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
        auto regex = Regex();
        regex.compile("((ab)|(cd))*e");
        REQUIRE(regex.match("e"));
        REQUIRE(regex.match("abe"));
        REQUIRE(regex.match("cde"));
        REQUIRE(regex.match("ababe"));
        REQUIRE(regex.match("cdcde"));
        REQUIRE(regex.match("abcde"));
        REQUIRE(regex.match("cdabe"));
    }

    {
        auto regex = Regex();
        regex.compile("A\\|\\|B");
        REQUIRE(regex.match("A||B"));
    }

    {
        auto regex = Regex();
        regex.compile("A\\*B");
        REQUIRE(regex.match("A*B"));
    }

    {
        auto regex = Regex();
        regex.compile("\\(abc\\)");
        REQUIRE(regex.match("(abc)"));
    }

    {
        auto regex = Regex();
        regex.compile("while|if|void|break");
        REQUIRE(regex.match("if"));
        REQUIRE(regex.match("while"));
        REQUIRE(regex.match("void"));
        REQUIRE(regex.match("break"));

    }

    //  ANY (.)
    {
        auto regex = Regex();
        regex.compile("a.*b");
        REQUIRE(regex.match("a123b"));
        REQUIRE(regex.match("a456b"));
    }

    //  Digit (\d)
    {
        auto regex = Regex();
        regex.compile("a\\db");
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
        auto regex = Regex();
        regex.compile("a\\Db");
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
        auto regex = Regex();
        regex.compile("a\\sb");
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
        auto regex = Regex();
        regex.compile("a\\Sb");
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
        auto regex = Regex();
        regex.compile("a\\wb");
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("a1b"));
        REQUIRE(regex.match("a_b"));
        REQUIRE(!regex.match("a\vb"));
        REQUIRE(!regex.match("a.b"));
        REQUIRE(!regex.match("a+b"));
    }

    //  Non Word Character (\W)
    {
        auto regex = Regex();
        regex.compile("a\\Wb");
        REQUIRE(!regex.match("aab"));
        REQUIRE(!regex.match("a1b"));
        REQUIRE(!regex.match("a_b"));
        REQUIRE(regex.match("a\vb"));
        REQUIRE(regex.match("a.b"));
        REQUIRE(regex.match("a+b"));
    }

    //  Newline
    {
        auto regex = Regex();
        regex.compile("\\n");
        REQUIRE(regex.match("\n"));
    }

    //  FormFeed
    {
        auto regex = Regex();
        regex.compile("\\f");
        REQUIRE(regex.match("\f"));
    }

    //  CarriageReturn
    {
        auto regex = Regex();
        regex.compile("\\r");
        REQUIRE(regex.match("\r"));
    }

    //  Horizontal Tab
    {
        auto regex = Regex();
        regex.compile("\\t");
        REQUIRE(regex.match("\t"));
    }

    //  Vertical Tab
    {
        auto regex = Regex();
        regex.compile("\\v");
        REQUIRE(regex.match("\v"));
    }

    //  Realistic tests using dates
    {
        auto regex = Regex();
        regex.compile("\\d\\d-(\\d\\d|JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC)-\\d\\d(\\d\\d)?");
        REQUIRE(regex.match("12-12-22"));
        REQUIRE(regex.match("12-12-2022"));
        REQUIRE(regex.match("12-OCT-2022"));
    }
    
    //  Unicode characters
    {
        // 「こんにちは世界」 - Hello World! (Kanji)
        const char utf8String[] = {'\xE3', '\x80', '\x8C', '\xE3', '\x81', '\x93', '\xE3', '\x82', '\x93', '\xE3', '\x81', '\xAB', '\xE3', '\x81', '\xA1', '\xE3', '\x81', '\xAF', '\xE4', '\xB8', '\x96', '\xE7', '\x95', '\x8C', '\xE3', '\x80', '\x8D', '\x00'};
        std::string string = utf8String;
        auto regex = Regex();
        regex.compile(string);
        REQUIRE(regex.match(utf8String));
        REQUIRE(!regex.match("hello world!"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[abc]");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(!regex.match("["));
        REQUIRE(!regex.match("]"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[a-c]");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(!regex.match("["));
        REQUIRE(!regex.match("]"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[a-c1-3]");
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
        auto regex = Regex();
        regex.compile("[-]");
        REQUIRE(regex.match("-"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[a-]");
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[ab-]");
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[^-]");
        REQUIRE(!regex.match("-"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[^-a-c]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[^\\-a-c]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[^a-c\\-]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[^-a]");
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
        REQUIRE(regex.match("b"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[-a]");
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));
        REQUIRE(!regex.match("b"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[b-b]");
        REQUIRE(regex.match("b"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("c"));
    }

    //Character class
    {
        auto regex = Regex();
        regex.compile("[^b-b]");
        REQUIRE(!regex.match("b"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("c"));
    }




    //Character class with shorthand
    {
        auto regex = Regex();
        regex.compile("[\\d]");
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


    //Character class
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[c-a]"), std::runtime_error);
    }

    //Character class
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[b-a]"), std::runtime_error);
    }


/*
    {
        auto regex = Regex();
        regex.compile("c");
        regex.search("abcde");
    }

    {
        auto regex = Regex();
        regex.compile("abc");
        regex.search("abcde");
    }

    {
        auto regex = Regex();
        regex.compile("abc");
        regex.search("abcabcabc");
    }

    {
        auto regex = Regex();
        regex.compile("cat|hat");
        regex.search("the cat has a hat");
    }

    {
        auto regex = Regex();
        regex.compile("cat|hat|h.s");
        regex.search("the cat has a hat");
    }

*/

//TODO: unit test this stuff below.!

    //ab (correct)
    /*
    @startuml
hide empty description
[*] --> 4
0 -> 2 : a
0 -> 1 : a
1 -> 5 : b
1 -> 3 : b
2 -> 5 : b
2 -> 3 : b
3 : Final
4 -> 2 : a
4 -> 1 : a
5 : Final
@enduml
*/




    // a*b (correct)
    /*
    @startuml
hide empty description
[*] --> 6
0 -> 4 : a
0 -> 3 : a
0 -> 1 : a
0 -> 0 : a
1 -> 4 : a
1 -> 3 : a
1 -> 1 : a
1 -> 0 : a
1 -> 7 : b
1 -> 5 : b
2 -> 4 : a
2 -> 3 : a
2 -> 1 : a
2 -> 0 : a
2 -> 7 : b
2 -> 5 : b
3 -> 7 : b
3 -> 5 : b
4 -> 7 : b
4 -> 5 : b
5 : Final
6 -> 4 : a
6 -> 3 : a
6 -> 1 : a
6 -> 0 : a
6 -> 7 : b
6 -> 5 : b
7 : Final
@enduml
*/

// a|b
/*
@startuml
hide empty description
[*] --> 6
0 -> 7 : a
0 -> 5 : a
0 -> 1 : a
1 : Final
2 -> 7 : b
2 -> 5 : b
2 -> 3 : b
3 : Final
4 -> 7 : a
4 -> 5 : a
4 -> 1 : a
4 -> 7 : b
4 -> 5 : b
4 -> 3 : b
5 : Final
6 -> 7 : a
6 -> 5 : a
6 -> 1 : a
6 -> 7 : b
6 -> 5 : b
6 -> 3 : b
7 : Final
@enduml

*/
    
}

#if 0
SCENARIO("Shorthand \\d matches [0-9]") 
{
    auto i = GENERATE(range(0x000001, 0x110000));

    auto regex = Regex();
    regex.compile("[\\d]");

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

    auto regex = Regex();
    regex.compile("[\\D]");

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

    auto regex = Regex();
    regex.compile("[\\s]");

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

    auto regex = Regex();
    regex.compile("[\\S]");

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

    auto regex = Regex();
    regex.compile("[\\w]");

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

    auto regex = Regex();
    regex.compile("[\\W]");

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

    auto regex = Regex();
    regex.compile("[\\n]");

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

    auto regex = Regex();
    regex.compile("[\\f]");

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

    auto regex = Regex();
    regex.compile("[\\r]");

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

    auto regex = Regex();
    regex.compile("[\\t]");

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

    auto regex = Regex();
    regex.compile("[\\v]");

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

    auto regex = Regex();
    regex.compile("[\\a]");

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

    auto regex = Regex();
    regex.compile("[\\b]");

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

    auto regex = Regex();
    regex.compile("[\\b]");

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

    auto regex = Regex();
    regex.compile("[\\\\]");

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

    auto regex = Regex();
    regex.compile("[\\^]");

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

    auto regex = Regex();
    regex.compile("[\\[]");

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

    auto regex = Regex();
    regex.compile("[\\]]");

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

    auto regex = Regex();
    regex.compile("[\\-]");

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
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\uF]"), std::runtime_error);
    }

    SECTION("2 digit Codepoint is rejected")
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\uFF]"), std::runtime_error);
    }

    SECTION("3 digit Codepoint is rejected")
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\uFFF]"), std::runtime_error);
    }

    SECTION("4 digit Codepoint is rejected")
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\uFFFF]"), std::runtime_error);
    }

    SECTION("5 digit Codepoint is rejected")
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\uFFFFF]"), std::runtime_error);
    }

    SECTION("Codepoint 0x000000 is rejected") //TODO should this code point be supported? 
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\u000000]"), std::runtime_error);
    }

    SECTION("Codepoints 0x110000 to 0xFFFFFF are rejected")
    {
        auto i = GENERATE(range(0x110000, 0xFFFFFF+1));

        std::ostringstream ss;
        ss << "[\\u" << std::setfill('0') << std::setw(6) << std::hex << i << "]";
        auto exp = ss.str();
        
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile(exp), std::runtime_error);
    }

    SECTION("Codepoints 0x000001 to 0x10FFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0x10FFFF+1));

        std::ostringstream ss;
        ss << "[\\u" << std::setfill('0') << std::setw(6) << std::hex << i << "]";

        auto exp = ss.str();
        auto target = utf8Encode(i);

        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }
}


//TODO this test should be done outside of character class if code is common
SCENARIO("Character by codepoint using syntax \\u{XXXXXX}") 
{
    SECTION("Codepoint 0x000000 is rejected") //TODO should this code point be supported? 
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\u{000000}]"), std::runtime_error);
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
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile(input), std::runtime_error);
    }

    SECTION("Invalid syntax: brackets contain more than 6 hexidecimal digits")
    {
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile("[\\u{1234567}]"), std::runtime_error);
    }

    SECTION("Codepoints 0x110000 to 0xFFFFFF are rejected")
    {
        auto i = GENERATE(range(0x110000, 0xFFFFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(6) << std::hex << i << "}]";
        auto exp = ss.str();
        
        auto regex = Regex();
        REQUIRE_THROWS_AS(regex.compile(exp), std::runtime_error);
    }

    SECTION("1 digit Codepoints 0x1-0xF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(1) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("2 digit Codepoints 0x01-0xFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(2) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("3 digit Codepoints 0x001-0xFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(3) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("4 digit Codepoints 0x0001-0xFFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(4) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("5 digit Codepoints 0x00001-0xFFFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0xFFFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(5) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }

    SECTION("6 digit Codepoints 0x000001-0x10FFFF are accepted")
    {
        auto i = GENERATE(range(0x1, 0x10FFFF+1));

        std::ostringstream ss;
        ss << "[\\u{" << std::setfill('0') << std::setw(6) << std::hex << i << "}]";
        auto exp = ss.str();
        auto target = utf8Encode(i);
        
        auto regex = Regex();
        regex.compile(exp);
        REQUIRE(regex.match(target));
    }
}
#endif


} // namespace 
} // namespace regex