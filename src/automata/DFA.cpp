#include "DFA.hpp"

#include <cassert>
#include <optional>
#include <unordered_set>

namespace automata
{

DFAState::DFAState(StateId id, bool isStart, bool isFinal)
    : Id{id}
    , IsStart{isStart}
    , IsFinal{isFinal}
    , IsDead{true}
    , Transitions{}
{}

void DFAState::addTransition(InputType input, StateId destination)
{
    Transitions[input] = destination;

    if(destination != Id)
    {
        IsDead = false;
    }
}

DFA::DFA(Alphabet alphabet)
    : mStates{}
    , mStateCount{0}
    , mStartState{}
    , mFinalStates{}
    , mAlphabet{alphabet}
{}

StateId DFA::addState(bool isStart, bool isFinal)
{
    if(isStart)
    {
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

std::string DFA::serialize() const
{
    std::string out = "";
    
    for (const auto& state : mStates)
    {
        out += std::to_string(state.Id);
        out += " : Start =" ;
        out += (state.IsStart? "true" : "false");
        out += " , Final = ";
        out += (state.IsFinal ? "true" : "false");
        out += " , Dead = ";
        out += (state.IsDead ? "true" : "false");
        out += "\n";

        for (auto const& [input, destination] : state.Transitions)
        {
            out += std::to_string(state.Id);
            out += " -> ";
            out += std::to_string(destination);
            out += " : ";
            out += std::to_string(input);
            out += "\n";
        }
    }

    return out;
}

StateId DFA::getStartState() const
{
    return mStartState;
}

StateId DFA::step(StateId current, InputType input) const
{
    const auto next = mStates.at(current).Transitions.at(input);
    return mStates.at(next).Id;
}

bool DFA::isDeadState(StateId current) const
{
    return mStates.at(current).IsDead;
}

bool DFA::isFinalState(StateId current) const
{
    return mStates.at(current).IsFinal;
}

struct Partition
{
    Partition(PartitionId id)
    : States{}
    , LeaderSelected {false}
    , Leader{}
    , Id{id}
    {}

    void insert(StateId state)
    {
        if(LeaderSelected == false)
        {
            Leader = state;
            LeaderSelected = true;
        }

        States.insert(state);
    }

    std::unordered_set<StateId> States;
    bool LeaderSelected;
    StateId Leader;
    const PartitionId Id;
};

struct PartitionPool
{
    PartitionPool();

    PartitionId addPartition();

    ParitionMap makePartitionMap() const;

    void addState(PartitionId partition, StateId state);

    std::vector<Partition> Partitions;

    unsigned int PartitionCount;

};

PartitionPool::PartitionPool()
: Partitions{}
, PartitionCount{0}
{}

PartitionId PartitionPool::addPartition()
{
    Partitions.emplace_back(PartitionCount);
    return PartitionCount++;
}

ParitionMap PartitionPool::makePartitionMap() const
{
    ParitionMap map;

    for (const auto& parition : Partitions)
    {
        for (const auto state : parition.States)
        {
            map[state] = parition.Id;
        }
    }

    return map;
}

void DFA::minimize()
{
    // Using the partitioning method of DFA minimization 

    //STEP1: parition the original dfa

    PartitionPool pool;
    ParitionMap prevPartitionMap;
    ParitionMap currPartitionMap;

    auto partitionFinal = std::optional<PartitionId>();
    auto partitionNonFinal = std::optional<PartitionId>();

    for (const auto& state : mStates)
    {
        if (!state.IsFinal)
        {
            if(!partitionNonFinal.has_value())
            {
                partitionNonFinal = pool.addPartition();
            }
            pool.Partitions[partitionNonFinal.value()].insert(state.Id);
        }
        else
        {
            if(!partitionFinal.has_value())
            {
                partitionFinal = pool.addPartition();
            }
            pool.Partitions[partitionFinal.value()].insert(state.Id);
        }
    }

    currPartitionMap = pool.makePartitionMap();

    while(currPartitionMap != prevPartitionMap)
    {
        std::vector<StateId> movedStates{};

        for(const auto& partition : pool.Partitions)
        {
            movedStates.clear();

            for (const auto state : pool.Partitions[partition.Id].States)
            {
                if(state != pool.Partitions[partition.Id].Leader)
                {
                    if(!checkEquivalence(currPartitionMap, pool.Partitions[partition.Id].Leader, state))
                    {
                        movedStates.emplace_back(state);
                    }
                }
            }

            if(!movedStates.empty())
            {
                const auto newPartition = pool.addPartition();

                for (const auto state : movedStates)
                {
                    pool.Partitions[partition.Id].States.erase(state);
                    pool.Partitions[newPartition].insert(state);
                }
            }
        }

        std::swap(prevPartitionMap, currPartitionMap);
        currPartitionMap = pool.makePartitionMap();
    }


    //STEP2: create new DFA from partitions

    DFA newDFA(mAlphabet);

    for (const auto& partition : pool.Partitions)
    {
        bool isStart = false;
        bool isFinal = false;

        for (const auto state : pool.Partitions[partition.Id].States)
        {
            isStart |= mStates.at(state).IsStart;
            isFinal |= mStates.at(state).IsFinal;
        }

        const auto newState = newDFA.addState(isStart, isFinal);

        for(const auto c : mAlphabet)
        {
            const auto targetState = currPartitionMap.at(mStates.at(partition.Leader).Transitions.at(c));
            newDFA.addTransition(c, newState, targetState);
        }
    }

    //STEP3: replace old DFA with new DFA
    *this = std::move(newDFA);
}

bool DFA::checkEquivalence(const ParitionMap& paritionMap, StateId stateA, StateId stateB) const
{
    for(const auto c : mAlphabet)
    {
        const auto stateADest = mStates.at(stateA).Transitions.at(c);
        const auto stateBDest = mStates.at(stateB).Transitions.at(c);

        if (stateADest != stateBDest)
        {
            if(paritionMap.at(stateADest) != paritionMap.at(stateBDest))
            {
                return false;
            }
        } 
    }

    return true;
}

} //namespace automata