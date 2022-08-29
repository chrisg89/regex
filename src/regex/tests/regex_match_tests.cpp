#include "regex_match.hpp" //intentional first include
#include "catch.hpp"

namespace regex {

namespace {

SCENARIO( "regex_match API", "[API]" ) 
{
    SECTION("Overload #1: Using iterators (with results)")
    {
        SECTION("iterator is a char pointer")
        {
            GIVEN("A regex and match result object")
            {
                auto re = regex("a.*b");
                auto m = cmatch();

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const char * target = "aaaaaab";
                    const char * first = target;
                    const char * last = first + regex::traits_type::length(target);

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful and the match result is returned")
                        {
                            REQUIRE(regex_match(first, last, m, re));
                            REQUIRE(m.ready());
                            REQUIRE(!m.empty());
                            REQUIRE(m.size() == 1);
                            REQUIRE(m.length(0) == regex::traits_type::length(target));
                            REQUIRE(m.position(0) == 0);
                            REQUIRE(m.str(0) == "aaaaaab");
                            REQUIRE(m.prefix().str() == "");
                            REQUIRE(m.suffix().str() == "");
                        }
                    }
                }
            }
        }

        SECTION("iterator is a wchar_t pointer")
        {
            /* TODO: wchar_t is not yet supported in the impl
            GIVEN("A regex and match result object")
            {
                auto re = wregex(L"a.*b");
                auto m = wcmatch();
                
                GIVEN("The target sequence of 'aaaaaab")
                {
                    const char * target = "aaaaaab";
                    const char * first = target;
                    const char * last = first + regex::traits_type::length(target);

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful and the match result is returned")
                        {
                            REQUIRE(regex_match(first, last, m, re));
                            REQUIRE(m.ready());
                            REQUIRE(!m.empty());
                            REQUIRE(m.size() == 1);
                            REQUIRE(m.length(0) == regex::traits_type::length(target));
                            REQUIRE(m.position(0) == 0);
                            REQUIRE(m.str(0) == "aaaaaab");
                            REQUIRE(m.prefix().str() == "");
                            REQUIRE(m.suffix().str() == "");
                        }
                    }
                }
            }
            */  
        }

        SECTION("iterator is a basic_string<char>::const_iterator")
        {
            GIVEN("A regex and match result object")
            {
                auto re = regex("a.*b");
                auto m = smatch();

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const std::string target("aaaaaab");

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful and the match result is returned")
                        {
                            REQUIRE(regex_match(target.begin(), target.end() , m, re));
                            REQUIRE(m.ready());
                            REQUIRE(!m.empty());
                            REQUIRE(m.size() == 1);
                            REQUIRE(m.length(0) == target.length());
                            REQUIRE(m.position(0) == 0);
                            REQUIRE(m.str(0) == "aaaaaab");
                            REQUIRE(m.prefix().str() == "");
                            REQUIRE(m.suffix().str() == "");
                        }
                    }
                }
            }
        }

        SECTION("iterator is a basic_string<wchar_t>::const_iterator")
        {
            /* TODO: implement this */
        }
    }

    SECTION("Overload #2: Using iterators (without results)")
    {
        SECTION("iterator is a char pointer")
        {
            GIVEN("A regex object")
            {
                auto re = regex("a.*b");

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const char * target = "aaaaaab";
                    const char * first = target;
                    const char * last = first + regex::traits_type::length(target);

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful")
                        {
                            REQUIRE(regex_match(first, last, re));
                        }
                    }
                }
            }
        }

        SECTION("iterator is a wchar_t pointer")
        {
            /* TODO: implement this */
        }

        SECTION("iterator is a basic_string<char>::const_iterator")
        {
            GIVEN("A regex object")
            {
                auto re = regex("a.*b");

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const std::string target("aaaaaab");

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful and the match result is returned")
                        {
                            REQUIRE(regex_match(target.begin(), target.end(), re));
                        }
                    }
                }
            }
        }

        SECTION("iterator is a basic_string<wchar_t>::const_iterator")
        {
            /* TODO: implement this */
        }
    }

    SECTION("Overload #3: C-style string (with results)")
    {
        SECTION("C-Style string when character type is char")
        {
            GIVEN("A regex and match result object")
            {
                auto re = regex("a.*b");
                auto m = cmatch();

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const char * target = "aaaaaab";

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful")
                        {
                            REQUIRE(regex_match(target, m, re));
                            REQUIRE(m.ready());
                            REQUIRE(!m.empty());
                            REQUIRE(m.size() == 1);
                            REQUIRE(m.length(0) == regex::traits_type::length(target));
                            REQUIRE(m.position(0) == 0);
                            REQUIRE(m.str(0) == "aaaaaab");
                            REQUIRE(m.prefix().str() == "");
                            REQUIRE(m.suffix().str() == "");
                        }
                    }
                }
            }
        }

        SECTION("C-Style string when character type is wchar_t")
        {
            /* TODO: implement this */
        }
    }

    SECTION("Overload #4: std::basic_string (with results)")
    {
        SECTION("basic_string is specialized on char")
        {
            GIVEN("A regex and match result object")
            {
                auto re = regex("a.*b");
                auto m = smatch();

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const std::string target("aaaaaab");

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful and the match result is returned")
                        {
                            REQUIRE(regex_match(target, m, re));
                            REQUIRE(m.ready());
                            REQUIRE(!m.empty());
                            REQUIRE(m.size() == 1);
                            REQUIRE(m.length(0) == target.length());
                            REQUIRE(m.position(0) == 0);
                            REQUIRE(m.str(0) == "aaaaaab");
                            REQUIRE(m.prefix().str() == "");
                            REQUIRE(m.suffix().str() == "");
                        }
                    }
                }
            }
        }

        SECTION("basic_string is specialized on wchar_t")
        {
            /* TODO: implement this */ 
        }
    }

    SECTION("Overload #5: C-style string (without results)")
    {
        SECTION("C-Style string when character type is char")
        {
            GIVEN("A regex object")
            {
                auto re = regex("a.*b");

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const char * target = "aaaaaab";

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful")
                        {
                            REQUIRE(regex_match(target, re));
                        }
                    }
                }
            }
        }

        SECTION("C-Style string when character type is wchar_t")
        {
            /* TODO: implement this */
        }
    }

    SECTION("Overload #6: std::basic_string (without results)")
    {
        SECTION("basic_string is specialized on char")
        {
            GIVEN("A regex object")
            {
                auto re = regex("a.*b");

                GIVEN("The target sequence of 'aaaaaab")
                {
                    const std::string target("aaaaaab");

                    WHEN("The target sequence is matched against the regex")
                    {
                        THEN("The match is successful and the match result is returned")
                        {
                            REQUIRE(regex_match(target, re));
                        }
                    }
                }
            }
        }

        SECTION("basic_string is specialized on wchar_t")
        {
            /* TODO: implement this */ 
        }
    }
}


SCENARIO( "TODOTODO: this is a functional test not API", "[Traits]" ) 
{

    //regex a;  //empty detail::Regex is broken TODO
    //TODO need to test the above!


    {
        auto re = regex("a");
        REQUIRE(!regex_match("",re));
        REQUIRE(regex_match("a",re));
        REQUIRE(!regex_match("aa",re));
    }


    {
        auto re = regex("a|b");
        REQUIRE(regex_match("a",re));
        REQUIRE(regex_match("b",re));
        REQUIRE(!regex_match("aa",re));
        REQUIRE(!regex_match("bb",re));
        REQUIRE(!regex_match("ab",re));
        REQUIRE(!regex_match("ba",re));
        REQUIRE(!regex_match("",re));
    }

    {
        auto re = regex("ab");
        REQUIRE(regex_match("ab",re));
        REQUIRE(!regex_match("a",re));
        REQUIRE(!regex_match("b",re));
        REQUIRE(!regex_match("ba",re));
        REQUIRE(!regex_match("aa",re));
        REQUIRE(!regex_match("bb",re));
        REQUIRE(!regex_match("abb",re));
        REQUIRE(!regex_match("aba",re));
        REQUIRE(!regex_match("",re));
    }

    {
        auto re = regex("a*");
        REQUIRE(regex_match("",re));
        REQUIRE(regex_match("a",re));
        REQUIRE(regex_match("aa",re));
        REQUIRE(regex_match("aaa",re));
    }

    {
        auto re = regex("a+");
        REQUIRE(!regex_match("",re));
        REQUIRE(regex_match("a",re));
        REQUIRE(regex_match("aa",re));
        REQUIRE(regex_match("aaa",re));
    }

    {
        auto re = regex("a?");
        REQUIRE(regex_match("",re));
        REQUIRE(regex_match("a",re));
        REQUIRE(!regex_match("aa",re));
        REQUIRE(!regex_match("aaa",re));
    }

    {
        auto re = regex("a*(b|c)");
        REQUIRE(regex_match("b",re));
        REQUIRE(regex_match("ab",re));
        REQUIRE(regex_match("aab",re));
        REQUIRE(regex_match("aaab",re));

        REQUIRE(regex_match("c",re));
        REQUIRE(regex_match("ac",re));
        REQUIRE(regex_match("aac",re));
        REQUIRE(regex_match("aaac",re));

        REQUIRE(!regex_match("",re));
        REQUIRE(!regex_match("a",re));
        REQUIRE(!regex_match("aa",re));
        REQUIRE(!regex_match("aaa",re));

        REQUIRE(!regex_match("aba",re));
        REQUIRE(!regex_match("abc",re));
        REQUIRE(!regex_match("aca",re));
        REQUIRE(!regex_match("acb",re));
    }

    {
        auto re = regex("((ab)|(cd))*e");
        REQUIRE(regex_match("e",re));
        REQUIRE(regex_match("abe",re));
        REQUIRE(regex_match("cde",re));
        REQUIRE(regex_match("ababe",re));
        REQUIRE(regex_match("cdcde",re));
        REQUIRE(regex_match("abcde",re));
        REQUIRE(regex_match("cdabe",re));
    }

    {
        auto re = regex("A\\|\\|B");
        REQUIRE(regex_match("A||B",re));
    }

    {
        auto re = regex("A\\*B");
        REQUIRE(regex_match("A*B",re));
    }

    {
        auto re = regex("\\(abc\\)");
        REQUIRE(regex_match("(abc)",re));
    }

    {
        auto re = regex("while|if|void|break");
        REQUIRE(regex_match("if",re));
        REQUIRE(regex_match("while",re));
        REQUIRE(regex_match("void",re));
        REQUIRE(regex_match("break",re));

    }

    //  ANY (.)
    {
        auto re = regex("a.*b");
        REQUIRE(regex_match("a123b",re));
        REQUIRE(regex_match("a456b",re));
    }

    //  Digit (\d)
    {
        auto re = regex("a\\db");
        REQUIRE(regex_match("a0b",re));
        REQUIRE(regex_match("a1b",re));
        REQUIRE(regex_match("a2b",re));
        REQUIRE(regex_match("a4b",re));
        REQUIRE(regex_match("a5b",re));
        REQUIRE(regex_match("a6b",re));
        REQUIRE(regex_match("a7b",re));
        REQUIRE(regex_match("a8b",re));
        REQUIRE(regex_match("a9b",re));
        REQUIRE(!regex_match("aab",re));
    }

    //  NonDigit (\D)
    {
        auto re = regex("a\\Db");
        REQUIRE(!regex_match("a0b",re));
        REQUIRE(!regex_match("a1b",re));
        REQUIRE(!regex_match("a2b",re));
        REQUIRE(!regex_match("a4b",re));
        REQUIRE(!regex_match("a5b",re));
        REQUIRE(!regex_match("a6b",re));
        REQUIRE(!regex_match("a7b",re));
        REQUIRE(!regex_match("a8b",re));
        REQUIRE(!regex_match("a9b",re));
        REQUIRE(regex_match("aab",re));
    }

        //  Whitespace (\s)
    {
        auto re = regex("a\\sb");
        REQUIRE(regex_match("a b",re));
        REQUIRE(regex_match("a\tb",re));
        REQUIRE(regex_match("a\nb",re));
        REQUIRE(regex_match("a\vb",re));
        REQUIRE(regex_match("a\fb",re));
        REQUIRE(regex_match("a\rb",re));
        REQUIRE(!regex_match("aab",re));
    }

    //  Whitespace (\s)
    {
        auto re = regex("a\\Sb");
        REQUIRE(!regex_match("a b",re));
        REQUIRE(!regex_match("a\tb",re));
        REQUIRE(!regex_match("a\nb",re));
        REQUIRE(!regex_match("a\vb",re));
        REQUIRE(!regex_match("a\fb",re));
        REQUIRE(!regex_match("a\rb",re));
        REQUIRE(regex_match("aab",re));
    }

    //  Word Character (\w)
    {
        auto re = regex("a\\wb");
        REQUIRE(regex_match("aab",re));
        REQUIRE(regex_match("a1b",re));
        REQUIRE(regex_match("a_b",re));
        REQUIRE(!regex_match("a\vb",re));
        REQUIRE(!regex_match("a.b",re));
        REQUIRE(!regex_match("a+b",re));
    }

    //  Non Word Character (\W)
    {
        auto re = regex("a\\Wb");
        REQUIRE(!regex_match("aab",re));
        REQUIRE(!regex_match("a1b",re));
        REQUIRE(!regex_match("a_b",re));
        REQUIRE(regex_match("a\vb",re));
        REQUIRE(regex_match("a.b",re));
        REQUIRE(regex_match("a+b",re));
    }

    //  Newline
    {
        auto re = regex("\\n");
        REQUIRE(regex_match("\n",re));
    }

    //  FormFeed
    {
        auto re = regex("\\f");
        REQUIRE(regex_match("\f",re));
    }

    //  CarriageReturn
    {
        auto re = regex("\\r");
        REQUIRE(regex_match("\r",re));
    }

    //  Horizontal Tab
    {
        auto re = regex("\\t");
        REQUIRE(regex_match("\t",re));
    }

    //  Vertical Tab
    {
        auto re = regex("\\v");
        REQUIRE(regex_match("\v",re));
    }

    //  Realistic tests using dates
    {
        auto re = regex("\\d\\d-(\\d\\d|JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC)-\\d\\d(\\d\\d)?");
        REQUIRE(regex_match("12-12-22",re));
        REQUIRE(regex_match("12-12-2022",re));
        REQUIRE(regex_match("12-OCT-2022",re));
    }


    SECTION("TODO")
    {
        REQUIRE(true);
    }

}


} //namespace
} //namespace regex 