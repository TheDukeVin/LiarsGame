
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

// Liar's dice

const int deckSize = 5;
const int deckCount = 3;
const int handSize = 4;
const int TIME_HORIZON = deckSize * deckCount + 1;
const int NUM_ACTIONS = deckSize * deckCount + 1;

const int CALL = deckSize * deckCount;

long getTime();

class Type{
public:
    int type[deckSize];

    Type(){}
    Type(int* t);
    void const print() const;

    friend bool operator == (const Type& t, const Type& s){
        for(int i=0; i<deckSize; i++){
            if(t.type[i] != s.type[i]) return false;
        }
        return true;
    }
};

class TypeHash{
public:
    size_t operator()(const Type& t) const {
        int M = 1000000019;
        int val = 0;
        for(int i=0; i<deckSize; i++){
            val = (val * 10 + t.type[i]) % M;
        }
        return hash<int>{}(val);
    }
};

typedef unordered_map<Type, double, TypeHash> type_dist;

int choose(int n, int k);
int fact(int n);

unordered_set<Type, TypeHash> all_types();
type_dist initial_distribution();
type_dist opposing_distribution(const int type[]);

class State{
public:
    // State info
    int currPlayer; // 1 = maximizing, 2 = minimizing
    //bet[i][j] is betting there are at least (j+1) cards of value i
    bool bets[deckSize][deckCount];
    bool endState; // at the end state, currPlayer is the player who calls.

    State();
    int topBet();
    bool validAction(int action);
    void makeAction(int action);
    // Note: this reward function is symmetric. Test cases when it is not symmetric.
    double reward(Type t1, Type t2);

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
        int M = 1000000019;
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
public:
    unordered_map<State, unordered_map<Type, Action, TypeHash>, StateHash> policy;
    // visitation[c][s][t] = probability of reaching state s given
    // player 3-c has type t, player 3-c follows p
    // player c follows path to s
    unordered_map<State, unordered_map<Type, double, TypeHash>, StateHash> visitation[3];
    // value[c][s][t] = value of state s given
    // player 3-c has bayesian type, player 3-c follows p
    // player c has type t, player c plays best response against p
    unordered_map<State, unordered_map<Type, double, TypeHash>, StateHash> value[3];

    LiarsGame();
    void getVisitation();
    void updatePolicy();

private:
    void updateRecurse(State& s);
    void calculateEndValue(State& s);
    type_dist bayesian_type(State& s, Type t);
};

#endif