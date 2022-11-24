#include "catch.hpp"
#include "Parser.hpp"

#include <iostream>

namespace regex::parser {
namespace {

using Catch::Contains;

SCENARIO("Parse characters")
{
    SECTION("Ascii character")
    {
        const std::string regex = "a";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]");
    }

    SECTION("Cyrillic Capital Letter Nje")
    {
        const std::string regex = "Њ";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000040a-\\U0000040a]");
    }

    SECTION("Cyrillic Capital Letter Nje")
    {
        const std::string regex = "Њ";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000040a-\\U0000040a]");
    }

    SECTION("Armenian capital latter Ayb")
    {
        const std::string regex = "Ա";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000531-\\U00000531]");
    }

    SECTION("Arabic letter Beh")
    {
        const std::string regex = "ب";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000628-\\U00000628]");
    }
  
    SECTION("Telugu letter A")
    {
        const std::string regex = "అ";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000c05-\\U00000c05]");
    }  

    SECTION("Katakana letter Ga")
    {
        const std::string regex = "ガ";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U000030ac-\\U000030ac]");
    }
}

SCENARIO("Parse escaped characters with special meaning")
{
    SECTION("Escaped character \\n")
    {
        const std::string regex = "\\n";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000000a-\\U0000000a]");
    }

    SECTION("Escaped character \\f")
    {
        const std::string regex = "\\f";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000000c-\\U0000000c]");
    }

    SECTION("Escaped character \\r")
    {
        const std::string regex = "\\r";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000000d-\\U0000000d]");
    }

    SECTION("Escaped character \\t")
    {
        const std::string regex = "\\t";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000009-\\U00000009]");
    }

    SECTION("Escaped character \\v")
    {
        const std::string regex = "\\v";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000000b-\\U0000000b]");
    }

    SECTION("Escaped character \\a")
    {
        const std::string regex = "\\a";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000007-\\U00000007]");
    }

    SECTION("Escaped character \\")
    {
        const std::string regex = "\\\\";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000005c-\\U0000005c]");
    }
}

SCENARIO("Parse escaped meta characters")
{
    SECTION("Escaped meta character ^")
    {
        const std::string regex = "\\^";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000005e-\\U0000005e]");
    }

    SECTION("Escaped meta character $")
    {
        const std::string regex = "\\$";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000024-\\U00000024]");
    }

    SECTION("Escaped meta character *")
    {
        const std::string regex = "\\*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000002a-\\U0000002a]");
    }

    SECTION("Escaped meta character +")
    {
        const std::string regex = "\\+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000002b-\\U0000002b]");
    }

    SECTION("Escaped meta character ?")
    {
        const std::string regex = "\\?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000003f-\\U0000003f]");
    }

    SECTION("Escaped meta character .")
    {
        const std::string regex = "\\.";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000002e-\\U0000002e]");
    }

    SECTION("Escaped meta character |")
    {
        const std::string regex = "\\|";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000007c-\\U0000007c]");
    }

    SECTION("Escaped meta character (")
    {
        const std::string regex = "\\(";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000028-\\U00000028]");
    }

    SECTION("Escaped meta character )")
    {
        const std::string regex = "\\)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000029-\\U00000029]");
    }

    SECTION("Escaped meta character [")
    {
        const std::string regex = "\\[";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000005b-\\U0000005b]");
    }

    SECTION("Escaped meta character ]")
    {
        const std::string regex = "\\]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000005d-\\U0000005d]");
    }
}

SCENARIO("Parse character from 4 digit unicode code point")
{
    SECTION("Unicode code point decimal digits")
    {
        const std::string regex = "\\u1234";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00001234-\\U00001234]");
    }

    SECTION("Unicode code point lower case hex digits")
    {
        const std::string regex = "\\uabcd";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000abcd-\\U0000abcd]");
    }

    SECTION("Unicode code point upper case hex digits")
    {
        const std::string regex = "\\uABCD";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U0000abcd-\\U0000abcd]");
    }

    SECTION("Unicode code point mixed digits")
    {
        const std::string regex = "\\u1Ab3";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00001ab3-\\U00001ab3]");
    }

    SECTION("Throw on incomplete 4 digit unicode codepoint")
    {
        auto regex = GENERATE
        (
            "\\u",
            "\\u1",
            "\\u12",
            "\\u123",
            "\\u123x" // x is not a hexidecimal digit, hence, codepoint is incomplete
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The Unicode codepoint is incomplete"));
    }
}

SCENARIO("Parse character from 8 digit unicode code point")
{
    SECTION("Unicode code point decimal digits")
    {
        const std::string regex = "\\U00012345";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00012345-\\U00012345]");
    }

    SECTION("Unicode code point lower case hex digits")
    {
        const std::string regex = "\\U000abcde";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U000abcde-\\U000abcde]");
    }

    SECTION("Unicode code point upper case hex digits")
    {
        const std::string regex = "\\U000ABCDE";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U000abcde-\\U000abcde]");
    }

    SECTION("Unicode code point mixed digits")
    {
        const std::string regex = "\\U00012abC";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00012abc-\\U00012abc]");
    }

    SECTION("Throw on incomplete 8 digit unicode codepoint")
    {
        auto regex = GENERATE
        (
            "\\U",        
            "\\U1",
            "\\U12",
            "\\U123",
            "\\U1234",
            "\\U12345",
            "\\U123456",
            "\\U1234567",
            "\\U1234567x" // x is not a hexidecimal digit, hence, codepoint is incomplete
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The Unicode codepoint is incomplete"));
    }

    SECTION("Throw when unicode codepoint exceeds maximum value of 0x0010FFFF")
    {
        const std::string regex = "\\U00110000";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The Unicode codepoint invalid"));
    }
}

SCENARIO("Parse any (.) character")
{
    SECTION("Any character .")
    {
        const std::string regex = ".";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U00000009]|([\\U0000000b-\\U0010ffff]|))");
    }
}

SCENARIO("Parse shorthand character classes") 
{
    SECTION("Digit")
    {
        const std::string regex = "\\d";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000030-\\U00000039]|)");
    }

    SECTION("Negated digit character")
    {
        const std::string regex = "\\D";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U0000002f]|([\\U0000003a-\\U0010ffff]|))");
    }

    SECTION("Word character")
    {
        const std::string regex = "\\w";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000030-\\U00000039]|([\\U00000041-\\U0000005a]|([\\U0000005f-\\U0000005f]|([\\U00000061-\\U0000007a]|))))");
    }

    SECTION("Negated word character")
    {
        const std::string regex = "\\W";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U0000002f]|([\\U0000003a-\\U00000040]|([\\U0000005b-\\U0000005e]|([\\U00000060-\\U00000060]|([\\U0000007b-\\U0010ffff]|)))))");
    }

    SECTION("White space character")
    {
        const std::string regex = "\\s";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000009-\\U0000000d]|([\\U00000020-\\U00000020]|))");
    }

    SECTION("Negated white space character")
    {
        const std::string regex = "\\S";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U00000008]|([\\U0000000e-\\U0000001f]|([\\U00000021-\\U0010ffff]|)))");
    }
}

SCENARIO("Parse character classes") 
{

    SECTION("Simple character class")
    {
        const std::string regex = "[abc]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|)))");
    }

    SECTION("Negated character class")
    {
        const std::string regex = "[^abc]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U00000060]|([\\U00000064-\\U0010ffff]|))");
    }

    SECTION("Character class with shorthand \\w")
    {
        const std::string regex = "[\\w]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000030-\\U00000039]|([\\U00000041-\\U0000005a]|([\\U0000005f-\\U0000005f]|([\\U00000061-\\U0000007a]|))))");
    }

    SECTION("Character class with shorthand \\W")
    {
        const std::string regex = "[\\W]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U0000002f]|([\\U0000003a-\\U00000040]|([\\U0000005b-\\U0000005e]|([\\U00000060-\\U00000060]|([\\U0000007b-\\U0010ffff]|)))))");
    }

    SECTION("Character class with shorthand \\d")
    {
        const std::string regex = "[\\d]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000030-\\U00000039]|)");
    }

    SECTION("Character class with shorthand \\D")
    {
        const std::string regex = "[\\D]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U0000002f]|([\\U0000003a-\\U0010ffff]|))");
    }

    SECTION("Character class with shorthand \\s")
    {
        const std::string regex = "[\\s]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000009-\\U0000000d]|([\\U00000020-\\U00000020]|))");
    }

    SECTION("Character class with shorthand \\S")
    {
        const std::string regex = "[\\S]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U00000008]|([\\U0000000e-\\U0000001f]|([\\U00000021-\\U0010ffff]|)))");
    }

    SECTION("Character class with escaped characters that have special meaning")
    {
        const std::string regex = "[\\n\\f\\r\\t\\v\\a\\\\]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000000a-\\U0000000a]|([\\U0000000c-\\U0000000c]|([\\U0000000d-\\U0000000d]|([\\U00000009-\\U00000009]|([\\U0000000b-\\U0000000b]|([\\U00000007-\\U00000007]|([\\U0000005c-\\U0000005c]|)))))))");
    }

    SECTION("Character class with escaped meta characters")
    {
        const std::string regex = "[\\^\\]\\[\\-]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000005e-\\U0000005e]|([\\U0000005d-\\U0000005d]|([\\U0000005b-\\U0000005b]|([\\U0000002d-\\U0000002d]|))))");
    }

    SECTION("Character class with characters that are considered meta characters outside the character class")
    {
        const std::string regex = "[$*+?.()]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000024-\\U00000024]|([\\U0000002a-\\U0000002a]|([\\U0000002b-\\U0000002b]|([\\U0000003f-\\U0000003f]|([\\U0000002e-\\U0000002e]|([\\U00000028-\\U00000028]|([\\U00000029-\\U00000029]|)))))))");
    }

    SECTION("Character class with unicode code point (4 digits)")
    {
        const std::string regex = "[\\uFFFF]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000ffff-\\U0000ffff]|)");
    }

    SECTION("Character class with unicode code point (8 digits)")
    {
        const std::string regex = "[\\U0010FFFF]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0010ffff-\\U0010ffff]|)");
    }

    SECTION("Character class where hyphen is only character")
    {
        const std::string regex = "[-]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000002d-\\U0000002d]|)");
    }

    SECTION("Character class where hyphen is first character")
    {
        const std::string regex = "[-a]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000002d-\\U0000002d]|([\\U00000061-\\U00000061]|))");
    }

    SECTION("Character class where hyphen is final character")
    {
        const std::string regex = "[a-]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U0000002d-\\U0000002d]|))");
    }

    SECTION("Character class where hyphen is treated literally in presence of shorthand character class")
    {
        const std::string regex = "[\\d-\\D]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000000-\\U0000002f]|([\\U0000003a-\\U0010ffff]|))");
    }

    SECTION("Character class where closing bracket is first character")
    {
        const std::string regex = "[[]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000005b-\\U0000005b]|)");
    }

    SECTION("Character class with range (literal characters)")
    {
        const std::string regex = "[a-z]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U0000007a]|)");
    }

    SECTION("Character class with range (escaped characters)")
    {
        const std::string regex = "[\\^-\\^]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000005e-\\U0000005e]|)");
    }

    SECTION("Character class with range (mixed literal and escaped)")
    {
        const std::string regex = "[\\^-z]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000005e-\\U0000007a]|)");
    }

    SECTION("Character class with multiple ranges")
    {
        const std::string regex = "[a-zA-Z0-9]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U0000007a]|([\\U00000041-\\U0000005a]|([\\U00000030-\\U00000039]|)))");
    }

    SECTION("Character class with range containing a single character")
    {
        const std::string regex = "[a-a]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|)");
    }

    SECTION("Character class with range, literal character and escaped character")
    {
        const std::string regex = "[a-zXYZ\\a]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U0000007a]|([\\U00000058-\\U00000058]|([\\U00000059-\\U00000059]|([\\U0000005a-\\U0000005a]|([\\U00000007-\\U00000007]|)))))");
    }

    SECTION("Throw exception when character range is out of order")
    {
        const std::string regex = "[z-a]";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Character range is out of order"));
    }

    SECTION("Throw exception when the character class is empty")
    {
        const std::string regex = "[]";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Character class missing closing bracket"));
    }

    SECTION("Throw exception when the closing bracket is missing")
    {
        const std::string regex = "[123";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Character class missing closing bracket"));
    }
}

SCENARIO("Parse backreference") 
{
    SECTION("Backreference is not supported")
    {
        const std::string regex = "a\\4";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Backreferences are not supported"));
    }
}

SCENARIO("Parse Anchors") 
{
    SECTION("Start of string anchor (^) is not supported")
    {
        const std::string regex = "^abc";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Anchors are not supported"));
    }

    SECTION("End of string anchor ($) is not supported")
    {
        const std::string regex = "abc$";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Anchors are not supported"));
    }

    SECTION("Word boundry anchor (\\b) is not recognized")
    {
        const std::string regex = "\\b";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("This token has no special meaning and has thus been rendered erroneous"));
    }

    SECTION("Non-word boundry anchor (\\B) is not recognized")
    {
        const std::string regex = "\\B";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("This token has no special meaning and has thus been rendered erroneous"));
    }

    SECTION("Anchor given by \\A is not recognized")
    {
        const std::string regex = "\\A";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("This token has no special meaning and has thus been rendered erroneous"));
    }

    SECTION("Anchor given by \\Z is not recognized")
    {
        const std::string regex = "\\Z";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("This token has no special meaning and has thus been rendered erroneous"));
    }

    SECTION("Anchor given by \\z is not recognized")
    {
        const std::string regex = "\\z";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("This token has no special meaning and has thus been rendered erroneous"));
    }
}

SCENARIO("Parse group") 
{
    SECTION("Simple group")
    {
        const std::string regex = "(a)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]");
    }

    SECTION("Nested group")
    {
        const std::string regex = "((a))";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]");
    }

    SECTION("Twice nested group")
    {
        const std::string regex = "(((a)))";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]");
    }

    SECTION("Empty group")
    {
        const std::string regex = "()";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "");
    }

    SECTION("Non-capturing group is not supported")
    {
        const std::string regex = "(?:a)";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Non-capturing groups are the default. Capturing groups not supported"));
    }

    SECTION("Throw when group is not complete due to missing closing parenthesis")
    {
        const std::string regex = "(a";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Incomplete group structure"));
    }

    SECTION("Throw when nested group is not complete due to missing closing parenthesis")
    {
        const std::string regex = "((a)";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Incomplete group structure"));
    }

    SECTION("Throw when there exists an unmatch left parenthesis")
    {
        const std::string regex = "abc)";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Unmatched parenthesis"));
    }
}

SCENARIO("Parse quantifiers") 
{
    SECTION("Lazy Modifier (kleene star *) is not supported")
    {
        const std::string regex = "a*?";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (kleene plus +) is not supported")
    {
        const std::string regex = "a+?";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (optional ?) is not supported")
    {
        const std::string regex = "a??";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (ranged quantifier) is not supported")
    {
        const std::string regex = "a{1,3}?";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Lazy modifier is not supported"));
    }

    SECTION("Kleene star (*)")
    {
        const std::string regex = "a*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{0,}");
    }

    SECTION("Kleene star (*) operating on a group")
    {
        const std::string regex = "(a)*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{0,}");
    }

    SECTION("Kleene star (*) operating on a character class")
    {
        const std::string regex = "[abc]*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|))){0,}");
    }

    SECTION("Throw when token preceding Kleene star is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "*",
            "(*",
            "a?*",
            "a+*",
            "a**",
            "a{1}*",
            "a{1,}*",
            "a{1,2}*"
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Kleene plus (+)")
    {
        const std::string regex = "a+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{1,}");
    }

    SECTION("Kleene plus (+) operating on a group")
    {
        const std::string regex = "(a)+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{1,}");
    }

    SECTION("Kleene plus (+) operating on a character class")
    {
        const std::string regex = "[abc]+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|))){1,}");
    }

    SECTION("Throw when token preceding Kleene plus is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "+",
            "(+",
            "a?+",
            "a++",
            "a*+",
            "a{1}+",
            "a{1,}+",
            "a{1,2}+"
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Optional (?)")
    {
        const std::string regex = "a?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{0,1}");
    }

    SECTION("Optional (?) operating on a group")
    {
        const std::string regex = "(a)?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{0,1}");
    }

    SECTION("Optional (?) operating on a character class")
    {
        const std::string regex = "[abc]?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|))){0,1}");
    }

    SECTION("Throw when token preceding optional is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "?",
            "(?"
            //"a??",      /* This is lazy modifier and valid syntax*/
            //"a+?",      /* This is lazy modifier and valid syntax*/
            //"a*?",      /* This is lazy modifier and valid syntax*/
            //"a{1}?",    /* This is lazy modifier and valid syntax*/
            //"a{1,}?",   /* This is lazy modifier and valid syntax*/
            //"a{1,2}?"   /* This is lazy modifier and valid syntax*/
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Ranged quantifier: lower bound only ")
    {
        const std::string regex = "a{100}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{100,100}");
    }

    SECTION("Ranged quantifier: lower bound only operating on a group")
    {
        const std::string regex = "(a){100}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{100,100}");
    }

    SECTION("Ranged quantifier: lower bound only operating on a character class")
    {
        const std::string regex = "[abc]{100}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|))){100,100}");
    }

    SECTION("Ranged quantifier: upper bound is omitted ")
    {
        const std::string regex = "a{100,}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{100,}");
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a group")
    {
        const std::string regex = "(a){100,}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{100,}");
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a character class")
    {
        const std::string regex = "[abc]{100,}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|))){100,}");
    }

    SECTION("Ranged quantifier: lower and upper bound ")
    {
        const std::string regex = "a{100,200}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{100,200}");
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a group")
    {
        const std::string regex = "(a){100,200}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "[\\U00000061-\\U00000061]{100,200}");
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a character class")
    {
        const std::string regex = "[abc]{100,200}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|))){100,200}");
    }

    SECTION("Throw when token preceding ranged quantifier is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "{100}",
            "({100}",
            "a?{100}",
            "a+{100}",
            "a*{100}",
            "a{1}{100}",
            "a{1,}{100}",
            "a{1,2}{100}",
            "{100,}",
            "({100,}",
            "a?{100,}",
            "a+{100,}",
            "a*{100,}",
            "a{1}{100,}",
            "a{1,}{100,}",
            "a{1,2}{100,}",
            "{100,200}",
            "({100,200}",
            "a?{100,200}",
            "a+{100,200}",
            "a*{100,200}",
            "a{1}{100,200}",
            "a{1,}{100,200}",
            "a{1,2}{100,200}"
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Ranged quantifier throws when lower bound is too large")
    {
        const std::string regex = "a{999999999999}";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Lower bound on ranged quantifier too large"));
    }

    SECTION("Ranged quantifier throws when upper bound is too large")
    {
        const std::string regex = "a{1,999999999999}";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Upper bound on ranged quantifier too large"));
    }

    SECTION("Ranged quantifier throws when lower bound is greater than upper bound")
    {
        const std::string regex = "a{2,1}";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The quantifier range is out of order"));
    }
}

SCENARIO("Parse concatenation")
{
    SECTION("Concatenation of two characters")
    {
        const std::string regex = "ab";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061][\\U00000062-\\U00000062])");
    }

    SECTION("Concatenation of two escaped meta characters")
    {
        const std::string regex = "\\*\\*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000002a-\\U0000002a][\\U0000002a-\\U0000002a])");
    }

    SECTION("Concatenation of two escaped characters with special meaning")
    {
        const std::string regex = "\\n\\r";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000000a-\\U0000000a][\\U0000000d-\\U0000000d])");
    }

    SECTION("Concatenation of two characters from unicode (4 digits)")
    {
        const std::string regex = "\\u1111\\u2222";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00001111-\\U00001111][\\U00002222-\\U00002222])");
    }

    SECTION("Concatenation of two characters from unicode (8 digits)")
    {
        const std::string regex = "\\U0010FFFF\\U00000001";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0010ffff-\\U0010ffff][\\U00000001-\\U00000001])");
    }

    SECTION("Concatenation of two character classes")
    {
        const std::string regex = "[abc][123]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|)))([\\U00000031-\\U00000031]|([\\U00000032-\\U00000032]|([\\U00000033-\\U00000033]|))))");
    }

    SECTION("Concatenation of two groups")
    {
        const std::string regex = "(a)(b)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061][\\U00000062-\\U00000062])");
    }
}

SCENARIO("Parse alternation")
{
    SECTION("Alternation of two characters")
    {
        const std::string regex = "a|b";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|[\\U00000062-\\U00000062])");
    }

    SECTION("Alternation with empty (epsilon) right hand side")
    {
        const std::string regex = "a|";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|)");
    }

    SECTION("Alternation with empty (epsilon) left hand side")
    {
        const std::string regex = "|b";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(|[\\U00000062-\\U00000062])");
    }

    SECTION("Alternation with empty (epsilon) left and right hand side")
    {
        const std::string regex = "|";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(|)");
    }

    SECTION("Alternation of two escaped meta characters")
    {
        const std::string regex = "\\*|\\*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000002a-\\U0000002a]|[\\U0000002a-\\U0000002a])");
    }

    SECTION("Alternation of two escaped characters with special meaning")
    {
        const std::string regex = "\\n|\\r";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0000000a-\\U0000000a]|[\\U0000000d-\\U0000000d])");
    }

    SECTION("Alternation of two characters from unicode (4 digits)")
    {
        const std::string regex = "\\u3333|\\u4444";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00003333-\\U00003333]|[\\U00004444-\\U00004444])");
    }

    SECTION("Alternation of two characters from unicode (8 digits)")
    {
        const std::string regex = "\\U0010FFFF|\\U00001234";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U0010ffff-\\U0010ffff]|[\\U00001234-\\U00001234])");
    }

    SECTION("Alternation of two character classes")
    {
        const std::string regex = "[abc]|[123]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|)))|([\\U00000031-\\U00000031]|([\\U00000032-\\U00000032]|([\\U00000033-\\U00000033]|))))");
    }

    SECTION("Alternation of two groups")
    {
        const std::string regex = "(a)|(b)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\U00000061-\\U00000061]|[\\U00000062-\\U00000062])");
    }

    SECTION("Alternation and concatenation")
    {
        const std::string regex = "123|abc";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "((([\\U00000031-\\U00000031][\\U00000032-\\U00000032])[\\U00000033-\\U00000033])|(([\\U00000061-\\U00000061][\\U00000062-\\U00000062])[\\U00000063-\\U00000063]))");
    }

    SECTION("Alternation, concatenation and character class")
    {
        const std::string regex = "123|[abc]|z";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "((([\\U00000031-\\U00000031][\\U00000032-\\U00000032])[\\U00000033-\\U00000033])|(([\\U00000061-\\U00000061]|([\\U00000062-\\U00000062]|([\\U00000063-\\U00000063]|)))|[\\U0000007a-\\U0000007a]))");
    }

    SECTION("Alternation, concatenation and group")
    {
        const std::string regex = "123|(abc|xy)|z";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "((([\\U00000031-\\U00000031][\\U00000032-\\U00000032])[\\U00000033-\\U00000033])|(((([\\U00000061-\\U00000061][\\U00000062-\\U00000062])[\\U00000063-\\U00000063])|([\\U00000078-\\U00000078][\\U00000079-\\U00000079]))|[\\U0000007a-\\U0000007a]))");
    }
}

} // namespace 
} // namespace regex::parser