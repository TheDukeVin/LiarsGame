
#include "liar.h"

State::State(){
    currPlayer = 1;
    for(int i=0; i<deckSize; i++){
        for(int j=0; j<deckCount; j++){
            bets[i][j] = false;
        }
    }
    endState = false;
}

int State::topBet(){
    int high_val = -1;
    int high_count = -1;
    for(int i=0; i<deckCount; i++){
        for(int j=0; j<deckSize; j++){
            if(bets[j][i]){
                high_val = j;
                high_count = i;
            }
        }
    }
    return high_count * deckSize + high_val;
}

bool State::validAction(int action){
    int top = topBet();
    int high_count = top / deckSize;
    int high_val = top % deckSize;
    if(action == CALL){
        return high_val != -1;
    }
    int act_val = action / deckCount;
    int act_count = action % deckCount;
    if(act_count == high_count){
        return act_val > high_val;
    }
    return act_count > high_count;
}

void State::makeAction(int action){
    if(action == CALL){
        endState = true;
        return;
    }
    int act_val = action / deckCount;
    int act_count = action % deckCount;
    bets[act_val][act_count] = true;
    currPlayer = 3 - currPlayer;
    if(act_val == deckSize - 1 && act_count == deckCount - 1){
        endState = true;
    }
}

string State::toString(){
    string s = "EndState: " + to_string(endState) + '\n';
    for(int i=0; i<deckSize; i++){
        for(int j=0; j<deckCount; j++){
            s += to_string(bets[i][j]);
        }
        s += '\n';
    }
    return s;
}

unordered_set<State, StateHash> all_states(){
    unordered_set<State, StateHash> states;
    vector<State> currStates;
    currStates.push_back(State());
    while(!currStates.empty()){
        State s = currStates.back();
        states.insert(s);
        currStates.pop_back();
        if(s.endState){
            continue;
        }
        int numValidActions = 0;
        for(int i=0; i<NUM_ACTIONS; i++){
            if(s.validAction(i)){
                State next_s = s;
                next_s.makeAction(i);
                currStates.push_back(next_s);
                numValidActions++;
            }
        }
        assert(numValidActions > 1);
    }
    return states;
}

double State::reward(Type t1, Type t2){
    int top = topBet();
    int high_count = top / deckSize;
    int high_val = top % deckSize;

    int winner;
    if(high_count > t1.type[high_val] + t2.type[high_val]){
        winner = currPlayer;
    }
    else{
        winner = 3 - currPlayer;
    }
    if(winner == 1){
        return 1;
    }
    return -1;
}

// Iterating through states:

// vector<State> currStates;
// currStates.push_back(State());
// while(!currStates.empty()){
//     State s = currStates.back();
//     currStates.pop_back();
//     if(s.endState){
//         continue;
//     }
//     for(int i=0; i<NUM_ACTIONS; i++){
//         if(s.validAction(i)){
//             State next_s = s;
//             next_s.makeAction(i);
//             currStates.push_back(next_s);
//         }
//     }
// }