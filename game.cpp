
#include "liar.h"

long getTime(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

LiarsGame::LiarsGame(){
    for(auto s : all_states()){
        for(auto t : all_types()){
            policy[s][t] = Action(s);
        }
    }
    for(auto s : all_states()){
        for(auto ot : all_opp_types()){
            visitation[currPlayer][s][ot] = 0;
        }
        for(auto t : all_types()){
            value[s][t] = 0;
        }
    }
}

void LiarsGame::getVisitation(){
    vector<State> currStates;
    State start_state;
    currStates.push_back(start_state);
    for(auto ot : all_opp_types()){
        visitation[currPlayer][start_state][ot] = 1;
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
            if(s.currPlayer == currPlayer){
                visitation[currPlayer][next_s] = visitation[currPlayer][s];
            }
            else{
                for(auto ot : all_opp_types()){
                    visitation[currPlayer][next_s][ot] = visitation[currPlayer][s][ot] * policy[s][ot.types[s.currPlayer]].move_probability(i);
                }
            }
        }
    }
}

void LiarsGame::updatePolicy(){
    updateRecurse(State());
}

void LiarsGame::improvePolicy(int playerID){
    currPlayer = playerID;
    getVisitation();
    updatePolicy();
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
    if(currPlayer == s.currPlayer){
        value[s][t] = 
    }
    for(auto t : all_types()){
        double sum;
        for(auto& [ot, prob] : bayesian_type(s, t)){
            sum += prob * 
        }
        policy[s][t] = 
    }
}

double LiarsGame::calculateEndValue(State& s){
    // calculate base value for each type the current player can have.
    for(auto t : all_types()){
        double sum = 0;
        for(auto& [ot, prob] : bayesian_type(s, t)){
            for(int i=0; i<NUM_AGENT; i++){
                if(i == currPlayer){
                    s.revealed_types[i] = t;
                }
                else{
                    s.revealed_types[i] = ot.types[i];
                }
            }
            sum += prob * s.getReward();
        }
        value[s][t] = sum;
    }
}

opp_type_dist State::bayesian_type(State& s, Type t){
    opp_type_dist opp_type = opposing_distribution(t.type);
    double total_visitation = 0;
    for(auto& [ot, prob] : opp_type){
        total_visitation += prob * visitation[currPlayer][s][ot];
    }
    opp_type_dist posterior_dist;
    for(auto& [ot, prob] : opp_type){
        posterior_dist[ot] = prob * visitation[currPlayer][s][ot] / total_visitation;
    }
    return posterior_dist;
}