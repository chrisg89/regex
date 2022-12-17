# What I learned

Designing and implementing a regex engine from scratch was an incredible learning experience. I'll break it down below.

## Learning the theory
Surprisingly (or not), topics covered in computation theory classes are very relevant when designing a regex engine. The following concepts were at the core of this implementation:
* [Deterministic Finite Automata (DFA)](https://en.wikipedia.org/wiki/Deterministic_finite_automaton)
* [Non-Deterministic Finite Automata (NFA)](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton)
* [Regular Grammar and Regular Language](https://en.wikipedia.org/wiki/Regular_grammar)
* [Context-Free Grammar(CFG)](https://en.wikipedia.org/wiki/Context-free_grammar)

## From theory to implementation
This implementation consists of two logical stages:
1. Front-end (parser)
2. Back-end (generator)

### The front-end (parser)

The front end is responsible for parsing the regex according to a given context-free grammar (CFG). Context-free grammar is generally described in a notation called the [extended Backus-Naur form (EBNF)](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) and generates context-free language. There are numerous types of parsers capable of parsing a context-free language. The most common hand-written parser is the [recursive-decent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser) (a top-down parser). Alternatives include the bottom-up parsers typically generated from tools such as [YACC](https://en.wikipedia.org/wiki/Yacc). This implementation uses a recursive-decent parser. The objective of the parser is to build the [Abstract Syntax Tree (AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree) used downstream by the back-end.

### The back-end (generator)

The AST built by the front-end provides the inputs required by [Thompson's Construction](https://en.wikipedia.org/wiki/Thompson%27s_construction) to generate an ε-NFA. An ε-NFA is an NFA that contains ε-transitions (transitions that don't consume inputs). A conversion from NFA to DFA takes place after removing all ε-transitions. The final step is to minimize the DFA for optimal performance.

### Executing the DFA and matching 

With the DFA constructed, matching a target string against the pattern (now implicitly part of the DFA) can proceed. Each character of the input string brings about a state transition of the DFA. Should the DFA end in a "final state" upon consumption of the last character of the target string, then the algorithm will report a successful match. Otherwise, the algorithm indicates a failure.

### The challenge of Unicode

At some point in the implementation, I considered supporting characters beyond basic ASCII. I decided to support [UTF-8 encoding](https://en.wikipedia.org/wiki/UTF-8) (for its compatibility with ASCII) and its ubiquity on the internet. Naturally, an encoding such as UTF-8 doesn't exist in a vacuum - it is backed by a character set, in this case, [Unicode](https://en.wikipedia.org/wiki/Unicode). The design I had previously crafted needed to change to accommodate Unicode. Doing so turned out to be a challenge because Unicode defines nearly 150,000 characters, and, by definition, a DFA must define a transition for each possible character in its alphabet. Creating a DFA with thousands of state transitions was not practical. Ultimately, I migrated to a design that maps input characters to a "compressed alphabet" known to the DFA. Consequently, a translation of the current input to the corresponding DFA input must occur before advancing the DFA.

## The C++ ecosystem

Larger projects greatly benefit from adopting best practices and taking advantage of tooling in the c++ ecosystem. This project embraces best practices such as:

* Utilizing modern C++ libraries and language features
* Using modern CMake best practices for building, testing, and installation
* Using clang-tidy for static code analysis
* Using Valgrind for dynamic (runtime) code analysis
* Using Clang-format to apply a consistent style across the code base
* Using Doxygen to document the public API
* Enabling many compiler warnings
* Hosting the project on GitHub 
* Automatic deployment of generated documentation on Github pages
* Automating most of the above via GitHub actions (continuous integration)

I also improve my understanding of time complexity and space complexity concerning algorithms and containers in the standard library.