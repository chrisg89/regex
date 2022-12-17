# regex

[![Coverage Status](https://coveralls.io/repos/github/chrisg89/regex/badge.svg?branch=main)](https://coveralls.io/github/chrisg89/regex?branch=main)
![CI](https://github.com//chrisg89/regex/actions/workflows/CI.yml/badge.svg)


## Motivation

Why another regex implementation?   
While anyone is welcome to use this library, I doubt that it will be of any practical use. Most programming languages come with a native regex implementation (e.g. std::regex in c++). Furthermore, many standalone implementations exist with more functionality and better performance than this library. I created this library first and foremost as a [learning exercise](https://github.com/chrisg89/regex/blob/main/docs/learning.md).

## About
This library differentiates itself from other modern regular expression engines by being entirely implemented as a Deterministic Finite Automata (for better or worse). Engines driven by an underlying backtracking NFA tend to be faster because their construction is relatively cheap. However, this does come at a cost. Backtracking can lead to [Catastrophic Backtracking](https://www.regular-expressions.info/catastrophic.html). Furthermore, NFAs are, in principle, less deterministic in their behavior than DFAs. A DFA-based implementation has a high upfront construction time, but can match inputs in linear time.

## Installation and CMake integration

Please check [here](https://github.com/chrisg89/regex/blob/main/docs/install.md) for installation and CMake integration instructions.

## Usage

Find online Doxygen documentation [here](https://chrisg89.github.io/regex/).

Example code snippet:
```
#include <iostream>
#include <regex/Regex.hpp>
#include <string>

int main()
{
    auto regex = regex::Regex("col[ou]r");
    std::cout << std::boolalpha << regex.match("color") << std::endl;
    return 0;
}
```

Example code snippet with non-ASCII character code points:
```
#include <iostream>
#include <regex/Regex.hpp>
#include <string>

int main()
{
    auto regex = regex::Regex("[AÅÃ]");
    std::cout << std::boolalpha << regex.match("A") << std::endl;
    std::cout << std::boolalpha << regex.match("Å") << std::endl;
    std::cout << std::boolalpha << regex.match("Ã") << std::endl;
    return 0;
}
```

Exceptions are thrown on illegal usage (e.g. usage of unsupported regex features):
```
#include <iostream>
#include <regex/Regex.hpp>
#include <string>

int main()
{
    /* Anchors are not supported*/
    auto regex = regex::Regex("^Hello World!$");
    return 0;
}
```

throws exception:
```
terminate called after throwing an instance of 'std::runtime_error'
  what():  Error at position 1. Message: Anchors are not supported 
```

Take a look at the [unit tests](https://github.com/chrisg89/regex/blob/main/tests/RegexMatch_tests.cpp) for more examples.

## Supported Regex Features

[Regular Expression Engine Comparison Chart](https://gist.github.com/CMCDragonkai/6c933f4a7d713ef712145c5eb94a1816) of all mainstream regular expression engines

<table class="body">
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/characters.html">Characters</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td>Backslash escapes one metacharacter</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\Q...\E escapes a string of metacharacters</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td>\x00 through \xFF (ASCII character)</td>
        <td>NO</td>
        <td>See <br> \u0000-\uFFFF <br> \u00000000-\u0010FFFF </td>
    </tr>
    <tr>
        <td>\n (LF)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\r (CR)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\t (tab)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\f (form feed)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\v (vtab)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\a (bell)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\e (escape)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td>\b (backspace)</td>
        <td> NO</td>
        <td></td>
    </tr>
    <tr>
        <td>\B (backslash)</td>
        <td> NO</td>
        <td></td>
    </tr>
    <tr>
        <td>\\ (backslash)</td>
        <td> YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\cA through \cZ (control character)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td>\ca through \cz (control character)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/charclass.html">Character Classes or Character Sets</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td>[abc] character class</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>[^abc] negated character class</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>Hyphen in [\d-z] is a literal</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>Hyphen in [a-\d] is a literal</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>Hyphen in [\w-\d] is a literal</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>Backslash escapes one character class metacharacter</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td>\Q...\E escapes a string of character class metacharacters</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td>\d shorthand for digits</td>
        <td>YES</td>
        <td>Equivalent to [0-9]</td>
    </tr>
    <tr>
        <td>\D (negation of \d)</td>
        <td>YES</td>
        <td>Equivalent to [^0-9]</td>
    </tr>
    <tr>
        <td>\w shorthand for word characters</td>
        <td> YES</td>
        <td>Equivalent to [a-zA-Z0-9_]</td>
    </tr>
    <tr>
        <td>\W (negation of \w)</td>
        <td>YES</td>
        <td>Equivalent to [^a-zA-Z0-9_]</td>
    </tr>
    <tr>
        <td>\s shorthand for whitespace</td>
        <td>YES</td>
        <td>Equivalent to [ \t\r\n\f]</td>
    </tr>
    <tr>
        <td>\S (negation of \s)</td>
        <td>YES</td>
        <td>Equivalent to [^ \t\r\n\f]</td>
    </tr>
    <tr>
        <td>[\b] backspace</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/dot.html">Dot</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> . (dot) </td>
        <td>YES</td>
        <td>Excludes new-line (\n)</td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/anchors.html">Anchors</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> ^ (start of string/line)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> $ (end of string/line)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \A (start of string)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \Z (end of string, before final line break)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \z (end of string)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \` (start of string)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \' (end of string)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/wordboundaries.html">Word Boundaries</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> \b (at the beginning or end of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \B (NOT at the beginning or end of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \y (at the beginning or end of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \Y (NOT at the beginning or end of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \m (at the beginning of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \M (at the end of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \< (at the beginning of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \> (at the end of a word)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/alternation.html">Alternation</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> | (alternation)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/repeat.html">Quantifiers</a></th>
    </tr>
    <tr>
        <td> ? (0 or 1)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> * (0 or more)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> + (1 or more)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> {n} (exactly n)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> {n,m} (between n and m)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> {n,} (n or more)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> ? after any of the above quantifiers to make it "lazy"</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/brackets.html">Grouping and Backreferences</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> (regex) (numbered capturing group)</td>
        <td>NO</td>
        <td>Used for non-capturing groups instead (see below)</td>
    </tr>
    <tr>
        <td> (regex) (non-capturing group)</td>
        <td>YES</td>
        <td> This library uses this syntax for non-capturing groups <br> In contrast, most other implementations use it for capturing groups!</td>
    </tr>
    <tr>
        <td> (?:regex) (non-capturing group)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \1 through \9 (backreferences)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \10 through \99 (backreferences)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Forward references \1 through \9</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Nested references \1 through \9</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Backreferences non-existent groups are an error</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Backreferences to failed groups also fail</td>
        <td>NO</td>
        <td></td>
    </tr>  
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/modifiers.html">Modifiers</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> (?i) (case insensitive)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?s) (dot matches newlines)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?m) (^ and $ match at line breaks)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?x) (free-spacing mode)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?n) (explicit capture)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?-ismxn) (turn off mode modifiers)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?ismxn:group) (mode modifiers local to group)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/atomic.html">Atomic Grouping and Possessive Quantifiers</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> (?>regex) (atomic group)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> ?+, *+, ++ and {m,n}+ (possessive quantifiers)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/lookaround.html">Lookaround</a></th>
    </tr>
    <tr>
        <td> (?=regex) (positive lookahead)</td>
        <td>NO</td>
        <td></td>
    </tr>
        <td> (?!regex) (negative lookahead)</td>
        <td>NO</td>
        <td></td>
    </tr>
        <td> (?<=text) (positive lookbehind)</td>
        <td>NO</td>
        <td></td>
    </tr>
        <td> (?<!text) (negative lookbehind)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/continue.html">Continuing from The Previous Match</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> \G (start of match attempt)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/conditional.html">Conditionals</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> (?(?=regex)then|else) (using any lookaround)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?(regex)then|else)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?(1)then|else)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?(group)then|else)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/comments.html">Comments</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> (?#comment)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/freespacing.html">Free-Spacing Syntax</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> Free-spacing syntax supported</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Character class is a single token</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> # starts a comment</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/unicode.html">Unicode Characters</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> \X (Unicode grapheme)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \u0000 through \uFFFF (4 digit Unicode character)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> \U00000000 through \u0010FFFF (8 digit Unicode character)</td>
        <td>YES</td>
        <td></td>
    </tr>
    <tr>
        <td> \x{0} through \x{FFFF} (Unicode character)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/unicode.html#prop">Unicode Properties, Scripts and Blocks</a></th>
    </tr>
    <tr>
        <td> \pL through \pC (Unicode properties)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{L} through \p{C} (Unicode properties)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{Lu} through \p{Cn} (Unicode property)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{L&} and \p{Letter&} (equivalent of [\p{Lu}\p{Ll}\p{Lt}] Unicode properties)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsL} through \p{IsC} (Unicode properties)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsLu} through \p{IsCn} (Unicode property)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{Letter} through \p{Other} (Unicode properties)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{Lowercase_Letter} through \p{Not_Assigned} (Unicode property)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsLetter} through \p{IsOther} (Unicode properties)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsLowercase_Letter} through \p{IsNot_Assigned} (Unicode property)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{Arabic} through \p{Yi} (Unicode script)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsArabic} through \p{IsYi} (Unicode script)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{BasicLatin} through \p{Specials} (Unicode block)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{InBasicLatin} through \p{InSpecials} (Unicode block)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsBasicLatin} through \p{IsSpecials} (Unicode block)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Part between {} in all of the above is case insensitive</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> Spaces, hyphens and underscores allowed in all long names listed above  <br>
         (e.g. BasicLatin can be written as Basic-Latin or Basic_Latin or Basic Latin)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \P (negated variants of all \p as listed above)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{^...} (negated variants of all \p{...} as listed above)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/named.html">Named Capture and Backreferences</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> (?<name>regex) (.NET-style named capturing group)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?'name'regex) (.NET-style named capturing group)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \k<name> (.NET-style named backreference)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \k'name' (.NET-style named backreference)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?P<name>regex) (Python-style named capturing group</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> (?P=name) (Python-style named backreference)</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> multiple capturing groups can have the same name</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/xmlcharclass.html">XML Character Classes</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> \i, \I, \c and \C shorthand XML name character classes</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> [abc-[abc]] character class subtraction</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <th colspan="3"><a href="http://www.regular-expressions.info/posixbrackets.html">POSIX Bracket Expressions</a></th>
    </tr>
    <tr>
        <th align="left">Feature</th>
        <th align="left">Support</th>
        <th align="left">Note(s)</th>
    </tr>
    <tr>
        <td> [:alpha:] POSIX character class</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{Alpha} POSIX character class</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> \p{IsAlpha} POSIX character class</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> [.span-ll.] POSIX collation sequence</td>
        <td>NO</td>
        <td></td>
    </tr>
    <tr>
        <td> [=x=] POSIX character equivalence</td>
        <td>NO</td>
        <td></td>
    </tr>
</table>

## Quality

This repository employs the following practices to achieve a reasonable level of quality:
* Modern CMake to build, test, and install the library.
* Continuous integration that:
    * builds on GNU g++
    * builds on clang++
    * runs unit tests 
    * runs static code analysis (clang-tidy)
    * runs dynamic code analysis (Valgrind)
    * analyzes unit test code coverage
    * builds with many compiler warnings enabled