
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
    Policy* p = new Policy();
    for(int i=0; i<TIME_HORIZON; i++){
        cout<<p->states[i].size()<<'\n';
    }
    cout<<"Successfully ran, ms:" << (getTime() - start_time) <<'\n';
    cout<<"Deleting memory\n";
    delete p;
    cout<<"Successfully closed, ms: " << (getTime() - start_time) <<'\n';
}