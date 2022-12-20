
#include "liar.h"

Action::Action(State& s){
    total_count = 0;
    for(int i=0; i<NUM_ACTIONS; i++){
        if(s.validAction(i)){
            action_count[i] = 1;
            total_count ++;
        }
        else{
            action_count[i] = 0;
        }
    }
}

double Action::move_probability(int a){
    return action_count[a] / total_count;
}