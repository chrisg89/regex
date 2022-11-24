#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"
#include "NFA.hpp"
#include "ThompsonConstruction.hpp"

#include <iomanip>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace regex::ast
{

class Node;
using NodePtr = std::unique_ptr<Node>;
using NFA = fa::NFA;

class Node
{
public:
    virtual BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const = 0;
    virtual void print(std::string&) const = 0;
    virtual void makeAlphabet(Alphabet&) const = 0; 
};

class Alternative : public Node
{
public:
    Alternative(NodePtr& lhs, NodePtr& rhs)
    : mLeft {std::move(lhs)}
    , mRight {std::move(rhs)}
    {}

    BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto BB1 = mLeft->makeNFA(alphabet, nfa);
        auto BB2 = mRight->makeNFA(alphabet, nfa);

        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        nfa.addTransition(fa::kEpsilon, entry, BB1.entry);
        nfa.addTransition(fa::kEpsilon, entry, BB2.entry);
        nfa.addTransition(fa::kEpsilon, BB1.exit, exit);
        nfa.addTransition(fa::kEpsilon, BB2.exit, exit);
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

    BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto BB1 = mLeft->makeNFA(alphabet, nfa);
        auto BB2 = mRight->makeNFA(alphabet, nfa);

        auto entry = BB1.entry;
        auto exit = BB2.exit;
        nfa.addTransition(fa::kEpsilon, BB1.exit, BB2.entry);
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
    Quantifier(NodePtr& mInner, uint64_t min, uint64_t max, bool isMaxBounded)
    : mInner {std::move(mInner)}
    , mMin {min}
    , mMax {max}
    , mIsMaxBounded {isMaxBounded}
    {}

    BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        auto prev = entry;

        for(uint64_t min = 0; min < mMin; min++)
        {
            auto next = mInner->makeNFA(alphabet, nfa);
            nfa.addTransition(fa::kEpsilon, prev, next.entry);
            prev = next.exit;
        }

        if(mIsMaxBounded)
        {
            for(uint64_t max = mMin; max < mMax; max++)
            {
                auto next = mInner->makeNFA(alphabet, nfa);
                nfa.addTransition(fa::kEpsilon, prev, next.entry);
                nfa.addTransition(fa::kEpsilon, prev, exit);
                prev = next.exit;
            }
        }
        else
        {
            auto next = mInner->makeNFA(alphabet, nfa);
            nfa.addTransition(fa::kEpsilon, prev, next.entry);
            nfa.addTransition(fa::kEpsilon, prev, exit);
            nfa.addTransition(fa::kEpsilon, next.exit, next.entry);
            prev = next.exit;
        }

        nfa.addTransition(fa::kEpsilon, prev, exit);
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

    uint64_t mMin;
    uint64_t mMax;
    bool mIsMaxBounded;     
    NodePtr mInner;
};

class Epsilon : public Node
{
public:
    BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);
        nfa.addTransition(fa::kEpsilon, entry, exit);
        return BlackBox(entry, exit);
    }

    void makeAlphabet(Alphabet& alphabet) const final
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

    CharacterRange(CodePoint codepoint)  
    : mStart{codepoint}
    , mEnd{codepoint}
    {}

    BlackBox makeNFA(const Alphabet& alphabet, NFA& nfa) const final
    {
        auto interval = CodePointInterval{mStart, mEnd};

        auto entry = nfa.addState(false, false);
        auto exit = nfa.addState(false, false);

        int index = 0;
        for (auto c : alphabet)
        {
            if(isSubset(interval, c))
            {
                nfa.addTransition(index, entry, exit);
            }
            index++;
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

    AST(NodePtr& node ) : mRoot{std::move(node)}
    {}

    std::string print() const
    {
        std::string str;
        mRoot->print(str);
        return str;
    }

    Alphabet makeAlphabet() const
    {
        Alphabet alphabet;
        mRoot->makeAlphabet(alphabet);
        DisjoinOverlap(alphabet, kCodePointMin, kCodePointMax);
        return alphabet;
    }

    NFA makeNFA(const Alphabet& alphabet) const
    {
        // Make the NFA's alphabet
        auto nfaAlphabet = fa::Alphabet(alphabet.size());
        std::iota(std::begin(nfaAlphabet), std::end(nfaAlphabet), 0);

        // Make the empty NFA
        auto nfa = NFA(nfaAlphabet);

        // Populate the NFA using thompson construction
        auto bb = mRoot->makeNFA(alphabet, nfa);
        auto start = nfa.addState(true, false);
        auto end = nfa.addState(false, true);
        nfa.addTransition(fa::kEpsilon, start, bb.entry);
        nfa.addTransition(fa::kEpsilon, bb.exit, end);

        return nfa;
    }

private:
    NodePtr mRoot;
};

} // namespace regex::ast
