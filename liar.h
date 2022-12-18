
// g++ -std=c++11 main.cpp type.cpp dist.cpp state.cpp policy.cpp

#include <iostream>
#include <vector>
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

const int deckSize = 6;
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
        return val;
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
    Type type; // type of current player
    bool bets[deckSize][deckCount];
    bool endState;

    // Policy info
    int action_count[NUM_ACTIONS];
    int total_count;

    void initialize_counts(); // run this after defining state
    bool validAction(int action);

    void makeAction(int action);

    friend bool operator == (const State& t, const State& s){
        assert(!s.endState);
        assert(!t.endState);
        for(int i=0; i<deckSize; i++){
            for(int j=0; j<deckCount; j++){
                if(t.bets[i][j] != s.bets[i][j]) return false;
            }
        }
        return t.type == s.type;
    }
};

class StateHash{
public:
    size_t operator()(const State& s) const {
        assert(!s.endState);
        int M = 1000000019;
        TypeHash TH;
        int val = TH(s.type);
        for(int i=0; i<deckSize; i++){
            for(int j=0; j<deckCount; j++){
                val = (val * 2 + s.bets[i][j]) % M;
            }
        }
        return hash<int>{}(val);
    }
};

class Policy{
public:
    unordered_set<State, StateHash> states[TIME_HORIZON];

    Policy();
};

class LiarsGame{
public:
    LiarsGame();
    Policy bestResponse(Policy p);
};

#endif