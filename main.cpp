
#include "liar.h"
#define eps 0.0000001

void test_dist(){
    type_dist t = initial_distribution();
    double sum = 0;
    for(type_dist::iterator it = t.begin(); it != t.end(); it++){
        sum += it->second;
    }
    assert(abs(sum - 1) < eps);
    
    type_dist conditional;
    for(type_dist::iterator it = t.begin(); it != t.end(); it++){
        conditional[it->first] = 0;
    }
    for(type_dist::iterator it = t.begin(); it != t.end(); it++){
        type_dist t2 = opposing_distribution(it->first.type);
        double sum = 0;
        for(type_dist::iterator it2 = t2.begin(); it2 != t2.end(); it2++){
            sum += it2->second;
            conditional[it2->first] += it2->second * it->second;
        }
        assert(abs(sum - 1) < eps);
    }
    for(type_dist::iterator it = t.begin(); it != t.end(); it++){
        assert(abs(conditional[it->first] - it->second) < eps);
    }
}

int main(){
    test_dist();

    long start_time = getTime();
    LiarsGame* LG = new LiarsGame();
    cout<<"Game constructed, time="<<(getTime() - start_time)<<'\n';
    LG->getVisitation();
    cout<<"Calculated visitation, time="<<(getTime() - start_time)<<'\n';

    State s0;
    State s1 = s0;
    s1.makeAction(3);
    State s2 = s1;
    s2.makeAction(7);
    for(auto t : all_types()){
        cout<<LG->visitation[1][s0][t]<<' '<<LG->visitation[2][s0][t]<<' ';
        cout<<LG->visitation[1][s1][t]<<' '<<LG->visitation[2][s1][t]<<' ';
        cout<<LG->visitation[1][s2][t]<<' '<<LG->visitation[2][s2][t]<<'\n';
    }
    delete LG;
    cout<<"Game destructed, time="<<(getTime() - start_time)<<'\n';
}