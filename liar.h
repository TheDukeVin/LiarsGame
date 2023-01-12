
// g++ -O2 -std=c++17 main.cpp type.cpp dist.cpp state.cpp action.cpp game.cpp

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <ctime>
#include <chrono>
#include <sys/time.h>

#ifndef liar_h
#define liar_h

using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

// Dummy's poker

/*
3-player game. Each player gets one card.
Each player puts 1 chip in the pot. 
Players can call/check or raise the pot (by at least double) or fold their card. 
Player with highest card wins the pot.
*/

const int NUM_AGENT = 3;

const int deckSize = 6;
const int maxBet = 10;
// TIME_HORIZON = maximum game length.
// Here, TIME_HORIZON = NUM_AGENT * (1 + ceil log2(maxBet))
const int TIME_HORIZON = 15;

/*
Actions:
0 = fold
1 = call
k>1 = raise to k
*/
const int NUM_ACTIONS = maxBet + 1;

// large prime for hashing
const int M = 1000000019;

long getTime();

class Type{
public:
    int cardVal;

    Type(){}
    Type(int val);
    void const print() const;

    friend bool operator == (const Type& t, const Type& s){
        return t.cardVal == s.cardVal;
    }
};

class TypeHash{
public:
    size_t operator()(const Type& t) const {
        return hash<int>{}(t.cardVal);
    }
};

// a class to describe the state of opponent's types.
class OppType{
public:
    Type types[NUM_AGENT];
    int playerID;

    friend bool operator == (const OppType& t, OppType& s){
        assert(t.playerID == s.playerID);
        for(int i=0; i<NUM_AGENT; i++){
            if(i == playerID) continue;
            if(!(t.types[i] == s.types[i])){
                return false;
            }
        }
        return true;
    }
}

class OppHash{
    size_t operator()(const OppType& t) const {
        long val = 0;
        for(int i=0; i<NUM_AGENT; i++){
            if(i == playerID) continue;
            val = (val * 54321 + TypeHash(types[i])) % M;
        }
        return val;
    }
    
}

typedef unordered_map<Type, double, TypeHash> type_dist;
typedef unordered_set<Type, TypeHash> type_set;
typedef unordered_map<OppType, double, OppHash> opp_type_dist;
typedef unordered_set<OppType, OppHash> opp_type_set;

int choose(int n, int k);
int fact(int n);

type_set all_types();
type_dist initial_distribution();
opp_type_set all_opp_types();
opp_type_dist opposing_distribution(Type& t, int playerID);

class State{
private:
    int committed_chips[NUM_AGENT];
    bool folded[NUM_AGENT];
    int currTime;

    const int FOLD = 0;
public:
    // State info
    int currPlayer; // 0, 1, 2.

    /*
    Events:
    Positive number = bet of that size (starts at 1).
    0 = fold
    -1 = not yet set.
    */
    int event_seq[TIME_HORIZON];
    bool endState;

    State();
    int topBet();
    bool validAction(int action);
    void makeAction(int action);

    double reward[NUM_AGENT];
    Type revealed_types[NUM_AGENT];
    void getReward(); // reads revealed_types, modifies reward array to represent rewards.

    string toString();

    friend bool operator == (const State& t, const State& s){
        // assert(!s.endState);
        // assert(!t.endState);
        for(int i=0; i<deckSize; i++){
            for(int j=0; j<deckCount; j++){
                if(t.bets[i][j] != s.bets[i][j]) return false;
            }
        }
        assert(t.currPlayer == s.currPlayer);
        return true;
    }
};

class StateHash{
public:
    size_t operator()(const State& s) const {
        // assert(!s.endState);
        // int val = 0;
        int val = s.endState;
        for(int i=0; i<deckSize; i++){
            for(int j=0; j<deckCount; j++){
                val = (val * 2 + s.bets[i][j]) % M;
            }
        }
        return hash<int>{}(val);
    }
};

unordered_set<State, StateHash> all_states();

class Action{
public:
    double action_count[NUM_ACTIONS];
    double total_count;

    Action(){}
    Action(State& s);
    double move_probability(int a);
};

class LiarsGame{
private:
    int currPlayer;
public:
    // store policies of all players
    unordered_map<State, unordered_map<Type, Action, TypeHash>, StateHash> policy;
    // visitation[i][s][t] = probability of reaching state s given
    // player -i has type t, player -i follows p
    // player i follows path to s
    unordered_map<State, unordered_map<OppType, double, OppHash>, StateHash> visitation[NUM_AGENT];
    // value[s][t] = value of state s given
    // i = currPlayer
    // player -i has bayesian type, player -i follows p
    // player i has type t, player i plays best response against p
    unordered_map<State, unordered_map<Type, double, TypeHash>, StateHash> value;

    LiarsGame();
    void getVisitation();
    // calculate value and find best response, update policy
    void updatePolicy();

    //combine above two functions
    void improvePolicy(int playerID);

private:
    void updateRecurse(State& s);
    void calculateEndValue(State& s);

    // at state s given
    // player i has type t
    // what is the distribution of opposing types
    opp_type_dist bayesian_type(State& s, Type t);
};

#endif