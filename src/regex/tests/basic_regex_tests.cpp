#include "basic_regex.hpp" //intentional first include
#include "catch.hpp"
#include <initializer_list>

namespace regex {

// TODO rename c-style string to null-terminated everywhere

namespace {

SCENARIO( "basic_regex Traits", "[Traits]" ) 
{
    SECTION("character type 'char' Trait")
    {   
        REQUIRE(std::is_same_v<regex::value_type,
                char>);

        REQUIRE(std::is_same_v<regex::traits_type,
                regex_traits<char>>);

        REQUIRE(std::is_same_v<regex::string_type,
                regex_traits<char>::string_type>);

        REQUIRE(std::is_same_v<regex::flag_type,
                regex_constants::syntax_option_type>);
    }

    SECTION("character type 'wchar_t' Trait")
    {
        REQUIRE(std::is_same_v<wregex::value_type,
                wchar_t>);

        REQUIRE(std::is_same_v<wregex::traits_type,
                regex_traits<wchar_t>>);

        REQUIRE(std::is_same_v<wregex::string_type,
                regex_traits<wchar_t>::string_type>);

        REQUIRE(std::is_same_v<wregex::flag_type,
                regex_constants::syntax_option_type>);
    }
}

SCENARIO( "basic_regex API", "[API]" ) 
{
    SECTION("Default construction")
    {
        WHEN("A default constructed regex is created")
        {
            auto re = regex();

            THEN( "re.mark_count() == 0" ) 
            {
                REQUIRE(re.mark_count() == 0);
            }

            THEN( "re.flags() returns default flags" ) 
            {
                REQUIRE(re.flags() == regex_constants::ECMAScript);
            }

            THEN( "re.getloc() returns default (global) locale" ) 
            {
                REQUIRE(re.getloc() == std::locale());
            }
        }
    }

    SECTION("Parameterized construction taking C-Style string and flags")
    {
        GIVEN("A C-Style string and default flags")
        {
            const char * target = "c-string";
            auto flags = regex_constants::syntax_option_type{};

            WHEN("A regex is constructed given the string and flags")
            {
                auto re = regex(target, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == regex_constants::syntax_option_type{});
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Parameterized construction taking pointer, count and flags")
    {
        GIVEN("A pointer, count, and default flags")
        {
            const char * target = "pointer";
            auto flags = regex_constants::syntax_option_type{};
            auto count = std::size_t{4};

            WHEN("A regex is constructed given the pointer, count and flags")
            {
                auto re = regex(target, count, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == regex_constants::syntax_option_type{});
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Copy Construction and copy assignment")
    {
        GIVEN("A regex")
        {
            auto original = regex("abc", regex_constants::icase | regex_constants::nosubs);

            WHEN("A new regex is copy constructed from the original")
            {
                auto copy = regex(original);
                THEN("Both copy and original are identical" ) 
                {
                    // This test is a bit awkward. There is no comparison operator
                    // defined for a regex. Best that can be done is to compare
                    // members indivitually
                    REQUIRE(copy.mark_count() == original.mark_count());
                    REQUIRE(copy.flags() == original.flags());
                    REQUIRE(copy.getloc() == original.getloc());
                }
            }

            WHEN("A new regex is copy assigned from the original")
            {
                auto copy = regex();
                copy = original;
                THEN("Both copy and original are identical" ) 
                {
                    // This test is a bit awkward. There is no comparison operator
                    // defined for a regex. Best that can be done is to compare
                    // members indivitually
                    REQUIRE(copy.mark_count() == original.mark_count());
                    REQUIRE(copy.flags() == original.flags());
                    REQUIRE(copy.getloc() == original.getloc());
                }
            }
        }
    }

    SECTION("Move Construction and move assignment")
    {
        GIVEN("A regex")
        {
            auto original = regex("abc", regex_constants::icase | regex_constants::nosubs);

            WHEN("A copy of the original is made")
            {
                auto copy = original;

                WHEN("A new regex is move constructed from the original")
                {
                    auto moved = regex(std::move(original));
                    THEN("Both copy and moved are identical" ) 
                    {
                        // This test is a bit awkward. There is no comparison operator
                        // defined for a regex. Best that can be done is to compare
                        // members indivitually
                        REQUIRE(moved.mark_count() == copy.mark_count());
                        REQUIRE(moved.flags() == copy.flags());
                        REQUIRE(copy.getloc() == original.getloc());
                    }
                }

                WHEN("A new regex is move assigned from the original")
                {
                    auto moved = std::move(original);
                    THEN("Both copy and moved are identical" ) 
                    {
                        // This test is a bit awkward. There is no comparison operator
                        // defined for a regex. Best that can be done is to compare
                        // members indivitually
                        REQUIRE(moved.mark_count() == copy.mark_count());
                        REQUIRE(moved.flags() == copy.flags());
                        REQUIRE(copy.getloc() == original.getloc());
                    }
                }
            }
        }
    }

    SECTION("Parameterized construction taking basic_string and flags")
    {
        GIVEN("A basic_string and default flags")
        {
            auto string = std::string("basic_string");
            auto flags = regex_constants::syntax_option_type{};

            WHEN("A regex is constructed given the basic_string and flags")
            {
                auto re = regex(string, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == regex_constants::syntax_option_type{});
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Parameterized construction taking begin+end iterators and flags")
    {
        GIVEN("Begin and end iterators and default flags")
        {
            auto string = std::string("basic_string");
            auto flags = regex_constants::syntax_option_type{};

            WHEN("A regex is constructed given the iterators and flags")
            {
                auto re = regex(string.begin(), string.end(), flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == regex_constants::syntax_option_type{});
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Parameterized construction taking initializer list and flags")
    {
        GIVEN("An initializer list and default flags")
        {
            std::initializer_list<char> il = {'a','b','c'};
            auto flags = regex_constants::syntax_option_type{};

            WHEN("A regex is constructed given the intializer list and flags")
            {
                auto re = regex(il, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == regex_constants::syntax_option_type{});
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Assignment operator with c-style string") 
    {
        GIVEN("A default constructed regex and a c-style string")
        {
            const char * target = "c-string";
            auto re = regex();

            WHEN("The c-style string is assigned to the regex")
            {
                re = target;

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns regex_constants::ECMAScript" ) 
                {
                    REQUIRE(re.flags() == regex_constants::ECMAScript);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Assignment operator with initialization_list") 
    {
        GIVEN("A default constructed regex and an initialization_list")
        {
            std::initializer_list<char> il = {'a','b','c'};
            auto re = regex();

            WHEN("The initialization_list is assigned to the regex")
            {
                re = il;

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns regex_constants::ECMAScript" ) 
                {
                    REQUIRE(re.flags() == regex_constants::ECMAScript);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("Assignment operator with basic_string") 
    {
        GIVEN("A default constructed regex and a basic_string")
        {
            auto string = std::string("basic_string");
            auto re = regex();

            WHEN("The basic_string is assigned to the regex")
            {
                re = string;

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns regex_constants::ECMAScript" ) 
                {
                    REQUIRE(re.flags() == regex_constants::ECMAScript);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("assign() using value (copy) semantics")
    {
        GIVEN("Two regexes 'A' and 'B'")
        {
            auto A = regex("abc", regex_constants::icase | regex_constants::nosubs);
            auto B = regex();

            WHEN("B.assign(A) is called")
            {
                B.assign(A);

                THEN("'A' and 'B' contain the same content" ) 
                {
                    REQUIRE(A.mark_count() == B.mark_count());
                    REQUIRE(A.flags() == B.flags());
                    REQUIRE(A.getloc() == B.getloc());
                }
            }
        }
    }

    SECTION("assign() using move semantics")
    {
        GIVEN("Two regexes 'A' and 'B'")
        {
            auto A = regex("abc", regex_constants::icase | regex_constants::nosubs);
            auto B = regex();

            GIVEN("A copy of 'A'")
            {
                auto copy = A;

                WHEN("B.assign(std::move(A)) is called")
                {
                    B.assign(std::move(A));

                    THEN("Copy of 'A' and 'B' contain the same content" ) 
                    {
                        REQUIRE(A.mark_count() == B.mark_count());
                        REQUIRE(A.flags() == B.flags());
                        REQUIRE(A.getloc() == B.getloc());
                    }
                }
            }
        }
    }

    SECTION("assign() using c-style string and flags")
    {
        GIVEN("A c-style string and flags")
        {
            const char * target = "c-string";
            auto flags = regex_constants::icase | regex_constants::nosubs;

            WHEN("When assign(...) is called on a regex and given the c-style string and flags")
            {
                auto re = regex();
                re.assign(target, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == flags);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("assign() using pointer, count and flags")
    {
        GIVEN("A pointer, count and flags")
        {
            const char * target = "pointer";
            auto flags = regex_constants::icase | regex_constants::nosubs;
            auto count = std::size_t{4};

            WHEN("When assign(...) is called on a regex and given the pointer, count and flags")
            {
                auto re = regex();
                re.assign(target, count, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == flags);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("assign() using basic_string and flags")
    {
        GIVEN("A basic_string and flags")
        {
            auto string = std::string("basic_string");
            auto flags = regex_constants::icase | regex_constants::nosubs;

            WHEN("When assign(...) is called on a regex and given the basic_string and flags")
            {
                auto re = regex();
                re.assign(string, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == flags);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("assign() using begin+end iterators and flags")
    {
        GIVEN("Begin and end iterators and flags")
        {
            auto string = std::string("basic_string");
            auto flags = regex_constants::icase | regex_constants::nosubs;

            WHEN("When assign(...) is called on a regex and given iterators and flags")
            {
                auto re = regex();
                re.assign(string.begin(), string.end(), flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == flags);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("assign() using initializer_list and flags")
    {
        GIVEN("An initializer_list and flags")
        {
            std::initializer_list<char> il = {'a','b','c'};
            auto flags = regex_constants::icase | regex_constants::nosubs;

            WHEN("When assign(...) is called on a regex and given the initializer_list and flags")
            {
                auto re = regex();
                re.assign(il, flags);

                THEN( "re.mark_count() == 0" ) 
                {
                    REQUIRE(re.mark_count() == 0);
                }

                THEN( "re.flags() returns default flags" ) 
                {
                    REQUIRE(re.flags() == flags);
                }

                THEN( "re.getloc() returns default (global) locale" ) 
                {
                    REQUIRE(re.getloc() == std::locale());
                }
            }
        }
    }

    SECTION("getloc() and imbue()")
    {
        GIVEN("A default constructed regex and a custom locale")
        {
            auto re = regex();
            auto custom = std::locale("en_US.UTF-8");
            REQUIRE(custom.name() != std::locale().name());

            WHEN("The locale is imbued")
            {
                auto imbued = re.imbue(custom);

                THEN( "imbue() returns the previous locale" ) 
                {
                    REQUIRE(imbued == std::locale());
                }

                THEN( "re.getloc() will return the imbued locale" ) 
                {
                    REQUIRE(re.getloc() == custom);
                }
            }
        }
    }
}



} //namespace
} //namespace regex 