#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Regex.hpp"



namespace regex{

namespace {






SCENARIO( "XXX", "[xxxx]" )  
{
    // TODO: unit test this or remove

    {
    auto myRanges = ranges{{2,2},{3,5}, {0,1}, {33,44},{33,37}, {31, 40}, {33,39}};
    DisjoinOverlap(myRanges, 0, 255);

    for (auto x : myRanges)
    {
        //std::cout << x.first << " " << x.second << std::endl;
    }

    CHECK((myRanges == ranges({{0,1}, {2,2}, {3,5}, {6,30}, {31,32}, {33,37}, {38,39}, {40,40}, {41,44}, {45, 255}})));

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
    auto myRanges = ranges{{2,2},{3,5}, {0,1}, {33,44},{33,37}, {31, 40}, {33,39}};
    DisjoinOverlap(myRanges, 0, 255);

    for (auto x : myRanges)
    {
        //std::cout << x.first << " " << x.second << std::endl;
    }

    CHECK((myRanges == ranges({{0,1}, {2,2}, {3,5}, {6,30}, {31,32}, {33,37}, {38,39}, {40,40}, {41,44}, {45, 255}})));

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
    auto myRanges = ranges{{65,65},{124,124}, {124,124}, {66,66}, {255,255}/*,{255,255}*/};
    DisjoinOverlap(myRanges, 0, 255);

    for (auto x : myRanges)
    {
        //std::cout << x.first << " " << x.second << " --- " << std::endl;
    }

    CHECK((myRanges == ranges({{0,64}, {65,65}, {66,66}, {67,123}, {124,124}, {125,254}, {255,255}})));

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

SCENARIO( "Regex", "[regex]" ) 
{
    SECTION("Validate Regex")
    {
        SECTION("Valid (Good) Regex")
        {
            auto goodRegex = GENERATE
            (
                "(a)",
                "((a))",
                "(a)(b)",
                "(a)*",
                "(a)+",
                "(a)|(b)",
                "(a)b",
                "(a*)",
                "(a+)",
                "a**",
                "a++",
                "a*+",
                "a+*",
                "a|b",
                "ab"
            );

            GIVEN( "a valid regex: " << goodRegex ) 
            {
                auto tokenStream = TokenStream(goodRegex);

                WHEN( "the regex is validated")
                {
                    THEN( "the validator exits with success" ) 
                    {
                        REQUIRE(isValidRegex(tokenStream));
                    }
                }
            }
        }

        SECTION("Invalid (Bad) Regex")
        {
            auto badRegex = GENERATE
            (
                "ab|",    // cant end in |
                "ab(",    // cant end in (
                "a(b",    // only one left bracket
                "a)b",    // only one right bracket
                "a)b(",   // right bracket before left bracket
                "(a(b)",  // mismatching number of brackets
                "a()b",   // () is invalid
                "a(*b)",  // (* is invalid
                "a(+b)",  // (+ is invalid
                "a(?b)",  // (? is invalid
                "a(|b)",  // (| is invalid
                "a(b|)",  // |) is invalid
                "a|*b",   // |* is invalid
                "a|+b",   // |+ is invalid
                "a|?b",   // |+ is invalid
                "a||b"    // || is invalid
            );

            GIVEN( "an invalid regex: " << badRegex ) 
            {

                auto tokenStream = TokenStream(badRegex);

                WHEN( "the regex is validated")
                {
                    THEN( "the validator exits with error" ) 
                    {
                        REQUIRE(!isValidRegex(tokenStream));
                    }
                }
            }
        }
    }

    SECTION("Pre-processing inserts concatenation operator ")
    {
        const auto [ input, output ] = GENERATE(table<std::string, std::string>
        ({
            { "ab", "a&b"},
            { "a*", "a*"},
            { "a|b", "a|b"},
            { "a(b)", "a&(b)"},
            { "(a)", "(a)"},
            { "a*b", "a*&b"},
            { "a**", "a**"},
            { "a*+", "a*+"},
            { "a*|b", "a*|b"},
            { "a*(b)", "a*&(b)"},
            { "(a*)", "(a*)"},
            { "a|(b)", "a|(b)"},
            { "((a))", "((a))"},
            { "(a)b", "(a)&b"},
            { "(a)*", "(a)*"},
            { "(a)+", "(a)+"},
            { "(a)|b", "(a)|b"},
            { "(a)(b)", "(a)&(b)"},
            { "a*+", "a*+"},
            { "a+*", "a+*"},
            { "a++", "a++"},
            { "a+|b", "a+|b"},
            { "a+(b)", "a+&(b)"},
            { "(a+)", "(a+)"},
            { "a+b", "a+&b"},
            { "a*?", "a*?"},
            { "a?*", "a?*"},
            { "a??", "a??"},
            { "a?|b", "a?|b"},
            { "a?(b)", "a?&(b)"},
            { "(a?)", "(a?)"},
            { "a?b", "a?&b"},
        }));

        GIVEN( "the regex: " << input ) 
        {
            WHEN( "the regex is pre-processed")
            {
                THEN( "the concatenation operator is inserted correctly" ) 
                {
                    auto inputStream = TokenStream(input);
                    auto preprocessed = InsertExplicitConcat(inputStream);
                    REQUIRE(preprocessed.toString() == output);
                }
            }
        }
    }

    
    SECTION("Converting regex to postfix notation")
    {
        const auto [ input, output ] = GENERATE(table<std::string, std::string>
        ({
            { "ab", "ab&"},
            { "a|b", "ab|"},
            { "a*b", "a*b&"},
            { "(a)", "a"},
            { "((a))", "a"},
            { "(ab)", "ab&"},
            { "(a)*", "a*"},
            { "(a|b)", "ab|"},
            { "(a*b)", "a*b&"},
            { "(a)*b", "a*b&"},
            { "(a)(b)", "ab&"},
            { "(ab)*", "ab&*"},
            { "a**", "a**"},
            { "a|b|c", "ab|c|"},
            { "abc", "ab&c&"},
            { "a(b|c)", "abc|&"},
            { "a|b|c", "ab|c|"},
            { "aa|bb|cc", "aa&bb&|cc&|"},
        }));

        GIVEN( "the infix regex: " << input ) 
        {
            WHEN( "the regex is converted to postfix")
            {
                auto tokenStream = TokenStream(input);
                REQUIRE(isValidRegex(tokenStream));
                auto infix = InsertExplicitConcat(tokenStream);
                auto postfix = InfixToPostfix(infix);

                THEN( "the conversion yields the correct postfix expression" ) 
                {
                    REQUIRE(postfix.toString() == output);
                }
            }
        }
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






} // namespace 
} // namespace regex