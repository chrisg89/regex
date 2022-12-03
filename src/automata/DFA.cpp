#include "DFA.hpp"

#include <cassert>
#include <unordered_set>

namespace automata
{

DFAState::DFAState(StateId id, bool isStart, bool isFinal)
    : mId{id}
    , mIsStart{isStart}
    , mIsFinal{isFinal}
    , mTransitions{}
    , mIsDead{true}
{}

void DFAState::addTransition(InputType input, StateId destination)
{
    mTransitions[input] = destination;

    if(destination != mId)
    {
        mIsDead = false;
    }
}

DFA::DFA(Alphabet alphabet)
    : mStates{}
    , mStateCount{0}
    , mStartState{kNullState}
    , mFinalStates{}
    , mAlphabet{alphabet}
{}

StateId DFA::addState(bool isStart, bool isFinal)
{
    if(isStart)
    {
        assert(mStartState == kNullState);
        mStartState = mStateCount;
    }

    if(isFinal)
    {
        mFinalStates.emplace_back(mStateCount);
    }

    mStates.emplace_back(mStateCount, isStart, isFinal);

    return mStateCount++;
}

void DFA::addTransition(InputType input, StateId source, StateId destination)
{
    mStates.at(source).addTransition(input, destination);
}

std::string DFA::serialize()
{
    std::string out = "";
    
    for (auto state : mStates)
    {
        out += std::to_string(state.mId);
        out += " : Start =" ;
        out += (state.mIsStart? "true" : "false");
        out += " , Final = ";
        out += (state.mIsFinal ? "true" : "false");
        out += " , Dead = ";
        out += (state.mIsDead ? "true" : "false");
        out += "\n";

        for (auto const& [input, destination] : state.mTransitions)
        {
            out += std::to_string(state.mId);
            out += " -> ";
            out += std::to_string(destination);
            out += " : ";
            out += input;
            out += "\n";
        }
    }

    return out;
}

StateId DFA::getStartState()
{
    assert(mStartState != kNullState);
    return mStartState;
}

StateId DFA::step(StateId current, InputType input)
{
    auto next = mStates.at(current).mTransitions.at(input);
    return mStates.at(next).mId;
}

bool DFA::isDeadState(StateId current)
{
    return mStates.at(current).mIsDead;
}

bool DFA::isFinalState(StateId current)
{
    return mStates.at(current).mIsFinal;
}

struct Partition
{
    Partition(PartitionId id)
    : States{}
    , Leader{kNullState}
    , ID{id}
    {}

    void insert(StateId state)
    {
        if(Leader == kNullState)
        {
            Leader = state;
        }

        States.insert(state);
    }

    std::unordered_set<StateId> States;
    StateId Leader;
    PartitionId ID;
};

struct PartitionPool
{
    PartitionPool();

    PartitionId addPartition();

    ParitionMap makePartitionMap();

    void addState(PartitionId partition, StateId state);

    std::vector<Partition> Partitions;

    int PartitionCount;

};

PartitionPool::PartitionPool()
: PartitionCount{0}
, Partitions{}
{}

PartitionId PartitionPool::addPartition()
{
    Partitions.emplace_back(PartitionCount);
    return PartitionCount++;
}

ParitionMap PartitionPool::makePartitionMap()
{
    ParitionMap map;

    for (auto parition : Partitions)
    {
        for (auto state : parition.States)
        {
            map[state] = parition.ID;
        }
    }

    return map;
}

void DFA::minimizeDFA()
{
    PartitionPool pool;
    ParitionMap prevPartitionMap;
    ParitionMap currPartitionMap;

    auto partitionFinal = kNullPartition;
    auto partitionNonFinal = kNullPartition;

    for (auto state : mStates)
    {
        if (!state.mIsFinal)
        {
            if(partitionNonFinal == kNullState)
            {
                partitionNonFinal = pool.addPartition();
            }
            pool.Partitions[partitionNonFinal].insert(state.mId);
        }
        else
        {
            if(partitionFinal == kNullPartition)
            {
                partitionFinal = pool.addPartition();
            }
            pool.Partitions[partitionFinal].insert(state.mId);
        }
    }

    currPartitionMap = pool.makePartitionMap();

    while(currPartitionMap != prevPartitionMap)
    {

        auto parition = PartitionId{0};
        std::vector<StateId> movedStates{};

        while(parition < pool.Partitions.size()) 
        {
            movedStates.clear();

            for (auto state : pool.Partitions[parition].States)
            {
                if(state != pool.Partitions[parition].Leader)
                {
                    if(!checkEquivalence(currPartitionMap, pool.Partitions[parition].Leader, state))
                    {
                        movedStates.emplace_back(state);
                    }
                }
            }

            if(!movedStates.empty())
            {
                auto newPartition = pool.addPartition();

                for (auto state : movedStates)
                {
                    pool.Partitions[parition].States.erase(state);
                    pool.Partitions[newPartition].insert(state);
                }
            }

            parition++;
        }

        prevPartitionMap = currPartitionMap;
        currPartitionMap = pool.makePartitionMap();

        
    }



    //////////////////////////////////////////////////////////////////////////////////////

    DFA newDFA(mAlphabet);

    for (auto partition : pool.Partitions)
    {
        bool isStart = false;
        bool isFinal = false;

        for (auto state : pool.Partitions[partition.ID].States)
        {
            isStart |= mStates.at(state).mIsStart;
            isFinal |= mStates.at(state).mIsFinal;
        }

        // parition id => new state id
        auto newState = newDFA.addState(isStart, isFinal);

        for(auto c : mAlphabet)
        {
            auto targetState = currPartitionMap.at(mStates.at(partition.Leader).mTransitions.at(c));
            newDFA.addTransition(c, newState, targetState);
        }
    }

    //STEP3: replace old DFA with new DFA
    *this = std::move(newDFA);

}

bool DFA::checkEquivalence(ParitionMap paritionMap, StateId stateA, StateId stateB)
{
    for(auto c : mAlphabet)
    {
        auto stateADest = mStates.at(stateA).mTransitions.at(c);
        auto stateBDest = mStates.at(stateB).mTransitions.at(c);

        if (stateADest != stateBDest)
        {
            if(paritionMap[stateADest] != paritionMap[stateBDest])
            {
                return false;
            }
        } 
    }

    return true;
}

} //namespace automata