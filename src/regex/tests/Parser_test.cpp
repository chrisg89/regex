#include "catch.hpp"
#include "Parser.hpp"

#include <stdexcept>
#include <iostream>

namespace regex::parser {

namespace {

using Catch::Contains;

//TODO : test anchors. Rename /B to word boundary?

SCENARIO("Parse characters")
{
    SECTION("Simple character")
    {
        const std::string regex = "a";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character from unicode")
    {
        const std::string regex = "a";
        //TODO not yet implemented
    }

    SECTION("Any character .")
    {
        const std::string regex = ".";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\n")
    {
        const std::string regex = "\\n";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\f")
    {
        const std::string regex = "\\f";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\r")
    {
        const std::string regex = "\\r";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\t")
    {
        const std::string regex = "\\t";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\v")
    {
        const std::string regex = "\\v";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\a")
    {
        const std::string regex = "\\a";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character ^")
    {
        const std::string regex = "\\^";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character $")
    {
        const std::string regex = "\\$";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character *")
    {
        const std::string regex = "\\*";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character +")
    {
        const std::string regex = "\\+";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character ?")
    {
        const std::string regex = "\\?";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character .")
    {
        const std::string regex = "\\.";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character |")
    {
        const std::string regex = "\\|";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character (")
    {
        const std::string regex = "\\(";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character )")
    {
        const std::string regex = "\\)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character [")
    {
        const std::string regex = "\\[";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character ]")
    {
        const std::string regex = "\\]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }
}

SCENARIO("Parse shorthand character classes") 
{
    SECTION("Digit")
    {
        const std::string regex = R"(\d)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated digit character")
    {
        const std::string regex = R"(\D)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Word character")
    {
        const std::string regex = R"(\w)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated word character")
    {
        const std::string regex = R"(\W)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("White space character")
    {
        const std::string regex = R"(\s)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated white space character")
    {
        const std::string regex = R"(\S)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }
}

SCENARIO("Parse character classes") 
{

    SECTION("Empty Character class is allowed")
    {
        const std::string regex = "[]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Simple character class")
    {
        const std::string regex = "[hello]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated character class")
    {
        const std::string regex = "[^hello]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with shorthand")
    {
        const std::string regex = R"([\w\W\d\D\s\S])";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with escaped characters that have special meaning")
    {
        const std::string regex = R"([\n\f\r\t\v\a\\])";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with escaped meta characters")
    {
        const std::string regex = R"([\^\]\[\-])";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with characters that are considered meta characters outside the character class")
    {
        const std::string regex = "[$*+?.()]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with range (literal characters)")
    {
        const std::string regex = "[a-z]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with range (escaped characters)")
    {
        const std::string regex = "[\\^-\\^]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with range (mixed literal and escaped)")
    {
        const std::string regex = "[\\^-z]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with multiple ranges")
    {
        const std::string regex = "[a-zA-Z0-9]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with range containing a single character")
    {
        const std::string regex = "[a-a]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with range, literal character and escaped character")
    {
        const std::string regex = "[a-zXYZ\\a]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Throw exception when character range is out of order")
    {
        const std::string regex = "[z-a]";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Character range is out of order"));
    }

    SECTION("Throw exception when character range is out of order")
    {
        const std::string regex = "[a-]";
        //TODO test this
    }

    SECTION("Throw exception when character range contains shorthand character class")
    {
        const std::string regex = "[\\w-a]";
        //TODO test this
    }

    SECTION("Throw exception when the closing bracket is missing")
    {
        const std::string regex = "[123";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Character class missing closing bracket"));
    }
}

SCENARIO("Parse backreference") 
{
    SECTION("Backreference is not supported")
    {
        const std::string regex = R"(a\4)";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Backreferences are not supported"));
    }
}

SCENARIO("Parse group") 
{
    SECTION("Simple group")
    {
        const std::string regex = "(a)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a");
    }

    SECTION("Nested group")
    {
        const std::string regex = "((a))";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a");
    }

    SECTION("Twice nested group")
    {
        const std::string regex = "(((a)))";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a");
    }

    SECTION("Empty group is not supported")
    {
        const std::string regex = "()";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Empty group is not supported"));
    }

    SECTION("Non-capturing group is not supported")
    {
        const std::string regex = "(?a)";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Non-capturing groups are the default. Capturing groups not supported"));
    }

    SECTION("Throw when group is not complete due to missing closing parenthesis")
    {
        const std::string regex = "(a";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Incomplete group structure"));
    }

    SECTION("Throw when nested group is not complete due to missing closing parenthesis")
    {
        const std::string regex = "((a)";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Incomplete group structure"));
    }

    SECTION("Throw when there exists an unmatch left parenthesis")
    {
        const std::string regex = "abc)";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Unmatched parenthesis"));
    }
}

SCENARIO("Parse quantifiers") 
{
    SECTION("Lazy Modifier (kleene star *) is not supported")
    {
        const std::string regex = "a*?";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (kleene plus +) is not supported")
    {
        const std::string regex = "a+?";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (optional ?) is not supported")
    {
        const std::string regex = "a??";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (ranged quantifier) is not supported")
    {
        const std::string regex = "a{1,3}?";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Kleene star (*)")
    {
        const std::string regex = "a*";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Kleene star (*) operating on a group")
    {
        const std::string regex = "(a)*";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a*");
    }

    SECTION("Kleene star (*) operating on a character class")
    {
        const std::string regex = "[abc]*";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
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
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Kleene plus (+)")
    {
        const std::string regex = "a+";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Kleene plus (+) operating on a group")
    {
        const std::string regex = "(a)+";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a+");
    }

    SECTION("Kleene plus (+) operating on a character class")
    {
        const std::string regex = "[abc]+";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
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
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Optional (?)")
    {
        const std::string regex = "a?";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Optional (?) operating on a group")
    {
        const std::string regex = "(a)?";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a?");
    }

    SECTION("Optional (?) operating on a character class")
    {
        const std::string regex = "[abc]?";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Throw when token preceding optional is not quantifiable ")
    {
        auto regex = GENERATE
        (
            "?"
        );
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Ranged quantifier: lower bound only ")
    {
        const std::string regex = "a{100}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: lower bound only operating on a group")
    {
        const std::string regex = "(a){100}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a{100}");
    }

    SECTION("Ranged quantifier: lower bound only operating on a character class")
    {
        const std::string regex = "[abc]{100}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: upper bound is omitted ")
    {
        const std::string regex = "a{100,}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a group")
    {
        const std::string regex = "(a){100,}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a{100,}");
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a character class")
    {
        const std::string regex = "[abc]{100,}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: lower and upper bound ")
    {
        const std::string regex = "a{100,200}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a group")
    {
        const std::string regex = "(a){100,200}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "a{100,200}");
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a character class")
    {
        const std::string regex = "[abc]{100,200}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "[abc]{100,200}");
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
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("The preceding token is not quantifiable"));
    }

    SECTION("Ranged quantifier throws when lower bound is too large")
    {
        const std::string regex = "a{999999999999}";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lower bound on ranged quantifier too large"));
    }

    SECTION("Ranged quantifier throws when upper bound is too large")
    {
        const std::string regex = "a{1,999999999999}";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Upper bound on ranged quantifier too large"));
    }

    SECTION("Ranged quantifier throws when lower bound is greater than upper bound")
    {
        const std::string regex = "a{2,1}";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("The quantifier range is out of order"));
    }
}

SCENARIO("Parse concatenation")
{
    SECTION("Concatenation of two characters")
    {
        const std::string regex = "ab";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(ab)");
    }

    SECTION("Concatenation of two escaped meta characters")
    {
        const std::string regex = R"(\*\*)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == R"((\*\*))");
    }

    SECTION("Concatenation of two escaped characters with special meaning")
    {
        const std::string regex = R"(\n\r)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == R"((\n\r))");
    }

    SECTION("Concatenation of two characters from unicode")
    {
        const std::string regex = R"(\n\r)";
        //TODO
    }

    SECTION("Concatenation of two character classes")
    {
        const std::string regex = "[abc][123]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "([abc][123])");
    }

    SECTION("Concatenation of two groups")
    {
        const std::string regex = "(a)(b)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(ab)");
    }
}

SCENARIO("Parse alternation")
{
    SECTION("Alternation of two characters")
    {
        const std::string regex = "a|b";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(a|b)");
    }

    SECTION("Alternation of two escaped meta characters")
    {
        const std::string regex = R"(\*|\*)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == R"((\*|\*))");
    }

    SECTION("Alternation of two escaped characters with special meaning")
    {
        const std::string regex = R"(\n|\r)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == R"((\n|\r))");
    }

    SECTION("Alternation of two characters from unicode")
    {
        const std::string regex = R"(\n|\r)";
        //TODO
    }

    SECTION("Alternation of two character classes")
    {
        const std::string regex = "[abc]|[123]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "([abc]|[123])");
    }

    SECTION("Alternation of two groups")
    {
        const std::string regex = "(a)|(b)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(a|b)");
    }

    SECTION("Alternation and concatenation")
    {
        const std::string regex = "123|abc";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(((12)3)|((ab)c))");
    }

    SECTION("Alternation, concatenation and character class")
    {
        const std::string regex = "123|[abc]|z";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(((12)3)|([abc]|z))");
    }

    SECTION("Alternation, concatenation and group")
    {
        const std::string regex = "123|(abc|xy)|z";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == "(((12)3)|((((ab)c)|(xy))|z))");
    }

    SECTION("TODO what to name this???")
    {
        const std::string regex = "1|";
        //TODO
    }
}


}
}