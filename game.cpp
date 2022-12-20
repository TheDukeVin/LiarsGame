
#include "liar.h"

long getTime(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

LiarsGame::LiarsGame(){
    for(auto s : all_states()){
        for(auto t : all_types()){
            policy[s][t] = Action(s);
            value[s][t] = 0;
        }
    }
    for(auto c : {1, 2}){
        for(auto s : all_states()){
            for(auto t : all_types()){
                visitation[c][s][t] = 0;
            }
        }
    }
}

void LiarsGame::getVisitation(){
    vector<State> currStates;
    State start_state;
    currStates.push_back(start_state);
    for(auto c : {1, 2}){
        for(auto t : all_types()){
            visitation[c][start_state][t] = 1;
        }
    }
    while(!currStates.empty()){
        State s = currStates.back();
        currStates.pop_back();
        if(s.endState) continue;
        for(int i=0; i<NUM_ACTIONS; i++){
            if(!s.validAction(i)) continue;
            State next_s = s;
            next_s.makeAction(i);
            currStates.push_back(next_s);
            for(auto c : {1, 2}){
                if(s.currPlayer == c){
                    visitation[c][next_s] = visitation[c][s];
                    continue;
                }
                for(auto t : all_types()){
                    visitation[c][next_s][t] = visitation[c][s][t] * policy[s][t].move_probability(i);
                }
            }
        }
    }
}

void LiarsGame::updatePolicy(){
    updateRecurse(State());
}

void LiarsGame::updateRecurse(State& s){
    if(s.endState){
        calculateEndValue(s);
    }
    for(int i=0; i<NUM_ACTIONS; i++){
        if(!s.validAction(i)) continue;
        State next_s = s;
        next_s.makeAction(i);
        updateRecurse(next_s);
    }
    for(auto t : all_types()){
        policy[s][t] = 
    }
}

double LiarsGame::calculateEndValue(State& s){
    // calculate base value for each type the current player can have.
    for(auto t : all_types()){
        for(auto& [t_, prob] : bayesian_type(s, t)){
            // reward function may be asymmetric.
            if(s.currPlayer == 1){
                sum += prob * s.reward(t, t_);
            }
            else{
                sum += prob * s.reward(t_, t);
            }
            count ++;
        }
        value[s][t] = sum / count;
    }
}

type_dist State::bayesian_type(State& s, Type t){
    type_dist opp_type = opposing_distribution(t.type);
    double total_visitation = 0;
    for(auto& [t_, prob] : opp_type){
        total_visitation += prob * visitation[s.currPlayer][s][t_];
    }
    type_dist posterior_dist;
    for(auto& [t_, prob] : opp_type){
        posterior_dist[t_] = prob * visitation[s.currPlayer][s][t_] / total_visitation;
    }
    return posterior_dist;
}