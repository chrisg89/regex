#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"
#include "NFA.hpp"

#include <iomanip>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace regex::ast
{

using automata::StateId;

struct BlackBox
{
    BlackBox(StateId entry, StateId exit)
    : Entry {entry}
    , Exit {exit}
    {}

    StateId Entry;
    StateId Exit;
};

class Node;
using NodePtr = std::unique_ptr<Node>;
using automata::NFA;

class Node
{
public:
    [[nodiscard]] virtual BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const = 0;
    virtual void print(std::string&) const = 0;
    virtual void makeAlphabet(Alphabet&) const = 0; 
    virtual ~Node() = default;

    Node() = default;

    // no copy
    Node ( const Node& ) = delete;
    Node& operator= ( const Node& ) = delete;

    // move allowed
    Node ( Node&& ) = default;
    Node& operator= ( Node&& ) = default;
};

class Alternative : public Node
{
public:
    Alternative(NodePtr& lhs, NodePtr& rhs)
    : mLeft {std::move(lhs)}
    , mRight {std::move(rhs)}
    {}

    [[nodiscard]] BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto BB1 = mLeft->makeNFA(alphabet, nfa);
        auto BB2 = mRight->makeNFA(alphabet, nfa);

        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        nfa.addTransition(automata::kEpsilon, entry, BB1.Entry);
        nfa.addTransition(automata::kEpsilon, entry, BB2.Entry);
        nfa.addTransition(automata::kEpsilon, BB1.Exit, exit);
        nfa.addTransition(automata::kEpsilon, BB2.Exit, exit);
        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet& alphabet) const final
    {
        mLeft->makeAlphabet(alphabet);
        mRight->makeAlphabet(alphabet);
    }

    void print(std::string& str) const final
    {
        str+= "(";
        mLeft->print(str);
        str+= "|";
        mRight->print(str);
        str+= ")";
    }
     
    NodePtr mLeft;
    NodePtr mRight;
};

class Concatenation : public Node
{
public:
    Concatenation(NodePtr& lhs, NodePtr& rhs)
    : mLeft {std::move(lhs)}
    , mRight {std::move(rhs)}
    {}

    [[nodiscard]] BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto BB1 = mLeft->makeNFA(alphabet, nfa);
        auto BB2 = mRight->makeNFA(alphabet, nfa);

        auto entry = BB1.Entry;
        auto exit = BB2.Exit;
        nfa.addTransition(automata::kEpsilon, BB1.Exit, BB2.Entry);
        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet& alphabet) const final
    {
        mLeft->makeAlphabet(alphabet);
        mRight->makeAlphabet(alphabet);
    }

    void print(std::string& str) const final
    {
        str+= "(";
        mLeft->print(str);
        mRight->print(str);
        str+= ")";
    }
     
    NodePtr mLeft;
    NodePtr mRight;
};

class Quantifier : public Node
{
public:
    Quantifier(NodePtr& inner, uint64_t min, uint64_t max, bool isMaxBounded)
    : mInner {std::move(inner)}
    , mMin {min}
    , mMax {max}
    , mIsMaxBounded {isMaxBounded}
    {}

    [[nodiscard]] BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        auto prev = entry;

        for(uint64_t min = 0; min < mMin; ++min)
        {
            auto next = mInner->makeNFA(alphabet, nfa);
            nfa.addTransition(automata::kEpsilon, prev, next.Entry);
            prev = next.Exit;
        }

        if(mIsMaxBounded)
        {
            for(uint64_t max = mMin; max < mMax; ++max)
            {
                auto next = mInner->makeNFA(alphabet, nfa);
                nfa.addTransition(automata::kEpsilon, prev, next.Entry);
                nfa.addTransition(automata::kEpsilon, prev, exit);
                prev = next.Exit;
            }
        }
        else
        {
            auto next = mInner->makeNFA(alphabet, nfa);
            nfa.addTransition(automata::kEpsilon, prev, next.Entry);
            nfa.addTransition(automata::kEpsilon, prev, exit);
            nfa.addTransition(automata::kEpsilon, next.Exit, next.Entry);
            prev = next.Exit;
        }

        nfa.addTransition(automata::kEpsilon, prev, exit);
        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet& alphabet) const final
    {
        mInner->makeAlphabet(alphabet);
    }

    void print(std::string& str) const final
    {
        mInner->print(str);
        if(mIsMaxBounded)
        {
            str += "{" + std::to_string(mMin) + "," + std::to_string(mMax) + "}";
        }
        else
        {
            str += "{" + std::to_string(mMin) + "," + "}";
        }
    }
     
    NodePtr mInner;
    uint64_t mMin;
    uint64_t mMax;
    bool mIsMaxBounded;
};

class Epsilon : public Node
{
public:
    [[nodiscard]] BlackBox makeNFA(const Alphabet&, NFA& nfa) const final
    {
        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        nfa.addTransition(automata::kEpsilon, entry, exit);
        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet&) const final
    {
        /* Do nothing */
    }

    void print(std::string& str) const final
    {
        str+= "";
    }
};

class Null : public Node
{
public:
    [[nodiscard]] BlackBox makeNFA(const Alphabet&, NFA& nfa) const final
    {
        //entry and exit are not connected by any transition
        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet&) const final
    {
        /* Do nothing */
    }

    void print(std::string& str) const final
    {
        str+= "";
    }
};

class CharacterRange : public Node
{
public:
    CharacterRange(CodePoint start, CodePoint end)
    : mStart{start}
    , mEnd{end}
    {}

    explicit CharacterRange(CodePoint codepoint)  
    : mStart{codepoint}
    , mEnd{codepoint}
    {}

    [[nodiscard]] BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto interval = CodePointInterval{mStart, mEnd};

        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);

        for(auto i=0U; i<alphabet.size(); ++i)
        {
            if(isSubset(alphabet[i], interval))
            {
                nfa.addTransition(static_cast<automata::InputType>(i), entry, exit);
            }
        }

        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet& alphabet) const final
    {
        alphabet.emplace_back(mStart, mEnd);
    }

    void print(std::string& str) const final
    {
        std::stringstream ss;
        ss << "[";
        ss << "\\U" << std::hex << std::setfill ('0') << std::setw(8) << mStart;
        ss << "-";
        ss << "\\U" << std::hex << std::setfill ('0') << std::setw(8) << mEnd;
        ss << "]";
        str+= ss.str();
    }

    CodePoint mStart;
    CodePoint mEnd;
};

class AST
{
public:

    explicit AST(NodePtr& node ) : mRoot{std::move(node)}
    {}

    [[nodiscard]] std::string print() const
    {
        std::string str;
        mRoot->print(str);
        return str;
    }

    [[nodiscard]] Alphabet makeAlphabet() const
    {
        Alphabet alphabet;
        mRoot->makeAlphabet(alphabet);
        disjoinOverlap(alphabet, kCodePointMin, kCodePointMax);
        return alphabet;
    }

    [[nodiscard]] NFA makeNFA(const Alphabet& alphabet) const
    {
        // Make the NFA's alphabet
        auto nfaAlphabet = automata::Alphabet(alphabet.size());
        std::iota(std::begin(nfaAlphabet), std::end(nfaAlphabet), 0);

        // Make the empty NFA
        auto nfa = NFA(nfaAlphabet);

        // Populate the NFA using thompson construction
        auto bb = mRoot->makeNFA(alphabet, nfa);
        auto start = nfa.addState(true, false);
        auto end = nfa.addState(false, true);
        nfa.addTransition(automata::kEpsilon, start, bb.Entry);
        nfa.addTransition(automata::kEpsilon, bb.Exit, end);

        // The NFA built via Thompson-Construction is an "Epsilon NFA"
        // Such NFA contains epsilon transitions. Removal of said 
        // transitions yields a standard NFA.
        nfa.removeEpsilonTransitions();

        return nfa;
    }

private:
    NodePtr mRoot;
};

} // namespace regex::ast
