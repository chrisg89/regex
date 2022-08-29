#include "sub_match.hpp" //intentional first include
#include "catch.hpp"

#include <type_traits>
#include <sstream>

namespace regex {

namespace {

SCENARIO( "sub_match Traits", "[Traits]" ) 
{
    SECTION("C_style character array Trait")
    {
        REQUIRE(std::is_same<csub_match::iterator,
                const char*>::value);

        REQUIRE(std::is_same<csub_match::value_type,
                std::iterator_traits<const char*>::value_type>::value);

        REQUIRE(std::is_same<csub_match::difference_type,
                std::iterator_traits<const char*>::difference_type>::value);

        REQUIRE(std::is_same<csub_match::string_type,
                std::basic_string<std::iterator_traits<const char*>::value_type>>::value);
    }

    SECTION("C_style wide character array Trait")
    {
        REQUIRE(std::is_same<wcsub_match::iterator,
                const wchar_t*>::value);

        REQUIRE(std::is_same<wcsub_match::value_type,
                std::iterator_traits<const wchar_t*>::value_type>::value);

        REQUIRE(std::is_same<wcsub_match::difference_type,
                std::iterator_traits<const wchar_t*>::difference_type>::value);

        REQUIRE(std::is_same<wcsub_match::string_type,
                std::basic_string<std::iterator_traits<const wchar_t*>::value_type>>::value);
    }

    SECTION("String Trait")
    {
        REQUIRE(std::is_same<ssub_match::iterator,
                std::string::const_iterator>::value);

        REQUIRE(std::is_same<ssub_match::value_type,
                std::iterator_traits<std::string::const_iterator>::value_type>::value);

        REQUIRE(std::is_same<ssub_match::difference_type,
                std::iterator_traits<std::string::const_iterator>::difference_type>::value);

        REQUIRE(std::is_same<ssub_match::string_type,
                std::basic_string<std::iterator_traits<std::string::const_iterator>::value_type>>::value);
    }

    SECTION("Wide String Trait")
    {
        REQUIRE(std::is_same<wssub_match::iterator,
                std::wstring::const_iterator>::value);

        REQUIRE(std::is_same<wssub_match::value_type,
                std::iterator_traits<std::wstring::const_iterator>::value_type>::value);

        REQUIRE(std::is_same<wssub_match::difference_type,
                std::iterator_traits<std::wstring::const_iterator>::difference_type>::value);

        REQUIRE(std::is_same<wssub_match::string_type,
                std::basic_string<std::iterator_traits<std::wstring::const_iterator>::value_type>>::value);
    }
}

SCENARIO( "sub_match API", "[API]" ) 
{
    SECTION("Default Construction")
    {
        WHEN("The default constructor is called")
        {
            auto submatch = ssub_match();
            THEN( "matched is false and length is 0" ) 
            {
                REQUIRE(submatch.matched==false);
                REQUIRE(submatch.length() == 0);
            }
        }
    }

    SECTION("Move Construction and move assignment")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            GIVEN("Submatch of 'AB'")
            {
                auto AB = ssub_match();
                AB.first = A;
                AB.second = B+1;
                AB.matched = true;

                WHEN("A submatch is move constructed from AB")
                {
                    auto movedAB = ssub_match(std::move(AB));

                    THEN("Members are moved correctly")
                    {
                        REQUIRE(movedAB.matched == true);
                        REQUIRE(movedAB.length() == 2);
                        REQUIRE((movedAB.first == A));
                        REQUIRE((movedAB.second == B+1));
                    }
                }

                WHEN("A submatch is move assigned from AB")
                {
                    auto movedAB = ssub_match();
                    movedAB = std::move(AB);

                    THEN("Members are moved correctly")
                    {
                        REQUIRE(movedAB.matched == true);
                        REQUIRE(movedAB.length() == 2);
                        REQUIRE((movedAB.first == A));
                        REQUIRE((movedAB.second == B+1));
                    }
                }
            }
        }
    }

    SECTION("Copy Construction and copy assignment")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            GIVEN("Submatch of 'AB'")
            {
                auto AB = ssub_match();
                AB.first = A;
                AB.second = B+1;
                AB.matched = true;

                WHEN("A submatch is copy constructed from AB")
                {
                    auto copiedAB = ssub_match(AB);

                    THEN("Members are copied correctly")
                    {
                        REQUIRE(copiedAB.matched == true);
                        REQUIRE(copiedAB.length() == 2);
                        REQUIRE((copiedAB.first == A));
                        REQUIRE((copiedAB.second == B+1));
                    }
                }

                WHEN("A submatch is copy assigned from AB")
                {
                    auto copiedAB = ssub_match();
                    copiedAB = AB;

                    THEN("Members are copied correctly")
                    {
                        REQUIRE(copiedAB.matched == true);
                        REQUIRE(copiedAB.length() == 2);
                        REQUIRE((copiedAB.first == A));
                        REQUIRE((copiedAB.second == B+1));
                    }
                }
            }
        }
    }

    SECTION("Length of a matched subsequence")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            GIVEN("A submatch of ''")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = A;
                submatch.matched = true;
                
                THEN( "length() returns 0" ) 
                {
                    REQUIRE(submatch.length() == 0);
                }
            }

            GIVEN("A submatch of 'A'")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = A+1;
                submatch.matched = true;
                
                THEN( "length() returns 1" ) 
                {
                    REQUIRE(submatch.length() == 1);
                }
            }

            GIVEN("A submatch of 'ABCD'")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = D+1;
                submatch.matched = true;
                
                THEN( "length() returns 4" ) 
                {
                    REQUIRE(submatch.length() == 4);
                }
            }
        }
    }

    SECTION("Convertion of matched subsequence to the underlying string type")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            GIVEN("A submatch of ''")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = A;
                submatch.matched = true;
                
                THEN( "implicit conversion yieds a string of ''" ) 
                {
                    ssub_match::string_type result = submatch;
                    REQUIRE(result=="");
                }

                THEN( "explicit conversion yieds a string of ''" ) 
                {
                    ssub_match::string_type result = submatch.str();
                    REQUIRE(result=="");
                }
            }

            GIVEN("A submatch of 'A'")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = A+1;
                submatch.matched = true;
                
                THEN( "implicit conversion yieds a string of 'A'" ) 
                {
                    ssub_match::string_type result = submatch;
                    REQUIRE(result=="A");
                }

                THEN( "explicit conversion yieds a string of 'A'" ) 
                {
                    ssub_match::string_type result = submatch.str();
                    REQUIRE(result=="A");
                }
            }

            GIVEN("A submatch of 'ABCD'")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = D+1;
                submatch.matched = true;
                
                THEN( "implicit conversion yieds a string of 'ABCD'" ) 
                {
                    ssub_match::string_type result = submatch;
                    REQUIRE(result=="ABCD");
                }

                THEN( "explicit conversion yieds a string of 'ABCD'" ) 
                {
                    ssub_match::string_type result = submatch.str();
                    REQUIRE(result=="ABCD");
                }
            }
        }
    }

    SECTION("Comparison of matched subsequence (using member functions)")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            GIVEN("Submatches of 'AB', 'CD', and 'EF'")
            {
                auto AB = ssub_match();
                AB.first = A;
                AB.second = B+1;
                AB.matched = true;

                auto CD = ssub_match();
                CD.first = C;
                CD.second = D+1;
                CD.matched = true;

                auto EF = ssub_match();
                EF.first = E;
                EF.second = F+1;
                EF.matched = true;

                SECTION("Two submatches are compared")
                {
                    WHEN( "Submatch CD is compared to submatch AB" ) 
                    {
                        THEN("the comparison is greater than 0")
                        {
                            REQUIRE(CD.compare(AB) > 0);
                        }
                    }

                    WHEN( "Submatch CD is compared to submatch CD" ) 
                    {
                        THEN("the comparison is equal to 0")
                        {
                            REQUIRE(CD.compare(CD) == 0);
                        }
                    }

                    WHEN( "Submatch CD is compared to submatch EF" ) 
                    {
                        THEN("the comparison is less than 0")
                        {
                            REQUIRE(CD.compare(EF) < 0);
                        }
                    }
                }

                SECTION("A submatch is compared to a string")
                {
                    WHEN( "Submatch CD is compared to string 'AB'" ) 
                    {
                        THEN("the comparison is greater than 0")
                        {
                            REQUIRE(CD.compare(ssub_match::string_type{"AB"}) > 0);
                        }
                    }

                    WHEN( "Submatch CD is compared to string 'CD'" ) 
                    {
                        THEN("the comparison is equal to 0")
                        {
                            REQUIRE(CD.compare(ssub_match::string_type{"CD"}) == 0);
                        }
                    }

                    WHEN( "Submatch CD is compared to string 'EF'" ) 
                    {
                        THEN("the comparison is less than 0")
                        {
                            REQUIRE(CD.compare(ssub_match::string_type{"EF"}) < 0);
                        }
                    }
                }

                SECTION("A submatch is compared to a c-string")
                {
                    WHEN( "Submatch CD is compared to the c-string 'AB'" ) 
                    {
                        THEN("the comparison is greater than 0")
                        {
                            ssub_match::value_type cStringAB[3] = "AB";
                            REQUIRE(CD.compare(cStringAB) > 0);
                        }
                    }

                    WHEN( "Submatch CD is compared to the c-string 'CD'" ) 
                    {
                        THEN("the comparison is equal to 0")
                        {
                            ssub_match::value_type cStringCD[3] = "CD";
                            REQUIRE(CD.compare(cStringCD) == 0);
                        }
                    }

                    WHEN( "Submatch CD is compared to the c-string 'EF'" ) 
                    {
                        THEN("the comparison is less than 0")
                        {
                            ssub_match::value_type cStringEF[3] = "EF";
                            REQUIRE(CD.compare(cStringEF) < 0);
                        }
                    }
                }

            }
        }
    }

    SECTION("Comparison of matched subsequence (using non-member functions)")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEFY");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;
            ssub_match::iterator Y = TargetSequence.begin() + 6;

            GIVEN("Submatches of 'AB', 'CD', and 'EF'")
            {
                auto AB = ssub_match();
                AB.first = A;
                AB.second = B+1;
                AB.matched = true;

                auto CD = ssub_match();
                CD.first = C;
                CD.second = D+1;
                CD.matched = true;

                auto EF = ssub_match();
                EF.first = E;
                EF.second = F+1;
                EF.matched = true;

                SECTION("Comparison of sub_match to sub_match")
                {
                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((AB==CD));
                            REQUIRE((CD==CD));
                            REQUIRE_FALSE((EF==CD));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((AB!=CD));
                            REQUIRE_FALSE((CD!=CD));
                            REQUIRE((EF!=CD));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((AB<CD));
                            REQUIRE_FALSE((CD<CD));
                            REQUIRE_FALSE((EF<CD));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((AB<=CD));
                            REQUIRE((CD<=CD));
                            REQUIRE_FALSE((EF<=CD));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((AB>CD));
                            REQUIRE_FALSE((CD>CD));
                            REQUIRE((EF>CD));
                        }
                    }

                    WHEN( "Evaluating correctness of >= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((AB>=CD));
                            REQUIRE((CD>=CD));
                            REQUIRE((EF>=CD));
                        }
                    }
                }

                SECTION("Comparison of sub_match to basic_string")
                {
                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((CD==ssub_match::string_type{"AB"}));
                            REQUIRE((CD==ssub_match::string_type{"CD"}));
                            REQUIRE_FALSE((CD==ssub_match::string_type{"EF"}));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((CD!=ssub_match::string_type{"AB"}));
                            REQUIRE_FALSE((CD!=ssub_match::string_type{"CD"}));
                            REQUIRE((CD!=ssub_match::string_type{"EF"}));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((CD<ssub_match::string_type{"AB"}));
                            REQUIRE_FALSE((CD<ssub_match::string_type{"CD"}));
                            REQUIRE((CD<ssub_match::string_type{"EF"}));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((CD<=ssub_match::string_type{"AB"}));
                            REQUIRE((CD<=ssub_match::string_type{"CD"}));
                            REQUIRE((CD<=ssub_match::string_type{"EF"}));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((CD>ssub_match::string_type{"AB"}));
                            REQUIRE_FALSE((CD>ssub_match::string_type{"CD"}));
                            REQUIRE_FALSE((CD>ssub_match::string_type{"EF"}));
                        }
                    }

                    WHEN( "Evaluating correctness of >= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((CD>=ssub_match::string_type{"AB"}));
                            REQUIRE((CD>=ssub_match::string_type{"CD"}));
                            REQUIRE_FALSE((CD>=ssub_match::string_type{"EF"}));
                        }
                    }
                }

                SECTION("Comparison of basic_string to sub_match")
                {
                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((ssub_match::string_type{"AB"}==CD));
                            REQUIRE((ssub_match::string_type{"CD"}==CD));
                            REQUIRE_FALSE((ssub_match::string_type{"EF"}==CD));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((ssub_match::string_type{"AB"}!=CD));
                            REQUIRE_FALSE((ssub_match::string_type{"CD"}!=CD));
                            REQUIRE((ssub_match::string_type{"EF"}!=CD));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((ssub_match::string_type{"AB"}<CD));
                            REQUIRE_FALSE((ssub_match::string_type{"CD"}<CD));
                            REQUIRE_FALSE((ssub_match::string_type{"EF"}<CD));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((ssub_match::string_type{"AB"}<=CD));
                            REQUIRE((ssub_match::string_type{"CD"}<=CD));
                            REQUIRE_FALSE((ssub_match::string_type{"EF"}<=CD));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((ssub_match::string_type{"AB"}>CD));
                            REQUIRE_FALSE((ssub_match::string_type{"CD"}>CD));
                            REQUIRE((ssub_match::string_type{"EF"}>CD));
                        }
                    }

                    WHEN( "Evaluating correctness of >= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((ssub_match::string_type{"AB"}>=CD));
                            REQUIRE((ssub_match::string_type{"CD"}>=CD));
                            REQUIRE((ssub_match::string_type{"EF"}>=CD));
                        }
                    }
                }

                SECTION("Comparison of sub_match to c-style string")
                {

                    ssub_match::value_type cStringAB[3] = "AB";
                    ssub_match::value_type cStringCD[3] = "CD";
                    ssub_match::value_type cStringEF[3] = "EF";

                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((CD==cStringAB));
                            REQUIRE((CD==cStringCD));
                            REQUIRE_FALSE((CD==cStringEF));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((CD!=cStringAB));
                            REQUIRE_FALSE((CD!=cStringCD));
                            REQUIRE((CD!=cStringEF));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((CD<cStringAB));
                            REQUIRE_FALSE((CD<cStringCD));
                            REQUIRE((CD<cStringEF));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((CD<=cStringAB));
                            REQUIRE((CD<=cStringCD));
                            REQUIRE((CD<=cStringEF));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((CD>cStringAB));
                            REQUIRE_FALSE((CD>cStringCD));
                            REQUIRE_FALSE((CD>cStringEF));
                        }
                    }

                    WHEN( "Evaluating correctness of >= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((CD>=cStringAB));
                            REQUIRE((CD>=cStringCD));
                            REQUIRE_FALSE((CD>=cStringEF));
                        }
                    }
                }

                SECTION("Comparison of c-style string to sub_match")
                {

                    ssub_match::value_type cStringAB[3] = "AB";
                    ssub_match::value_type cStringCD[3] = "CD";
                    ssub_match::value_type cStringEF[3] = "EF";

                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((cStringAB == CD));
                            REQUIRE((cStringCD == CD));
                            REQUIRE_FALSE((cStringEF == CD));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((cStringAB != CD));
                            REQUIRE_FALSE((cStringCD != CD));
                            REQUIRE((cStringEF != CD));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((cStringAB < CD));
                            REQUIRE_FALSE((cStringCD < CD));
                            REQUIRE_FALSE((cStringEF < CD));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((cStringAB <= CD));
                            REQUIRE((cStringCD <= CD));
                            REQUIRE_FALSE((cStringEF <= CD));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((cStringAB > CD));
                            REQUIRE_FALSE((cStringCD > CD));
                            REQUIRE((cStringEF > CD));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((cStringAB >= CD));
                            REQUIRE((cStringCD >= CD));
                            REQUIRE((cStringEF >= CD));
                        }
                    }
                }

                SECTION("Comparison of sub_match to character")
                {

                    auto submatchY = ssub_match();
                    submatchY.first = Y;
                    submatchY.second = Y+1;
                    submatchY.matched = true;

                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((submatchY=='X'));
                            REQUIRE((submatchY=='Y'));
                            REQUIRE_FALSE((submatchY=='Z'));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((submatchY!='X'));
                            REQUIRE_FALSE((submatchY!='Y'));
                            REQUIRE((submatchY!='Z'));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((submatchY!='X'));
                            REQUIRE_FALSE((submatchY!='Y'));
                            REQUIRE((submatchY!='Z'));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((submatchY<'X'));
                            REQUIRE_FALSE((submatchY<'Y'));
                            REQUIRE((submatchY<'Z'));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE((submatchY<='X'));
                            REQUIRE((submatchY<='Y'));
                            REQUIRE((submatchY<='Z'));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((submatchY>'X'));
                            REQUIRE_FALSE((submatchY>'Y'));
                            REQUIRE_FALSE((submatchY>'Z'));
                        }
                    }

                    WHEN( "Evaluating correctness of >= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE((submatchY>='X'));
                            REQUIRE((submatchY>='Y'));
                            REQUIRE_FALSE((submatchY>='Z'));
                        }
                    }
                }
                
                SECTION("Comparison of character to sub_match")
                {
                    auto submatchY = ssub_match();
                    submatchY.first = Y;
                    submatchY.second = Y+1;
                    submatchY.matched = true;

                    WHEN( "Evaluating correctness of == operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE(('X'==submatchY));
                            REQUIRE(('Y'==submatchY));
                            REQUIRE_FALSE(('Z'==submatchY));
                        }
                    }

                    WHEN( "Evaluating correctness of != operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE(('X'!=submatchY));
                            REQUIRE_FALSE(('Y'!=submatchY));
                            REQUIRE(('Z'!=submatchY));
                        }
                    }

                    WHEN( "Evaluating correctness of < operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE(('X'<submatchY));
                            REQUIRE_FALSE(('Y'<submatchY));
                            REQUIRE_FALSE(('Z'<submatchY));
                        }
                    }

                    WHEN( "Evaluating correctness of <= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE(('X'<=submatchY));
                            REQUIRE(('Y'<=submatchY));
                            REQUIRE_FALSE(('Z'<=submatchY));
                        }
                    }

                    WHEN( "Evaluating correctness of > operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE(('X'>submatchY));
                            REQUIRE_FALSE(('Y'>submatchY));
                            REQUIRE(('Z'>submatchY));
                        }
                    }

                    WHEN( "Evaluating correctness of >= operator" ) 
                    {
                        THEN("The operator works as intended")
                        {
                            REQUIRE_FALSE(('X'>=submatchY));
                            REQUIRE(('Y'>=submatchY));
                            REQUIRE(('Z'>=submatchY));
                        }
                    }

                }
            }
        }
    }

    SECTION("Writing matched subsequence to stream")
    {
        GIVEN("The target sequence ABCDEF")
        {
            std::string TargetSequence("ABCDEF");
            ssub_match::iterator A = TargetSequence.begin() + 0;
            ssub_match::iterator B = TargetSequence.begin() + 1;
            ssub_match::iterator C = TargetSequence.begin() + 2;
            ssub_match::iterator D = TargetSequence.begin() + 3; 
            ssub_match::iterator E = TargetSequence.begin() + 4;
            ssub_match::iterator F = TargetSequence.begin() + 5;

            GIVEN("A submatch of 'ABCD'")
            {
                auto submatch = ssub_match();
                submatch.first = A;
                submatch.second = D+1;
                submatch.matched = true;

                GIVEN("A string stream")
                {
                    std::stringstream ss;

                    WHEN("The submatch is piped to the stream")
                    {
                        ss << submatch;

                        THEN( "The stream contains the data" ) 
                        {
                            REQUIRE(ss.str()=="ABCD");
                        }
                    }
                }
            }
        }
    }
}

} //namespace
} //namespace regex 