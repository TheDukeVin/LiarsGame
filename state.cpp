
#include "liar.h"

void State::initialize_counts(){
    total_count = 0;
    for(int i=0; i<NUM_ACTIONS; i++){
        if(validAction(i)){
            action_count[i] = 1;
            total_count ++;
        }
    }
}

bool State::validAction(int action){
    int high_val = -1;
    int high_count = -1;
    for(int i=0; i<deckCount; i++){
        for(int j=0; j<deckSize; j++){
            high_val = j;
            high_count = i;
        }
    }
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
}