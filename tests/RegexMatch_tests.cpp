#include <catch2/catch.hpp>
#include <regex/Regex.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace regex
{

namespace
{

SCENARIO("Match empty regex")
{
    SECTION("Empty Regex")
    {
        auto regex = Regex("");

        // Positive test case(s)
        REQUIRE(regex.match(""));

        // Negative test case(s)
        REQUIRE(!regex.match("a"));
    }
}

SCENARIO("Match character")
{
    SECTION("Ascii character")
    {
        auto regex = Regex("a");

        // Positive test case(s)
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Cyrillic Capital Letter Nje")
    {
        auto regex = Regex("Њ");

        // Positive test case(s)
        REQUIRE(regex.match("Њ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Armenian capital latter Ayb")
    {
        auto regex = Regex("Ա");

        // Positive test case(s)
        REQUIRE(regex.match("Ա"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Arabic letter Beh")
    {
        auto regex = Regex("ب");

        // Positive test case(s)
        REQUIRE(regex.match("ب"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Telugu letter A")
    {
        auto regex = Regex("అ");

        // Positive test case(s)
        REQUIRE(regex.match("అ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Katakana letter Ga")
    {
        auto regex = Regex("ガ");

        // Positive test case(s)
        REQUIRE(regex.match("ガ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match escaped characters with special meaning")
{
    SECTION("Escaped character \\n")
    {
        auto regex = Regex("\\n");

        // Positive test case(s)
        REQUIRE(regex.match("\n"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped character \\f")
    {
        auto regex = Regex("\\f");

        // Positive test case(s)
        REQUIRE(regex.match("\f"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped character \\r")
    {
        auto regex = Regex("\\r");

        // Positive test case(s)
        REQUIRE(regex.match("\r"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped character \\t")
    {
        auto regex = Regex("\\t");

        // Positive test case(s)
        REQUIRE(regex.match("\t"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped character \\v")
    {
        auto regex = Regex("\\v");

        // Positive test case(s)
        REQUIRE(regex.match("\v"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped character \\a")
    {
        auto regex = Regex("\\a");

        // Positive test case(s)
        REQUIRE(regex.match("\a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped character \\")
    {
        auto regex = Regex("\\\\");

        // Positive test case(s)
        REQUIRE(regex.match("\\"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match escaped meta characters")
{
    SECTION("Escaped meta character ^")
    {
        auto regex = Regex("\\^");

        // Positive test case(s)
        REQUIRE(regex.match("^"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character $")
    {
        auto regex = Regex("\\$");

        // Positive test case(s)
        REQUIRE(regex.match("$"));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character *")
    {
        auto regex = Regex("\\*");

        // Positive test case(s)
        REQUIRE(regex.match("*"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character +")
    {
        auto regex = Regex("\\+");

        // Positive test case(s)
        REQUIRE(regex.match("+"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character ?")
    {
        auto regex = Regex("\\?");

        // Positive test case(s)
        REQUIRE(regex.match("?"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character .")
    {
        auto regex = Regex("\\.");

        // Positive test case(s)
        REQUIRE(regex.match("."));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character |")
    {
        auto regex = Regex("\\|");

        // Positive test case(s)
        REQUIRE(regex.match("|"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character (")
    {
        auto regex = Regex("\\(");

        // Positive test case(s)
        REQUIRE(regex.match("("));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character )")
    {
        auto regex = Regex("\\)");

        // Positive test case(s)
        REQUIRE(regex.match(")"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character [")
    {
        auto regex = Regex("\\[");

        // Positive test case(s)
        REQUIRE(regex.match("["));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Escaped meta character ]")
    {
        auto regex = Regex("\\]");

        // Positive test case(s)
        REQUIRE(regex.match("]"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match character from 4 digit unicode code point")
{
    SECTION("Unicode code point decimal digits")
    {
        auto regex = Regex("\\u1234");

        // Positive test case(s)
        REQUIRE(regex.match("ሴ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Unicode code point lower case hex digits")
    {
        auto regex = Regex("\\ufeda");

        // Positive test case(s)
        REQUIRE(regex.match("ﻚ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Unicode code point upper case hex digits")
    {
        auto regex = Regex("\\uFEDA");

        // Positive test case(s)
        REQUIRE(regex.match("ﻚ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Unicode code point mixed digits")
    {
        auto regex = Regex("\\u1F3E");

        // Positive test case(s)
        REQUIRE(regex.match("Ἶ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match character from 8 digit unicode code point")
{
    SECTION("Unicode code point decimal digits")
    {
        auto regex = Regex("\\U00001234");

        // Positive test case(s)
        REQUIRE(regex.match("ሴ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Unicode code point lower case hex digits")
    {
        auto regex = Regex("\\U0000feda");

        // Positive test case(s)
        REQUIRE(regex.match("ﻚ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Unicode code point upper case hex digits")
    {
        auto regex = Regex("\\U0000FEDA");

        // Positive test case(s)
        REQUIRE(regex.match("ﻚ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Unicode code point mixed digits")
    {
        auto regex = Regex("\\U00001F3E");

        // Positive test case(s)
        REQUIRE(regex.match("Ἶ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match any (.) character")
{
    SECTION("Any character .")
    {
        auto regex = Regex(".");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("ﻚ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("\n"));
    }
}

SCENARIO("Match 'digit' shorthand character class")
{
    SECTION("digits")
    {
        auto regex = Regex("\\d");

        // Positive test case(s)
        REQUIRE(regex.match("0"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(regex.match("4"));
        REQUIRE(regex.match("5"));
        REQUIRE(regex.match("6"));
        REQUIRE(regex.match("7"));
        REQUIRE(regex.match("8"));
        REQUIRE(regex.match("9"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match 'non-digit' shorthand character class")
{
    SECTION("non-digits")
    {
        auto regex = Regex("\\D");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("0"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("2"));
        REQUIRE(!regex.match("3"));
        REQUIRE(!regex.match("4"));
        REQUIRE(!regex.match("5"));
        REQUIRE(!regex.match("6"));
        REQUIRE(!regex.match("7"));
        REQUIRE(!regex.match("8"));
        REQUIRE(!regex.match("9"));
    }
}

SCENARIO("Match 'word' shorthand character class")
{
    SECTION("word character")
    {
        auto regex = Regex("\\w");

        // Positive test case(s)
        REQUIRE(regex.match("0"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(regex.match("4"));
        REQUIRE(regex.match("5"));
        REQUIRE(regex.match("6"));
        REQUIRE(regex.match("7"));
        REQUIRE(regex.match("8"));
        REQUIRE(regex.match("9"));
        REQUIRE(regex.match("A"));
        REQUIRE(regex.match("B"));
        REQUIRE(regex.match("C"));
        REQUIRE(regex.match("D"));
        REQUIRE(regex.match("E"));
        REQUIRE(regex.match("F"));
        REQUIRE(regex.match("G"));
        REQUIRE(regex.match("H"));
        REQUIRE(regex.match("I"));
        REQUIRE(regex.match("J"));
        REQUIRE(regex.match("K"));
        REQUIRE(regex.match("L"));
        REQUIRE(regex.match("M"));
        REQUIRE(regex.match("N"));
        REQUIRE(regex.match("O"));
        REQUIRE(regex.match("P"));
        REQUIRE(regex.match("Q"));
        REQUIRE(regex.match("R"));
        REQUIRE(regex.match("S"));
        REQUIRE(regex.match("T"));
        REQUIRE(regex.match("U"));
        REQUIRE(regex.match("V"));
        REQUIRE(regex.match("W"));
        REQUIRE(regex.match("X"));
        REQUIRE(regex.match("Y"));
        REQUIRE(regex.match("Z"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("d"));
        REQUIRE(regex.match("e"));
        REQUIRE(regex.match("f"));
        REQUIRE(regex.match("g"));
        REQUIRE(regex.match("h"));
        REQUIRE(regex.match("i"));
        REQUIRE(regex.match("j"));
        REQUIRE(regex.match("k"));
        REQUIRE(regex.match("l"));
        REQUIRE(regex.match("m"));
        REQUIRE(regex.match("n"));
        REQUIRE(regex.match("o"));
        REQUIRE(regex.match("p"));
        REQUIRE(regex.match("q"));
        REQUIRE(regex.match("r"));
        REQUIRE(regex.match("s"));
        REQUIRE(regex.match("t"));
        REQUIRE(regex.match("u"));
        REQUIRE(regex.match("v"));
        REQUIRE(regex.match("w"));
        REQUIRE(regex.match("x"));
        REQUIRE(regex.match("y"));
        REQUIRE(regex.match("z"));
        REQUIRE(regex.match("_"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("?"));
        REQUIRE(!regex.match("."));
        REQUIRE(!regex.match("&"));
    }
}

SCENARIO("Match 'non-word' shorthand character class")
{
    SECTION("non-word character")
    {
        auto regex = Regex("\\W");

        // Positive test case(s)
        REQUIRE(regex.match("?"));
        REQUIRE(regex.match("."));
        REQUIRE(regex.match("&"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("0"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("2"));
        REQUIRE(!regex.match("3"));
        REQUIRE(!regex.match("4"));
        REQUIRE(!regex.match("5"));
        REQUIRE(!regex.match("6"));
        REQUIRE(!regex.match("7"));
        REQUIRE(!regex.match("8"));
        REQUIRE(!regex.match("9"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("B"));
        REQUIRE(!regex.match("C"));
        REQUIRE(!regex.match("D"));
        REQUIRE(!regex.match("E"));
        REQUIRE(!regex.match("F"));
        REQUIRE(!regex.match("G"));
        REQUIRE(!regex.match("H"));
        REQUIRE(!regex.match("I"));
        REQUIRE(!regex.match("J"));
        REQUIRE(!regex.match("K"));
        REQUIRE(!regex.match("L"));
        REQUIRE(!regex.match("M"));
        REQUIRE(!regex.match("N"));
        REQUIRE(!regex.match("O"));
        REQUIRE(!regex.match("P"));
        REQUIRE(!regex.match("Q"));
        REQUIRE(!regex.match("R"));
        REQUIRE(!regex.match("S"));
        REQUIRE(!regex.match("T"));
        REQUIRE(!regex.match("U"));
        REQUIRE(!regex.match("V"));
        REQUIRE(!regex.match("W"));
        REQUIRE(!regex.match("X"));
        REQUIRE(!regex.match("Y"));
        REQUIRE(!regex.match("Z"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
        REQUIRE(!regex.match("d"));
        REQUIRE(!regex.match("e"));
        REQUIRE(!regex.match("f"));
        REQUIRE(!regex.match("g"));
        REQUIRE(!regex.match("h"));
        REQUIRE(!regex.match("i"));
        REQUIRE(!regex.match("j"));
        REQUIRE(!regex.match("k"));
        REQUIRE(!regex.match("l"));
        REQUIRE(!regex.match("m"));
        REQUIRE(!regex.match("n"));
        REQUIRE(!regex.match("o"));
        REQUIRE(!regex.match("p"));
        REQUIRE(!regex.match("q"));
        REQUIRE(!regex.match("r"));
        REQUIRE(!regex.match("s"));
        REQUIRE(!regex.match("t"));
        REQUIRE(!regex.match("u"));
        REQUIRE(!regex.match("v"));
        REQUIRE(!regex.match("w"));
        REQUIRE(!regex.match("x"));
        REQUIRE(!regex.match("y"));
        REQUIRE(!regex.match("z"));
        REQUIRE(!regex.match("_"));
    }
}

SCENARIO("Match 'whitespace' shorthand character class")
{
    SECTION("whitespace")
    {
        auto regex = Regex("\\s");

        // Positive test case(s)
        REQUIRE(regex.match("\t"));
        REQUIRE(regex.match("\r"));
        REQUIRE(regex.match("\n"));
        REQUIRE(regex.match("\f"));
        REQUIRE(regex.match("\v"));
        REQUIRE(regex.match(" "));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match 'non-whitespace' shorthand character class")
{
    SECTION("Non-whitespace")
    {
        auto regex = Regex("\\S");

        // Positive test case(s)
        REQUIRE(regex.match("A"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("\t"));
        REQUIRE(!regex.match("\r"));
        REQUIRE(!regex.match("\n"));
        REQUIRE(!regex.match("\f"));
        REQUIRE(!regex.match("\v"));
        REQUIRE(!regex.match(" "));
    }
}

SCENARIO("Match character class")
{
    SECTION("Simple character class")
    {
        auto regex = Regex("[abc]");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Negated character class")
    {
        auto regex = Regex("[^abc]");

        // Positive test case(s)
        REQUIRE(regex.match("A"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("!"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
    }

    SECTION("Character class with shorthand \\w")
    {
        auto regex = Regex("\\w");

        // Positive test case(s)
        REQUIRE(regex.match("0"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(regex.match("4"));
        REQUIRE(regex.match("5"));
        REQUIRE(regex.match("6"));
        REQUIRE(regex.match("7"));
        REQUIRE(regex.match("8"));
        REQUIRE(regex.match("9"));
        REQUIRE(regex.match("A"));
        REQUIRE(regex.match("B"));
        REQUIRE(regex.match("C"));
        REQUIRE(regex.match("D"));
        REQUIRE(regex.match("E"));
        REQUIRE(regex.match("F"));
        REQUIRE(regex.match("G"));
        REQUIRE(regex.match("H"));
        REQUIRE(regex.match("I"));
        REQUIRE(regex.match("J"));
        REQUIRE(regex.match("K"));
        REQUIRE(regex.match("L"));
        REQUIRE(regex.match("M"));
        REQUIRE(regex.match("N"));
        REQUIRE(regex.match("O"));
        REQUIRE(regex.match("P"));
        REQUIRE(regex.match("Q"));
        REQUIRE(regex.match("R"));
        REQUIRE(regex.match("S"));
        REQUIRE(regex.match("T"));
        REQUIRE(regex.match("U"));
        REQUIRE(regex.match("V"));
        REQUIRE(regex.match("W"));
        REQUIRE(regex.match("X"));
        REQUIRE(regex.match("Y"));
        REQUIRE(regex.match("Z"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("d"));
        REQUIRE(regex.match("e"));
        REQUIRE(regex.match("f"));
        REQUIRE(regex.match("g"));
        REQUIRE(regex.match("h"));
        REQUIRE(regex.match("i"));
        REQUIRE(regex.match("j"));
        REQUIRE(regex.match("k"));
        REQUIRE(regex.match("l"));
        REQUIRE(regex.match("m"));
        REQUIRE(regex.match("n"));
        REQUIRE(regex.match("o"));
        REQUIRE(regex.match("p"));
        REQUIRE(regex.match("q"));
        REQUIRE(regex.match("r"));
        REQUIRE(regex.match("s"));
        REQUIRE(regex.match("t"));
        REQUIRE(regex.match("u"));
        REQUIRE(regex.match("v"));
        REQUIRE(regex.match("w"));
        REQUIRE(regex.match("x"));
        REQUIRE(regex.match("y"));
        REQUIRE(regex.match("z"));
        REQUIRE(regex.match("_"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("?"));
        REQUIRE(!regex.match("."));
        REQUIRE(!regex.match("&"));
    }

    SECTION("Character class with shorthand \\W")
    {
        auto regex = Regex("\\W");

        // Positive test case(s)
        REQUIRE(regex.match("?"));
        REQUIRE(regex.match("."));
        REQUIRE(regex.match("&"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("0"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("2"));
        REQUIRE(!regex.match("3"));
        REQUIRE(!regex.match("4"));
        REQUIRE(!regex.match("5"));
        REQUIRE(!regex.match("6"));
        REQUIRE(!regex.match("7"));
        REQUIRE(!regex.match("8"));
        REQUIRE(!regex.match("9"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("B"));
        REQUIRE(!regex.match("C"));
        REQUIRE(!regex.match("D"));
        REQUIRE(!regex.match("E"));
        REQUIRE(!regex.match("F"));
        REQUIRE(!regex.match("G"));
        REQUIRE(!regex.match("H"));
        REQUIRE(!regex.match("I"));
        REQUIRE(!regex.match("J"));
        REQUIRE(!regex.match("K"));
        REQUIRE(!regex.match("L"));
        REQUIRE(!regex.match("M"));
        REQUIRE(!regex.match("N"));
        REQUIRE(!regex.match("O"));
        REQUIRE(!regex.match("P"));
        REQUIRE(!regex.match("Q"));
        REQUIRE(!regex.match("R"));
        REQUIRE(!regex.match("S"));
        REQUIRE(!regex.match("T"));
        REQUIRE(!regex.match("U"));
        REQUIRE(!regex.match("V"));
        REQUIRE(!regex.match("W"));
        REQUIRE(!regex.match("X"));
        REQUIRE(!regex.match("Y"));
        REQUIRE(!regex.match("Z"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("c"));
        REQUIRE(!regex.match("d"));
        REQUIRE(!regex.match("e"));
        REQUIRE(!regex.match("f"));
        REQUIRE(!regex.match("g"));
        REQUIRE(!regex.match("h"));
        REQUIRE(!regex.match("i"));
        REQUIRE(!regex.match("j"));
        REQUIRE(!regex.match("k"));
        REQUIRE(!regex.match("l"));
        REQUIRE(!regex.match("m"));
        REQUIRE(!regex.match("n"));
        REQUIRE(!regex.match("o"));
        REQUIRE(!regex.match("p"));
        REQUIRE(!regex.match("q"));
        REQUIRE(!regex.match("r"));
        REQUIRE(!regex.match("s"));
        REQUIRE(!regex.match("t"));
        REQUIRE(!regex.match("u"));
        REQUIRE(!regex.match("v"));
        REQUIRE(!regex.match("w"));
        REQUIRE(!regex.match("x"));
        REQUIRE(!regex.match("y"));
        REQUIRE(!regex.match("z"));
        REQUIRE(!regex.match("_"));
    }

    SECTION("Character class with shorthand \\d")
    {
        auto regex = Regex("\\d");

        // Positive test case(s)
        REQUIRE(regex.match("0"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(regex.match("4"));
        REQUIRE(regex.match("5"));
        REQUIRE(regex.match("6"));
        REQUIRE(regex.match("7"));
        REQUIRE(regex.match("8"));
        REQUIRE(regex.match("9"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class with shorthand \\D")
    {
        auto regex = Regex("\\D");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("0"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("2"));
        REQUIRE(!regex.match("3"));
        REQUIRE(!regex.match("4"));
        REQUIRE(!regex.match("5"));
        REQUIRE(!regex.match("6"));
        REQUIRE(!regex.match("7"));
        REQUIRE(!regex.match("8"));
        REQUIRE(!regex.match("9"));
    }

    SECTION("Character class with shorthand \\s")
    {
        auto regex = Regex("\\s");

        // Positive test case(s)
        REQUIRE(regex.match("\t"));
        REQUIRE(regex.match("\r"));
        REQUIRE(regex.match("\n"));
        REQUIRE(regex.match("\f"));
        REQUIRE(regex.match("\v"));
        REQUIRE(regex.match(" "));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class with shorthand \\S")
    {
        auto regex = Regex("\\S");

        // Positive test case(s)
        REQUIRE(regex.match("A"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("\t"));
        REQUIRE(!regex.match("\r"));
        REQUIRE(!regex.match("\n"));
        REQUIRE(!regex.match("\f"));
        REQUIRE(!regex.match("\v"));
        REQUIRE(!regex.match(" "));
    }

    SECTION("Empty character class due to negation of \\w and \\W")
    {
        auto regex = Regex("[^\\w\\W]");

        // Positive test case(s)
        // None - no matches

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Empty character class due to negation of \\d and \\D")
    {
        auto regex = Regex("[^\\d\\D]");

        // Positive test case(s)
        // None - no matches

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Empty character class due to negation of \\s and \\S")
    {
        auto regex = Regex("[^\\s\\S]");

        // Positive test case(s)
        // None - no matches

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Character class with escaped characters that have special meaning")
    {
        auto regex = Regex("[\\n\\f\\r\\t\\v\\a\\\\]");

        // Positive test case(s)
        REQUIRE(regex.match("\n"));
        REQUIRE(regex.match("\f"));
        REQUIRE(regex.match("\r"));
        REQUIRE(regex.match("\t"));
        REQUIRE(regex.match("\v"));
        REQUIRE(regex.match("\a"));
        REQUIRE(regex.match("\\"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Character class with escaped meta characters")
    {
        auto regex = Regex("[\\^\\]\\[\\-]");

        // Positive test case(s)
        REQUIRE(regex.match("^"));
        REQUIRE(regex.match("]"));
        REQUIRE(regex.match("["));
        REQUIRE(regex.match("-"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Character class with characters that are considered meta "
            "characters outside the character class")
    {
        auto regex = Regex("[$*+?.()]");

        // Positive test case(s)
        REQUIRE(regex.match("$"));
        REQUIRE(regex.match("*"));
        REQUIRE(regex.match("+"));
        REQUIRE(regex.match("?"));
        REQUIRE(regex.match("."));
        REQUIRE(regex.match("("));
        REQUIRE(regex.match(")"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Character class with unicode code point (4 digits)")
    {
        auto regex = Regex("\\u1234");

        // Positive test case(s)
        REQUIRE(regex.match("ሴ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class with unicode code point (8 digits)")
    {
        auto regex = Regex("\\U00001234");

        // Positive test case(s)
        REQUIRE(regex.match("ሴ"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class where hyphen is only character")
    {
        auto regex = Regex("[-]");

        // Positive test case(s)
        REQUIRE(regex.match("-"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class where hyphen is first character")
    {
        auto regex = Regex("[-a]");

        // Positive test case(s)
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Negated character class where hyphen is first character")
    {
        auto regex = Regex("[^-a]");

        // Positive test case(s)
        REQUIRE(regex.match("A"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("!"));

        // Negative test case(s)
        REQUIRE(!regex.match("-"));
        REQUIRE(!regex.match("a"));
    }

    SECTION("Character class where hyphen is final character")
    {
        auto regex = Regex("[a-]");

        // Positive test case(s)
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class where hyphen is treated literally because it is "
            "surrounded by shorthand character classes")
    {
        auto regex = Regex("[\\d-\\s]");

        // Positive test case(s)
        REQUIRE(regex.match("-"));
        REQUIRE(regex.match("\n"));
        REQUIRE(regex.match("1"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class where closing bracket is first character")
    {
        auto regex = Regex("[[]");

        // Positive test case(s)
        REQUIRE(regex.match("["));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Negated character class where closing bracket is first character")
    {
        auto regex = Regex("[^]]");

        // Positive test case(s)
        REQUIRE(regex.match("A"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("!"));

        // Negative test case(s)
        REQUIRE(!regex.match("]"));
    }

    SECTION("Character class with range (literal characters)")
    {
        auto regex = Regex("[b-d]");

        // Positive test case(s)
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("d"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("e"));
    }

    SECTION("Character class with range (escaped characters)")
    {
        auto regex = Regex("[\\^-\\^]");

        // Positive test case(s)
        REQUIRE(regex.match("^"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION("Character class with range (mixed literal and escaped)")
    {
        auto regex = Regex("[\\^-a]");

        // Positive test case(s)
        REQUIRE(regex.match("^"));
        REQUIRE(regex.match("_"));
        REQUIRE(regex.match("`"));
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("]"));
        REQUIRE(!regex.match("b"));
    }

    SECTION("Character class with multiple ranges")
    {
        auto regex = Regex("[b-d2-4]");

        // Positive test case(s)
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("d"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));
        REQUIRE(regex.match("4"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("e"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("5"));
    }

    SECTION("Character class with range containing a single character")
    {
        auto regex = Regex("[a-a]");

        // Positive test case(s)
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }

    SECTION(
      "Character class with range, literal character and escaped character")
    {
        auto regex = Regex("[a-cXYZ\\a]");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("X"));
        REQUIRE(regex.match("Y"));
        REQUIRE(regex.match("Z"));
        REQUIRE(regex.match("\a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
    }
}

SCENARIO("Match group")
{
    SECTION("Empty group")
    {
        auto regex = Regex("()");

        // Positive test case(s)
        REQUIRE(regex.match(""));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("("));
        REQUIRE(!regex.match(")"));
    }

    SECTION("Simple group")
    {
        auto regex = Regex("(a)");

        // Positive test case(s)
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("("));
        REQUIRE(!regex.match(")"));
    }

    SECTION("Nested group")
    {
        auto regex = Regex("((a))");

        // Positive test case(s)
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("("));
        REQUIRE(!regex.match(")"));
    }

    SECTION("Twice nested group")
    {
        auto regex = Regex("(((a)))");

        // Positive test case(s)
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("("));
        REQUIRE(!regex.match(")"));
    }
}

SCENARIO("Match quantifiers")
{
    SECTION("Kleene star (*)")
    {
        auto regex = Regex("a*");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Kleene star (*) operating on dot (any)")
    {
        auto regex = Regex(".*");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("xyz"));

        // Negative test case(s)
        REQUIRE(!regex.match("\n"));
    }

    SECTION("Kleene star (*) operating on a group")
    {
        auto regex = Regex("(a)*");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Kleene star (*) operating on a character class")
    {
        auto regex = Regex("[abc]*");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("bb"));
        REQUIRE(regex.match("bbb"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("cc"));
        REQUIRE(regex.match("ccc"));
        REQUIRE(regex.match("bcabcabc"));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Kleene plus (+)")
    {
        auto regex = Regex("a+");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Kleene plus (+) operating on dot (any)")
    {
        auto regex = Regex(".+");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("xyz"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("\n"));
    }

    SECTION("Kleene plus (+) operating on a group")
    {
        auto regex = Regex("(a)+");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Kleene plus (+) operating on a character class")
    {
        auto regex = Regex("[abc]+");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("bb"));
        REQUIRE(regex.match("bbb"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("cc"));
        REQUIRE(regex.match("ccc"));
        REQUIRE(regex.match("bcabcabc"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Optional (?)")
    {
        auto regex = Regex("a?");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Optional (?) operating on dot (any)")
    {
        auto regex = Regex(".?");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));

        // Negative test case(s)
        REQUIRE(!regex.match("\n"));
    }

    SECTION("Optional (?) operating on a group")
    {
        auto regex = Regex("(a)?");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));

        // Negative test case(s)
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Optional (?) operating on a character class")
    {
        auto regex = Regex("[abc]?");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));

        // Negative test case(s)
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("ab"));
        REQUIRE(!regex.match("ac"));
        REQUIRE(!regex.match("ba"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("bc"));
        REQUIRE(!regex.match("ca"));
        REQUIRE(!regex.match("cb"));
        REQUIRE(!regex.match("cc"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Ranged quantifier: lower bound only")
    {
        auto regex = Regex("a{3}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaaa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Ranged quantifier: lower bound only operating on dot (any)")
    {
        auto regex = Regex(".{3}");

        // Positive test case(s)
        REQUIRE(regex.match("abc"));
        REQUIRE(regex.match("xyz"));
    }

    SECTION("Ranged quantifier: lower bound only operating on a group")
    {
        auto regex = Regex("(a){3}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaaa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION(
      "Ranged quantifier: lower bound only operating on a character class")
    {
        auto regex = Regex("[abc]{3}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("aac"));
        REQUIRE(regex.match("aba"));
        REQUIRE(regex.match("abb"));
        REQUIRE(regex.match("abc"));
        REQUIRE(regex.match("aca"));
        REQUIRE(regex.match("acb"));
        REQUIRE(regex.match("acc"));
        REQUIRE(regex.match("baa"));
        REQUIRE(regex.match("bab"));
        REQUIRE(regex.match("bac"));
        REQUIRE(regex.match("bba"));
        REQUIRE(regex.match("bbb"));
        REQUIRE(regex.match("bbc"));
        REQUIRE(regex.match("bca"));
        REQUIRE(regex.match("bcb"));
        REQUIRE(regex.match("bcc"));
        REQUIRE(regex.match("caa"));
        REQUIRE(regex.match("cab"));
        REQUIRE(regex.match("cac"));
        REQUIRE(regex.match("cba"));
        REQUIRE(regex.match("cbb"));
        REQUIRE(regex.match("cbc"));
        REQUIRE(regex.match("cca"));
        REQUIRE(regex.match("ccb"));
        REQUIRE(regex.match("ccc"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaaa"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("bbbb"));
        REQUIRE(!regex.match("c"));
        REQUIRE(!regex.match("cc"));
        REQUIRE(!regex.match("cccc"));
    }

    SECTION("Ranged quantifier: upper bound is omitted")
    {
        auto regex = Regex("a{3,}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aaaa"));
        REQUIRE(regex.match("aaaaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on dot (any)")
    {
        auto regex = Regex(".{3,}");

        // Positive test case(s)
        REQUIRE(regex.match("abcdefgh"));
        REQUIRE(regex.match("xyz"));
    }

    SECTION("Ranged quantifier: upper bound is omitted operating on a group")
    {
        auto regex = Regex("a{3,}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aaaa"));
        REQUIRE(regex.match("aaaaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION(
      "Ranged quantifier: lower bound only operating on a character class")
    {
        auto regex = Regex("[abc]{3,}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("aac"));
        REQUIRE(regex.match("aba"));
        REQUIRE(regex.match("abb"));
        REQUIRE(regex.match("abc"));
        REQUIRE(regex.match("aca"));
        REQUIRE(regex.match("acb"));
        REQUIRE(regex.match("acc"));
        REQUIRE(regex.match("baa"));
        REQUIRE(regex.match("bab"));
        REQUIRE(regex.match("bac"));
        REQUIRE(regex.match("bba"));
        REQUIRE(regex.match("bbb"));
        REQUIRE(regex.match("bbc"));
        REQUIRE(regex.match("bca"));
        REQUIRE(regex.match("bcb"));
        REQUIRE(regex.match("bcc"));
        REQUIRE(regex.match("caa"));
        REQUIRE(regex.match("cab"));
        REQUIRE(regex.match("cac"));
        REQUIRE(regex.match("cba"));
        REQUIRE(regex.match("cbb"));
        REQUIRE(regex.match("cbc"));
        REQUIRE(regex.match("cca"));
        REQUIRE(regex.match("ccb"));
        REQUIRE(regex.match("ccc"));
        REQUIRE(regex.match("bacbacbcabc"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("c"));
        REQUIRE(!regex.match("cc"));
    }

    SECTION("Ranged quantifier: lower and upper bound")
    {
        auto regex = Regex("a{3,4}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aaaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaaaa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Ranged quantifier: lower and upper bound operating on dot (any)")
    {
        auto regex = Regex(".{3,4}");

        // Positive test case(s)
        REQUIRE(regex.match("abcd"));
        REQUIRE(regex.match("xyz"));
    }

    SECTION("Ranged quantifier: lower and upper bound operating on a group")
    {
        auto regex = Regex("a{3,4}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aaaa"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaaaa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION(
      "Ranged quantifier: lower and upper bound operating on a character class")
    {
        auto regex = Regex("[abc]{3,4}");

        // Positive test case(s)
        REQUIRE(regex.match("aaa"));
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("aac"));
        REQUIRE(regex.match("aba"));
        REQUIRE(regex.match("abb"));
        REQUIRE(regex.match("abc"));
        REQUIRE(regex.match("aca"));
        REQUIRE(regex.match("acb"));
        REQUIRE(regex.match("acc"));
        REQUIRE(regex.match("baa"));
        REQUIRE(regex.match("bab"));
        REQUIRE(regex.match("bac"));
        REQUIRE(regex.match("bba"));
        REQUIRE(regex.match("bbb"));
        REQUIRE(regex.match("bbc"));
        REQUIRE(regex.match("bca"));
        REQUIRE(regex.match("bcb"));
        REQUIRE(regex.match("bcc"));
        REQUIRE(regex.match("caa"));
        REQUIRE(regex.match("cab"));
        REQUIRE(regex.match("cac"));
        REQUIRE(regex.match("cba"));
        REQUIRE(regex.match("cbb"));
        REQUIRE(regex.match("cbc"));
        REQUIRE(regex.match("cca"));
        REQUIRE(regex.match("ccb"));
        REQUIRE(regex.match("ccc"));
        REQUIRE(regex.match("abca"));
        REQUIRE(regex.match("bcab"));
        REQUIRE(regex.match("cabc"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaaaa"));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }
}

SCENARIO("Match concatenation")
{
    SECTION("Concatenation of two characters")
    {
        auto regex = Regex("ab");

        // Positive test case(s)
        REQUIRE(regex.match("ab"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two escaped meta characters")
    {
        auto regex = Regex("\\*\\*");

        // Positive test case(s)
        REQUIRE(regex.match("**"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two escaped characters with special meaning")
    {
        auto regex = Regex("\\n\\r");

        // Positive test case(s)
        REQUIRE(regex.match("\n\r"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two characters from unicode (4 digits)")
    {
        auto regex = Regex("\\u0061\\u0062");

        // Positive test case(s)
        REQUIRE(regex.match("ab"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two characters from unicode (8 digits)")
    {
        auto regex = Regex("\\U00000061\\U00000062");

        // Positive test case(s)
        REQUIRE(regex.match("ab"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two character classes")
    {
        auto regex = Regex("[abc][123]");

        // Positive test case(s)
        REQUIRE(regex.match("a1"));
        REQUIRE(regex.match("a2"));
        REQUIRE(regex.match("a3"));
        REQUIRE(regex.match("b1"));
        REQUIRE(regex.match("b2"));
        REQUIRE(regex.match("b3"));
        REQUIRE(regex.match("c1"));
        REQUIRE(regex.match("c2"));
        REQUIRE(regex.match("c3"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two groups")
    {
        auto regex = Regex("(a)(b)");

        // Positive test case(s)
        REQUIRE(regex.match("ab"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Concatenation of two empty groups")
    {
        auto regex = Regex("()()");

        // Positive test case(s)
        REQUIRE(regex.match(""));
    }

    SECTION("Concatenation of empty and non-empty group")
    {
        auto regex = Regex("()(a)");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
    }
}

SCENARIO("Match alternation")
{
    SECTION("Alternation of two characters")
    {
        auto regex = Regex("a|b");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation with empty (epsilon) right hand side")
    {
        auto regex = Regex("a|");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match(""));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation with empty (epsilon) left hand side")
    {
        auto regex = Regex("|b");

        // Positive test case(s)
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match(""));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation with empty (epsilon) left and right hand side")
    {
        auto regex = Regex("|");

        // Positive test case(s)
        REQUIRE(regex.match(""));

        // Negative test case(s)
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two escaped meta characters")
    {
        auto regex = Regex("\\*|\\^");

        // Positive test case(s)
        REQUIRE(regex.match("*"));
        REQUIRE(regex.match("^"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two escaped characters with special meaning")
    {
        auto regex = Regex("\\n|\\r");

        // Positive test case(s)
        REQUIRE(regex.match("\n"));
        REQUIRE(regex.match("\r"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two characters from unicode (4 digits)")
    {
        auto regex = Regex("\\u0061|\\u0062");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two characters from unicode (8 digits)")
    {
        auto regex = Regex("\\U00000061|\\U00000062");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two character classes")
    {
        auto regex = Regex("[abc]|[123]");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("1"));
        REQUIRE(regex.match("2"));
        REQUIRE(regex.match("3"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two groups")
    {
        auto regex = Regex("(a)|(b)");

        // Positive test case(s)
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("A"));
        REQUIRE(!regex.match("1"));
        REQUIRE(!regex.match("!"));
    }

    SECTION("Alternation of two empty groups")
    {
        auto regex = Regex("()|()");

        // Positive test case(s)
        REQUIRE(regex.match(""));
    }

    SECTION("Alternation of empty and non-empty group")
    {
        auto regex = Regex("()|(a)");

        // Positive test case(s)
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
    }

    SECTION("Alternation and concatenation")
    {
        auto regex = Regex("123|abc");

        // Positive test case(s)
        REQUIRE(regex.match("123"));
        REQUIRE(regex.match("abc"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Alternation, concatenation and character class")
    {
        auto regex = Regex("123|[abc]|z");

        // Positive test case(s)
        REQUIRE(regex.match("123"));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("z"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }

    SECTION("Alternation, concatenation and group")
    {
        auto regex = Regex("123|(abc|xy)|z");

        // Positive test case(s)
        REQUIRE(regex.match("123"));
        REQUIRE(regex.match("abc"));
        REQUIRE(regex.match("xy"));
        REQUIRE(regex.match("z"));

        // Negative test case(s)
        REQUIRE(!regex.match(""));
    }
}

SCENARIO("Random tests")
{
    SECTION("Realistic tests using dates")
    {

        auto regex = Regex("\\d\\d-(\\d\\d|JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|"
                           "OCT|NOV|DEC)-\\d\\d(\\d\\d)?");
        REQUIRE(regex.match("12-12-22"));
        REQUIRE(regex.match("12-12-2022"));
        REQUIRE(regex.match("12-OCT-2022"));
    }

    SECTION("Hello World! in Kanji:「こんにちは世界」")
    {
        const std::array kanji = { '\xE3', '\x80', '\x8C', '\xE3', '\x81',
                                   '\x93', '\xE3', '\x82', '\x93', '\xE3',
                                   '\x81', '\xAB', '\xE3', '\x81', '\xA1',
                                   '\xE3', '\x81', '\xAF', '\xE4', '\xB8',
                                   '\x96', '\xE7', '\x95', '\x8C', '\xE3',
                                   '\x80', '\x8D', '\x00' };
        std::string string = kanji.data();

        auto regex = Regex(string);
        REQUIRE(regex.match(string));
    }
}

} // namespace
} // namespace regex