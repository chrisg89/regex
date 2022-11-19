#include "catch.hpp"
#include "Parser.hpp"

#include <iostream>

namespace regex::parser {
namespace {

using Catch::Contains;

SCENARIO("Parse characters")
{
    SECTION("Simple character")
    {
        const std::string regex = "a";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061");
    }

    SECTION("Any character .")
    {
        const std::string regex = ".";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == regex);
    }

    SECTION("Unicode code point")
    {
        const std::string regex = "\\u000058";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000058");
    }

    SECTION("Escaped character \\n")
    {
        const std::string regex = "\\n";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00000a");
    }

    SECTION("Escaped character \\f")
    {
        const std::string regex = "\\f";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00000c");
    }

    SECTION("Escaped character \\r")
    {
        const std::string regex = "\\r";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00000d");
    }

    SECTION("Escaped character \\t")
    {
        const std::string regex = "\\t";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000009");
    }

    SECTION("Escaped character \\v")
    {
        const std::string regex = "\\v";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00000b");
    }

    SECTION("Escaped character \\a")
    {
        const std::string regex = "\\a";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000007");
    }

    SECTION("Escaped character \\")
    {
        const std::string regex = "\\\\";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00005c");
    }

    SECTION("Escaped meta character ^")
    {
        const std::string regex = "\\^";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00005e");
    }

    SECTION("Escaped meta character $")
    {
        const std::string regex = "\\$";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000024");
    }

    SECTION("Escaped meta character *")
    {
        const std::string regex = "\\*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00002a");
    }

    SECTION("Escaped meta character +")
    {
        const std::string regex = "\\+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00002b");
    }

    SECTION("Escaped meta character ?")
    {
        const std::string regex = "\\?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00003f");
    }

    SECTION("Escaped meta character .")
    {
        const std::string regex = "\\.";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00002e");
    }

    SECTION("Escaped meta character |")
    {
        const std::string regex = "\\|";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00007c");
    }

    SECTION("Escaped meta character (")
    {
        const std::string regex = "\\(";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000028");
    }

    SECTION("Escaped meta character )")
    {
        const std::string regex = "\\)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000029");
    }

    SECTION("Escaped meta character [")
    {
        const std::string regex = "\\[";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00005b");
    }

    SECTION("Escaped meta character ]")
    {
        const std::string regex = "\\]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u00005d");
    }
}

SCENARIO("Parse shorthand character classes") 
{
    SECTION("Digit")
    {
        const std::string regex = R"(\d)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000030-\\u000039]|)");
    }

    SECTION("Negated digit character")
    {
        const std::string regex = R"(\D)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u00002f]|([\\u00003a-\\u10ffff]|))");
    }

    SECTION("Word character")
    {
        const std::string regex = R"(\w)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000030-\\u000039]|([\\u000041-\\u00005a]|([\\u00005f-\\u00005f]|([\\u000061-\\u00007a]|))))");
    }

    SECTION("Negated word character")
    {
        const std::string regex = R"(\W)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u00002f]|([\\u00003a-\\u000040]|([\\u00005b-\\u00005e]|([\\u000060-\\u000060]|([\\u00007b-\\u10ffff]|)))))");
    }

    SECTION("White space character")
    {
        const std::string regex = R"(\s)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000009-\\u00000d]|([\\u000020-\\u000020]|))");
    }

    SECTION("Negated white space character")
    {
        const std::string regex = R"(\S)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u000008]|([\\u00000e-\\u00001f]|([\\u000021-\\u10ffff]|)))");
    }
}

SCENARIO("Parse character classes") 
{

    SECTION("Simple character class")
    {
        const std::string regex = "[abc]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|)))");
    }

    SECTION("Negated character class")
    {
        const std::string regex = "[^abc]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u000060]|([\\u000064-\\u10ffff]|))");
    }

    SECTION("Character class with shorthand \\w")
    {
        const std::string regex = R"([\w])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000030-\\u000039]|([\\u000041-\\u00005a]|([\\u00005f-\\u00005f]|([\\u000061-\\u00007a]|))))");
    }

    SECTION("Character class with shorthand \\W")
    {
        const std::string regex = R"([\W])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u00002f]|([\\u00003a-\\u000040]|([\\u00005b-\\u00005e]|([\\u000060-\\u000060]|([\\u00007b-\\u10ffff]|)))))");
    }

    SECTION("Character class with shorthand \\d")
    {
        const std::string regex = R"([\d])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000030-\\u000039]|)");
    }

    SECTION("Character class with shorthand \\D")
    {
        const std::string regex = R"([\D])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u00002f]|([\\u00003a-\\u10ffff]|))");
    }

    SECTION("Character class with shorthand \\s")
    {
        const std::string regex = R"([\s])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000009-\\u00000d]|([\\u000020-\\u000020]|))");
    }

    SECTION("Character class with shorthand \\S")
    {
        const std::string regex = R"([\S])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u000008]|([\\u00000e-\\u00001f]|([\\u000021-\\u10ffff]|)))");
    }

    SECTION("Character class with escaped characters that have special meaning")
    {
        const std::string regex = R"([\n\f\r\t\v\a\\])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00000a-\\u00000a]|([\\u00000c-\\u00000c]|([\\u00000d-\\u00000d]|([\\u000009-\\u000009]|([\\u00000b-\\u00000b]|([\\u000007-\\u000007]|([\\u00005c-\\u00005c]|)))))))");
    }

    SECTION("Character class with escaped meta characters")
    {
        const std::string regex = R"([\^\]\[\-])";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00005e-\\u00005e]|([\\u00005d-\\u00005d]|([\\u00005b-\\u00005b]|([\\u00002d-\\u00002d]|))))");
    }

    SECTION("Character class with characters that are considered meta characters outside the character class")
    {
        const std::string regex = "[$*+?.()]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000024-\\u000024]|([\\u00002a-\\u00002a]|([\\u00002b-\\u00002b]|([\\u00003f-\\u00003f]|([\\u00002e-\\u00002e]|([\\u000028-\\u000028]|([\\u000029-\\u000029]|)))))))");
    }

    SECTION("Character class with unicode code point")
    {
        const std::string regex = "[\\u000058]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000058-\\u000058]|)");
    }

    SECTION("Character class where hyphen is only character")
    {
        const std::string regex = "[-]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00002d-\\u00002d]|)");
    }

    SECTION("Character class where hyphen is first character")
    {
        const std::string regex = "[-a]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00002d-\\u00002d]|([\\u000061-\\u000061]|))");
    }

    SECTION("Character class where hyphen is final character")
    {
        const std::string regex = "[a-]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u00002d-\\u00002d]|))");
    }

    SECTION("Character class where hyphen is treated literally in presence of shorthand character class")
    {
        const std::string regex = "[\\d-\\D]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000000-\\u00002f]|([\\u00003a-\\u10ffff]|))");
    }

    SECTION("Character class where closing bracket is first character")
    {
        const std::string regex = "[[]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00005b-\\u00005b]|)");
    }

    SECTION("Character class with range (literal characters)")
    {
        const std::string regex = "[a-z]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u00007a]|)");
    }

    SECTION("Character class with range (escaped characters)")
    {
        const std::string regex = "[\\^-\\^]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00005e-\\u00005e]|)");
    }

    SECTION("Character class with range (mixed literal and escaped)")
    {
        const std::string regex = "[\\^-z]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u00005e-\\u00007a]|)");
    }

    SECTION("Character class with multiple ranges")
    {
        const std::string regex = "[a-zA-Z0-9]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u00007a]|([\\u000041-\\u00005a]|([\\u000030-\\u000039]|)))");
    }

    SECTION("Character class with range containing a single character")
    {
        const std::string regex = "[a-a]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|)");
    }

    SECTION("Character class with range, literal character and escaped character")
    {
        const std::string regex = "[a-zXYZ\\a]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u00007a]|([\\u000058-\\u000058]|([\\u000059-\\u000059]|([\\u00005a-\\u00005a]|([\\u000007-\\u000007]|)))))");
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
        const std::string regex = R"(a\4)";
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("Backreferences are not supported"));
    }
}

SCENARIO("Parse group") 
{
    SECTION("Simple group")
    {
        const std::string regex = "(a)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061");
    }

    SECTION("Nested group")
    {
        const std::string regex = "((a))";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061");
    }

    SECTION("Twice nested group")
    {
        const std::string regex = "(((a)))";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061");
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
        const std::string regex = "(?a)";
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
        CHECK(ast.print() == "\\u000061{0,}");
    }

    SECTION("Kleene star (*) operating on a group")
    {
        const std::string regex = "(a)*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{0,}");
    }

    SECTION("Kleene star (*) operating on a character class")
    {
        const std::string regex = "[abc]*";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|))){0,}");
    }

    SECTION("Throw when token preceding Kleene star is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "*",
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
        CHECK(ast.print() == "\\u000061{1,}");
    }

    SECTION("Kleene plus (+) operating on a group")
    {
        const std::string regex = "(a)+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{1,}");
    }

    SECTION("Kleene plus (+) operating on a character class")
    {
        const std::string regex = "[abc]+";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|))){1,}");
    }

    SECTION("Throw when token preceding Kleene plus is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "+",
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
        CHECK(ast.print() == "\\u000061{0,1}");
    }

    SECTION("Optional (?) operating on a group")
    {
        const std::string regex = "(a)?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{0,1}");
    }

    SECTION("Optional (?) operating on a character class")
    {
        const std::string regex = "[abc]?";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|))){0,1}");
    }

    SECTION("Throw when token preceding optional is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "?"
        );
        auto parser = Parser(regex);
        REQUIRE_THROWS_WITH(parser.parse(), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Ranged quantifier: lower bound only ")
    {
        const std::string regex = "a{100}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{100,100}");
    }

    SECTION("Ranged quantifier: lower bound only operating on a group")
    {
        const std::string regex = "(a){100}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{100,100}");
    }

    SECTION("Ranged quantifier: lower bound only operating on a character class")
    {
        const std::string regex = "[abc]{100}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|))){100,100}");
    }

    SECTION("Ranged quantifier: upper bound is omitted ")
    {
        const std::string regex = "a{100,}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{100,}");
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a group")
    {
        const std::string regex = "(a){100,}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{100,}");
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a character class")
    {
        const std::string regex = "[abc]{100,}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|))){100,}");
    }

    SECTION("Ranged quantifier: lower and upper bound ")
    {
        const std::string regex = "a{100,200}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{100,200}");
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a group")
    {
        const std::string regex = "(a){100,200}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "\\u000061{100,200}");
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a character class")
    {
        const std::string regex = "[abc]{100,200}";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|))){100,200}");
    }

    SECTION("Throw when token preceding ranged quantifier is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "{100}",
            "a?{100}",
            "a+{100}",
            "a*{100}",
            "a{1}{100}",
            "a{1,}{100}",
            "a{1,2}{100}",
            "{100,}",
            "a?{100,}",
            "a+{100,}",
            "a*{100,}",
            "a{1}{100,}",
            "a{1,}{100,}",
            "a{1,2}{100,}",
            "{100,200}",
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
        CHECK(ast.print() == "(\\u000061\\u000062)");
    }

    SECTION("Concatenation of two escaped meta characters")
    {
        const std::string regex = R"(\*\*)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u00002a\\u00002a)");
    }

    SECTION("Concatenation of two escaped characters with special meaning")
    {
        const std::string regex = R"(\n\r)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u00000a\\u00000d)");
    }

    SECTION("Concatenation of two characters from unicode")
    {
        const std::string regex = "\\u000058\\u000059";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u000058\\u000059)");
    }

    SECTION("Concatenation of two character classes")
    {
        const std::string regex = "[abc][123]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|)))([\\u000031-\\u000031]|([\\u000032-\\u000032]|([\\u000033-\\u000033]|))))");
    }

    SECTION("Concatenation of two groups")
    {
        const std::string regex = "(a)(b)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u000061\\u000062)");
    }
}

SCENARIO("Parse alternation")
{
    SECTION("Alternation of two characters")
    {
        const std::string regex = "a|b";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u000061|\\u000062)");
    }

    SECTION("Alternation with empty (epsilon) right hand side")
    {
        const std::string regex = "a|";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u000061|)");
    }

    SECTION("Alternation with empty (epsilon) left hand side")
    {
        const std::string regex = "|b";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(|\\u000062)");
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
        const std::string regex = R"(\*|\*)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u00002a|\\u00002a)");
    }

    SECTION("Alternation of two escaped characters with special meaning")
    {
        const std::string regex = R"(\n|\r)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u00000a|\\u00000d)");
    }

    SECTION("Alternation of two characters from unicode")
    {
        const std::string regex = "\\u000058|\\u000059";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u000058|\\u000059)");
    }

    SECTION("Alternation of two character classes")
    {
        const std::string regex = "[abc]|[123]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|)))|([\\u000031-\\u000031]|([\\u000032-\\u000032]|([\\u000033-\\u000033]|))))");
    }

    SECTION("Alternation of two groups")
    {
        const std::string regex = "(a)|(b)";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(\\u000061|\\u000062)");
    }

    SECTION("Alternation and concatenation")
    {
        const std::string regex = "123|abc";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(((\\u000031\\u000032)\\u000033)|((\\u000061\\u000062)\\u000063))");
    }

    SECTION("Alternation, concatenation and character class")
    {
        const std::string regex = "123|[abc]|z";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(((\\u000031\\u000032)\\u000033)|(([\\u000061-\\u000061]|([\\u000062-\\u000062]|([\\u000063-\\u000063]|)))|\\u00007a))");
    }

    SECTION("Alternation, concatenation and group")
    {
        const std::string regex = "123|(abc|xy)|z";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        CHECK(ast.print() == "(((\\u000031\\u000032)\\u000033)|((((\\u000061\\u000062)\\u000063)|(\\u000078\\u000079))|\\u00007a))");
    }
}


SCENARIO("?????") //TODO delete
{
    SECTION("Alternation of two characters")
    {
        const std::string regex = "[^a-c]";
        auto parser = Parser(regex);
        auto ast = parser.parse();
        //CHECK(ast.print() == "(a|b)");

        auto alphabet = ast.makeAlphabet();

        for (auto& x : alphabet)
        {
            std::cout << x.first << " - " << x.second << std::endl;
        }
    }

   
}

} // namespace 
} // namespace regex::parser