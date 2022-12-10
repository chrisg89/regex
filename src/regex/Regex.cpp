#include <regex/Regex.hpp>

#include "Alphabet.hpp"
#include "AST.hpp"
#include "CodePoint.hpp"
#include "DFA.hpp"
#include "Parser.hpp"
#include "Utf8Iterator.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>

namespace regex
{

using automata::DFA;
using parser::Parser;

class Regex::RegexImpl
{
public:
    explicit RegexImpl(const std::string& pattern);
    bool match(const std::string& target);

private:
    RegexImpl(const ast::AST& ast);

    automata::InputType findInAlphabet(CodePoint input);

    Alphabet mAlphabet;
    DFA mDFA;
};

Regex::RegexImpl::RegexImpl(const std::string& pattern)
: RegexImpl{Parser(pattern).parse()}
{}

Regex::RegexImpl::RegexImpl(const ast::AST& ast)
: mAlphabet{ast.makeAlphabet()}
, mDFA{ast.makeNFA(mAlphabet).makeDFA()}
{}

automata::InputType Regex::RegexImpl::findInAlphabet(CodePoint input)
{
    const auto within = [input](CodePointInterval interval)
    { 
        return input>= interval.first && input <= interval.second; 
    };

    const auto begin = mAlphabet.begin();
    const auto end = mAlphabet.end();
    auto result = std::find_if(begin, end, within);
    assert(result != end);

    // The index is the input to the automata
    return static_cast<automata::InputType>(std::distance(begin, result));
}

bool Regex::RegexImpl::match(const std::string& target)
{
    auto state = mDFA.getStartState();

    for( Utf8Iterator it = target.cbegin(); it != target.cend(); ++it )
    {
        // lookup the codepoint in the alphabet
        auto input = findInAlphabet(*it);

        // advance the DFA
        state = mDFA.step(state, input);
        
        // exit on a dead state
        if(mDFA.isDeadState(state))
        {
            break;
        }
    }

    return mDFA.isFinalState(state);
}

Regex::Regex(const std::string& pattern)
: impl{std::make_unique<RegexImpl>(pattern)}
{}

Regex::~Regex() = default;

bool Regex::match(const std::string& target)
{
    return impl->match(target);
}

} //namespace regex