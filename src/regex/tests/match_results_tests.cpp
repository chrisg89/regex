#include "match_results.hpp" //intentional first include
#include "catch.hpp"

#include <type_traits>
#include <iostream>

namespace regex {

namespace {

SCENARIO( "match_result Traits", "[Traits]" ) 
{
    SECTION("C_style character array Trait")
    {   
        REQUIRE(std::is_same_v<cmatch::allocator_type,
                std::allocator<sub_match<const char*>>>);

        REQUIRE(std::is_same_v<cmatch::value_type,
                sub_match<const char*>>);

        REQUIRE(std::is_same_v<cmatch::const_reference,
                const sub_match<const char*>&>);

        REQUIRE(std::is_same_v<cmatch::reference,
                sub_match<const char*>&>);

        REQUIRE(std::is_same_v<cmatch::const_iterator,
                std::vector<sub_match<const char*>, std::allocator<sub_match<const char*>>>::const_iterator>);

        REQUIRE(std::is_same_v<cmatch::iterator,
                std::vector<sub_match<const char*>, std::allocator<sub_match<const char*>>>::const_iterator>);

        REQUIRE(std::is_same_v<cmatch::difference_type,
                std::iterator_traits<const char*>::difference_type>);

        REQUIRE(std::is_same_v<cmatch::size_type,
                std::allocator_traits<std::allocator<sub_match<const char*>>>::size_type>);

        REQUIRE(std::is_same_v<cmatch::char_type,
                std::iterator_traits<const char*>::value_type>);

        REQUIRE(std::is_same_v<cmatch::string_type,
                std::basic_string<std::iterator_traits<const char*>::value_type>>);
    }

    SECTION("C_style wide character array Trait")
    {
        REQUIRE(std::is_same_v<wcmatch::allocator_type,
                std::allocator<sub_match<const wchar_t*>>>);

        REQUIRE(std::is_same_v<wcmatch::value_type,
                sub_match<const wchar_t*>>);

        REQUIRE(std::is_same_v<wcmatch::const_reference,
                const sub_match<const wchar_t*>&>);

        REQUIRE(std::is_same_v<wcmatch::reference,
                sub_match<const wchar_t*>&>);

        REQUIRE(std::is_same_v<wcmatch::const_iterator,
                std::vector<sub_match<const wchar_t*>, std::allocator<sub_match<const wchar_t*>>>::const_iterator>);

        REQUIRE(std::is_same_v<wcmatch::iterator,
                std::vector<sub_match<const wchar_t*>, std::allocator<sub_match<const wchar_t*>>>::const_iterator>);

        REQUIRE(std::is_same_v<wcmatch::difference_type,
                std::iterator_traits<const wchar_t*>::difference_type>);

        REQUIRE(std::is_same_v<wcmatch::size_type,
                std::allocator_traits<std::allocator<sub_match<const wchar_t*>>>::size_type>);

        REQUIRE(std::is_same_v<wcmatch::char_type,
                std::iterator_traits<const wchar_t*>::value_type>);

        REQUIRE(std::is_same_v<wcmatch::string_type,
                std::basic_string<std::iterator_traits<const wchar_t*>::value_type>>);
    }

    SECTION("String Trait")
    {
        REQUIRE(std::is_same_v<smatch::allocator_type,
                std::allocator<sub_match<std::string::const_iterator>>>);

        REQUIRE(std::is_same_v<smatch::value_type,
                sub_match<std::string::const_iterator>>);

        REQUIRE(std::is_same_v<smatch::const_reference,
                const sub_match<std::string::const_iterator>&>);

        REQUIRE(std::is_same_v<smatch::reference,
                sub_match<std::string::const_iterator>&>);

        REQUIRE(std::is_same_v<smatch::const_iterator,
                std::vector<sub_match<std::string::const_iterator>, std::allocator<sub_match<std::string::const_iterator>>>::const_iterator>);

        REQUIRE(std::is_same_v<smatch::iterator,
                std::vector<sub_match<std::string::const_iterator>, std::allocator<sub_match<std::string::const_iterator>>>::const_iterator>);

        REQUIRE(std::is_same_v<smatch::difference_type,
                std::iterator_traits<std::string::const_iterator>::difference_type>);

        REQUIRE(std::is_same_v<smatch::size_type,
                std::allocator_traits<std::allocator<sub_match<std::string::const_iterator>>>::size_type>);

        REQUIRE(std::is_same_v<smatch::char_type,
                std::iterator_traits<std::string::const_iterator>::value_type>);

        REQUIRE(std::is_same_v<smatch::string_type,
                std::basic_string<std::iterator_traits<std::string::const_iterator>::value_type>>);
    }

    SECTION("Wide String Trait")
    {
        REQUIRE(std::is_same_v<wsmatch::allocator_type,
                std::allocator<sub_match<std::wstring::const_iterator>>>);

        REQUIRE(std::is_same_v<wsmatch::value_type,
                sub_match<std::wstring::const_iterator>>);

        REQUIRE(std::is_same_v<wsmatch::const_reference,
                const sub_match<std::wstring::const_iterator>&>);

        REQUIRE(std::is_same_v<wsmatch::reference,
                sub_match<std::wstring::const_iterator>&>);

        REQUIRE(std::is_same_v<wsmatch::const_iterator,
                std::vector<sub_match<std::wstring::const_iterator>, std::allocator<sub_match<std::wstring::const_iterator>>>::const_iterator>);

        REQUIRE(std::is_same_v<wsmatch::iterator,
                std::vector<sub_match<std::wstring::const_iterator>, std::allocator<sub_match<std::wstring::const_iterator>>>::const_iterator>);

        REQUIRE(std::is_same_v<wsmatch::difference_type,
                std::iterator_traits<std::wstring::const_iterator>::difference_type>);

        REQUIRE(std::is_same_v<wsmatch::size_type,
                std::allocator_traits<std::allocator<sub_match<std::wstring::const_iterator>>>::size_type>);

        REQUIRE(std::is_same_v<wsmatch::char_type,
                std::iterator_traits<std::wstring::const_iterator>::value_type>);

        REQUIRE(std::is_same_v<wsmatch::string_type,
                std::basic_string<std::iterator_traits<std::wstring::const_iterator>::value_type>>);
    }
}

SCENARIO( "match_result API", "[API]" ) 
{
    SECTION("Default construction")
    {
        WHEN("A default constructed match result is created")
        {
            auto result = smatch();

            THEN( "result.ready() is false" ) 
            {
                REQUIRE(result.ready()==false);
            }

            THEN( "result.empty() is true" ) 
            {
                REQUIRE(result.empty()==true);
            }

            THEN( "result.size() is 0" ) 
            {
                REQUIRE(result.size()==0);
            }

            THEN( "result.max_size() some number greater than 0" ) 
            {
                REQUIRE(result.max_size()>0);
            }

            THEN( "result.length(N) returns 0 for any N" ) 
            {
                auto N = GENERATE(0,1,2,3,100);
                REQUIRE(result.length(N)==0);
            }

            THEN( "result.position(N) returns 0 for any N" ) 
            {
                auto N = GENERATE(0,1,2,3,100);
                REQUIRE(result.length(N)==0);
            }

            THEN( "result.str(N) returns empty string for any N" ) 
            {
                auto N = GENERATE(0,1,2,3,100);
                REQUIRE(result.str(N)=="");
            }

            THEN( "result[N] returns empty string for any N" ) 
            {
                auto N = GENERATE(0,1,2,3,100);
                REQUIRE(result[N]=="");
            }

            THEN( "result.prefix() returns empty string" ) 
            {
                REQUIRE(result.prefix()=="");
            }

            THEN( "result.suffix() returns empty string" ) 
            {
                REQUIRE(result.suffix()=="");
            }

            THEN( "result.begin() == result.end()" ) 
            {
                REQUIRE(result.begin() == result.end());
            }
        }
    }

    SECTION("Parameterized construction (allocator is passed in)")
    {
        GIVEN("A allocator")
        {
            smatch::allocator_type allocator;

            WHEN("A match result is constructed given the allocator")
            {
                auto result = smatch(allocator);

                THEN( "result.get_allocator() returns the allocator used to construct the object" ) 
                {
                    REQUIRE(result.get_allocator()==allocator);
                }
            }
        }
    }

    SECTION("Copy Construction and copy assignment")
    {
        GIVEN("A match result")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto original = smatch();
            original.set_prefix(A, B, true);
            original.set_suffix(F, F, false);
            original.add_submatch(B, F, true);
            original.add_submatch(D, F, true);
            original.set_ready();

            WHEN("A new match result is copy constructed from the original")
            {
                auto copy = smatch(original);
                THEN("Both copy and original are identical" ) 
                {
                    REQUIRE((copy==original));
                }
            }

            WHEN("A new match result is copy assigned from the original")
            {
                auto copy = smatch();
                copy = original;
                THEN("Both copy and original are identical" ) 
                {
                    REQUIRE((copy==original));
                }
            }
        }
    }

    SECTION("Move Construction and move assignment")
    {
        GIVEN("A match result")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto original = smatch();
            original.set_prefix(A, B, true);
            original.set_suffix(F, F, false);
            original.add_submatch(B, F, true);
            original.add_submatch(D, F, true);
            original.set_ready();

            WHEN("A copy of the original is made")
            {
                auto copy = original;

                WHEN("A new match result is move constructed from the original")
                {
                    auto moved = smatch(std::move(original));
                    THEN("Both copy and moved are identical" ) 
                    {
                        REQUIRE((moved==copy));
                    }
                }

                WHEN("A new match result is move assigned from the original")
                {
                    auto moved = std::move(original);
                    THEN("Both copy and moved are identical" ) 
                    {
                        REQUIRE((moved==copy));
                    }
                }
            }
        }
    }

    SECTION("match_results.ready() checks if the results are available ")
    {
        WHEN("the match result has not been made ready")
        {
            auto result = smatch();

            THEN("ready() returns false")
            {
                REQUIRE(result.ready() == false);
            }
        }

        WHEN("the match result has been made ready")
        {
            auto result = smatch();
            result.set_ready();

            THEN("ready() returns true")
            {
                REQUIRE(result.ready() == true);
            }
        }
    }

    SECTION("match_results.empty() checks if the match is empty")
    {
        WHEN("the match is empty")
        {
            auto result = smatch();

            THEN("empty() returns true")
            {
                REQUIRE(result.empty() == true);
            }
        }

        WHEN("the match is not empty")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.add_submatch(B, F, true);
            result.add_submatch(D, F, true);
            result.set_ready();

            THEN("empty() returns false")
            {
                REQUIRE(result.empty() == false);
            }
        }
    }

    SECTION("match_results.size() returns the number of submatches")
    {
        WHEN("there are no submatches")
        {
            auto result = smatch();

            THEN("size() returns 0")
            {
                REQUIRE(result.size() == 0);
            }
        }

        WHEN("there is a single submatch")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.add_submatch(B, F, true);
            result.set_ready();

            THEN("size() returns 1")
            {
                REQUIRE(result.size() == 1);
            }
        }

        WHEN("there are multiple submatchs (4)")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.add_submatch(B, F, true);
            result.add_submatch(C, F, true);
            result.add_submatch(D, F, true);
            result.add_submatch(E, F, true);
            result.set_ready();

            THEN("size() returns 4")
            {
                REQUIRE(result.size() == 4);
            }
        }
    }

    SECTION("match_results.max_size() returns the maximum number of submatches the match_results type is able to hold")
    {
        WHEN("max_size() is invoked")
        {
            auto result = smatch();

            // Cant test this thouroughly since its an implementation detail
            // and dependent on the underlying container
            THEN("max_size() returns a value > 0")
            {
                REQUIRE(result.max_size() > 0);
            }
        }
    }

    SECTION("match_results.length(n) returns the length of the particular sub-match")
    {
        GIVEN("A results match containing 2 sub-matches")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.add_submatch(A, F, true);
            result.add_submatch(A, C, true);
            result.add_submatch(C, E, true);
            result.set_ready();

            THEN("length(0) returns length of the entire matched expression ")
            {
                REQUIRE(result.length(0) == 5);  // ABCDE
            }

            THEN("length(1) returnslength of the first submatch")
            {
                REQUIRE(result.length(1) == 2);  // AB
            }

            THEN("length(2) returns length of the second submatch")
            {
                REQUIRE(result.length(2) == 2);  // CD
            }

            THEN("length(3) returns length of the unmatched match")
            {
                REQUIRE(result.length(3) == 0);  // <empty>
            }
        }
    }

    SECTION("match_results.position(n) returns the position of the first character of the particular sub-match")
    {
        GIVEN("A results match containing 2 sub-matches")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.set_unmatched(TargetSequence.end(),TargetSequence.end(), false);  //TODO this is ugly...
            result.add_submatch(A, F, true);
            result.add_submatch(A, C, true);
            result.add_submatch(C, E, true);
            result.set_ready();

            THEN("position(0) returns position of first character of the entire matched expression ")
            {
                REQUIRE(result.position(0) == 0);  // position of "A" in "ABCDEF"
            }

            THEN("position(1) returns position of first character of the first submatch")
            {
                REQUIRE(result.position(1) == 0);  // position of "A" in "ABCDEF"
            }

            THEN("position(2) returns position of first character of the second submatch")
            {
                REQUIRE(result.position(2) == 2);  // position of "C" in "ABCDEF"
            }

            THEN("position(3) returns position of first character of the unmatched match")
            {
                REQUIRE(result.position(3) == 6);  // position after "F" in "ABCDEF"
            }
        }
    }

    SECTION("match_results.str(n) returns the sequence of characters for the particular sub-match")
    {
        GIVEN("A results match containing 2 sub-matches")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.set_unmatched(TargetSequence.end(),TargetSequence.end(), false);  //TODO this is ugly...
            result.add_submatch(A, F, true);
            result.add_submatch(A, C, true);
            result.add_submatch(C, E, true);
            result.set_ready();

            THEN("str(0) returns the sequence of characters of the entire matched expression ")
            {
                REQUIRE(result.str(0) == "ABCDE");
            }

            THEN("str(1) returns the sequence of characters of the first submatch")
            {
                REQUIRE(result.str(1) == "AB");
            }

            THEN("str(2) returns the sequence of characters of the second submatch")
            {
                REQUIRE(result.str(2) == "CD");
            }

            THEN("str(3) returns the sequence of characters of the unmatched match")
            {
                REQUIRE(result.str(3) == "");
            }
        }
    }

    SECTION("match_results.operator[](n) returns the specified sub-match")
    {
        GIVEN("A results match containing 2 sub-matches")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();

            auto ABCDE = ssub_match();
            ABCDE.first = A;
            ABCDE.second = E+1;
            ABCDE.matched = true;

            auto AB = ssub_match();
            AB.first = A;
            AB.second = B+1;
            AB.matched = true;

            auto CD = ssub_match();
            CD.first = A;
            CD.second = D+1;
            CD.matched = true;

            result.set_prefix(A, B, true);
            result.set_suffix(F, F, false);
            result.set_unmatched(TargetSequence.end(),TargetSequence.end(), false);  //TODO this is ugly...
            result.add_submatch(ABCDE.first, ABCDE.second, ABCDE.matched);
            result.add_submatch(AB.first, AB.second, AB.matched);
            result.add_submatch(CD.first, CD.second, CD.matched);
            result.set_ready();

            THEN("operator[](0) returns the sub-match of the entire matched expression ")
            {
                REQUIRE(result[0] == ABCDE);
            }

            THEN("operator[](1) returns the sub-match of the first submatch")
            {
                REQUIRE(result[1] == AB);
            }

            THEN("operator[](2) returns the sub-match of the second submatch")
            {
                REQUIRE(result[2] == CD);
            }

            THEN("operator[](3) returns an empty subrange of the target sequence")
            {
                REQUIRE(result[3].length() == 0);
                REQUIRE(result[3].str() == "");
            }
        }
    }

    SECTION("match_results.prefix() returns the prefix sub-match")
    {
        WHEN("A prefix is added to a result match")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            auto prefix = ssub_match();
            prefix.first = A;
            prefix.second = E+1;
            prefix.matched = true;
            result.set_prefix(prefix.first, prefix.second, prefix.matched);
            result.set_ready();

            THEN("prefix() returns the prefix sub-match")
            {
                REQUIRE(result.prefix() == prefix);
            }
        }
    }

    SECTION("match_results.suffix() returns the suffix sub-match")
    {
        WHEN("A suffix is added to a result match")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            auto result = smatch();
            auto suffix = ssub_match();
            suffix.first = A;
            suffix.second = E+1;
            suffix.matched = true;
            result.set_suffix(suffix.first, suffix.second, suffix.matched);
            result.set_ready();

            THEN("suffix() returns the suffix sub-match")
            {
                REQUIRE(result.suffix() == suffix);
            }
        }
    }

    SECTION("match_results.begin() and match_results.end() iterators")
    {
        GIVEN("A target sequence of 'ABCDEF'")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5; 

            GIVEN("A an empty results match")
            {
                auto result = smatch();
                result.set_ready();

                WHEN("Iterating over the results")
                {
                    int count = 0;
                    bool mismatch = false;

                    for(auto it = result.begin(); it != result.end(); it++)
                    {
                        mismatch |= (*it != result[count]);
                        count++;
                    }

                    THEN("The correct number of iterations are made and content is correct")
                    {
                        REQUIRE(count == 0);
                        REQUIRE(mismatch == false);
                    }
                }
            }

            GIVEN("A results match containing 6 sub-matches")
            {
                auto result = smatch();
                result.add_submatch(A, A, true);
                result.add_submatch(A, B, true);
                result.add_submatch(A, C, true);
                result.add_submatch(A, D, true);
                result.add_submatch(A, E, true);
                result.add_submatch(A, F, true);
                result.set_ready();

                WHEN("Iterating over the results")
                {
                    int count = 0;
                    bool mismatch = false;

                    for(auto it = result.begin(); it != result.end(); it++)
                    {
                        mismatch |= (*it != result[count]);
                        count++;
                    }

                    THEN("The correct number of iterations are made and content is correct")
                    {
                        REQUIRE(count == 6);
                        REQUIRE(mismatch == false);
                    }
                }
            }
        }
    }

    SECTION("match_results.swap(...) and swap(...) swap the contents")
    {
        GIVEN("A target sequence of 'ABCDEF'")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5; 

            GIVEN("Two match results formulated from the target sequence")
            {
                auto result1 = smatch();
                result1.set_prefix(A, B, true);
                result1.set_suffix(B, C, true);
                result1.add_submatch(A, B, true);
                result1.add_submatch(A, C, true);
                result1.add_submatch(A, D, true);
                result1.add_submatch(A, E, true);
                result1.add_submatch(A, F, true);
                result1.set_ready();

                auto result2 = smatch();
                result2.set_prefix(C, D, true);
                result2.set_suffix(D, E, true);
                result2.add_submatch(B, B, true);
                result2.add_submatch(B, C, true);
                result2.add_submatch(B, D, true);
                result2.add_submatch(B, E, true);
                result2.add_submatch(B, F, true);
                result2.set_ready();

                GIVEN("Copies of the two match results are made")
                {
                    auto result1Copy = result1;
                    auto result2Copy = result2;

                    WHEN("The originals are swapped via match_results.swap(...)")
                    {
                        result1.swap(result2);

                        THEN("The swapped results are equivalent to their counterpart's copy")
                        {
                            REQUIRE((result1 == result2Copy));
                            REQUIRE((result2 == result1Copy));
                        }
                    }

                    WHEN("The originals are swapped via swap(...)")
                    {
                        swap(result1, result2);

                        THEN("The swapped results are equivalent to their counterpart's copy")
                        {
                            REQUIRE((result1 == result2Copy));
                            REQUIRE((result2 == result1Copy));
                        }
                    }
                }
            }
        }
    }

    SECTION("Equality operators")
    {
        GIVEN("A target sequence of 'ABCDEF'")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5; 

            GIVEN("Two equivalent match results")
            {
                auto result1 = smatch();
                result1.set_suffix(B, C, true);
                result1.add_submatch(A, B, true);

                auto result2 = smatch();
                result2.set_suffix(B, C, true);
                result2.add_submatch(A, B, true);

                REQUIRE((result1 == result2));

                WHEN("One result is made ready while the other is not")
                {
                    result2.set_ready();

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                } 

                WHEN("One result has an extra sub-match added")
                {
                    result2.add_submatch(C, D, true);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                }    

                WHEN("Both results have sub-matches added with different content")
                {
                    result1.add_submatch(C, D, true);
                    result2.add_submatch(D, E, true);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                }

                WHEN("Both results have sub-matches added with diffent 'matched' state")
                {
                    result1.add_submatch(C, D, true);
                    result2.add_submatch(C, D, false);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                } 

                WHEN("One result has its prefix's content changed'")
                {
                    result1.set_prefix(A, B, true);
                    result2.set_prefix(A, C, true);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                }

                WHEN("One result has its prefix's 'matched' state changed'")
                {
                    result1.set_prefix(A, B, true);
                    result2.set_prefix(A, B, false);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                }

                WHEN("One result has its suffix's content changed'")
                {
                    result1.set_suffix(A, B, true);
                    result2.set_suffix(A, C, true);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                }

                WHEN("One result has its suffix's 'matched' state changed'")
                {
                    result1.set_suffix(A, B, true);
                    result2.set_suffix(A, B, false);

                    THEN("They are no longer deemed to be equivalent")
                    {
                        REQUIRE_FALSE((result1 == result2));
                        REQUIRE((result1 != result2));
                    }
                }
            }
        }
    }
}

} //namespace
} //namespace regex 