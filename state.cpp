
#include "liar.h"

State::State(){
    currPlayer = 0;
    for(int i=0; i<TIME_HORIZON; i++){
        event_seq[i] = -1;
    }
    endState = false;
    for(int i=0; i<NUM_AGENT; i++){
        committed_chips[i] = 1;
        folded[i] = false;
    }
    currTime = 0;
}

int State::topBet(){
    for(int i=currTime-1; i>=0; i--){
        if(event_seq[i] != 0){
            return event_seq[i];
        }
    }
    return 1;
}

bool State::validAction(int action){
    int top = topBet();
    if(action == FOLD){
        return committed_chips[currPlayer] < top;
    }
    return action == top || action >= top * 2 || action == maxBet;
}

void State::makeAction(int action){
    event_seq[currTime] = action;
    if(action == 0){
        folded[currPlayer] = true;
    }
    else{
        committed_chips[currPlayer] = action;
    }
    currTime ++;
    // check for endState
    endState = true;
    for(int i=0; i<NUM_AGENT; i++){
        if(!folded[i] && committed_chips[i] != action){
            endState = false;
        }
    }
    if(action == 1 && currPlayer != NUM_AGENT - 1){ // edge case where no bets are made at the beginning
        endState = false;
    }
    if(endState){
        return;
    }
    for(int player = currPlayer + 1; player < currPlayer + NUM_AGENT; player++){
        if(!folded[player % NUM_AGENT]){
            currPlayer = player % NUM_AGENT;
            return;
        }
    }
    assert(false);
}

string State::toString(){
    string s = "Events:\n";
    for(int i=0; i<currTime; i++){
        s += to_string(event_seq[i]) + ' ';
        if(i % 3 == 2){
            s += '\n';
        }
    }
    s += '\n';
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

void State::getReward(){
    int maxVal = 0;
    int winner = -1;
    for(int i=0; i<NUM_AGENT; i++){
        if(!folded[i]){
            if(maxVal < revealed_types[i]){
                maxVal = revealed_types[i];
                winner = i;
            }
        }
    }
    assert(winner != -1);
    int pot = 0;
    for(int i=0; i<NUM_AGENT; i++){
        pot += committed_chips[i];
    }
    for(int i=0; i<NUM_AGENT; i++){
        reward[i] = -committed_chips[i];
        if(i == winner){
            reward[i] += pot;
        }
    }
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