
#include "liar.h"

int choose(int n, int k){
    int prod = 1;
    for(int i=0; i<k; i++){
        prod = prod * (n - i) / (i + 1);
    }
    return prod;
}

int fact(int n){
    int prod = 1;
    for(int i=1; i<=n; i++){
        prod *= i;
    }
    return prod;
}

unordered_set<Type, TypeHash> all_types(){
    unordered_set<Type, TypeHash> types;
    int reps[deckSize];
    for(int i=0; i<deckSize; i++){
        reps[i] = 0;
    }
    reps[0] = handSize;
    while(true){
        Type t(reps);
        bool validHand = true;
        for(int i=0; i<deckSize; i++){
            if(reps[i] > deckCount){
                validHand = false;
            }
        }
        if(validHand){
            types.insert(t);
        }
        
        int firstIndex;
        for(int i=0; i<deckSize; i++){
            if(reps[i] != 0){
                firstIndex = i;
                break;
            }
        }
        if(firstIndex == deckSize - 1) break;
        // increment next by 1
        reps[firstIndex+1] ++;
        // reset all to 0
        int sum = 0;
        for(int i=0; i<=firstIndex; i++){
            sum += reps[i];
            reps[i] = 0;
        }
        reps[0] = sum-1;
    }
    return types;
}

type_dist initial_distribution(){
    type_dist dist;
    for(auto t : all_types()){
        double val = 1;
        for(int i=0; i<deckSize; i++){
            val *= choose(deckCount, t.type[i]);
        }
        dist[t] = val / choose(deckSize * deckCount, handSize);
    }
    return dist;
}

type_dist opposing_distribution(const int type[]){
    type_dist dist;
    for(auto t : all_types()){
        bool validType = true;
        for(int i=0; i<deckSize; i++){
            if(t.type[i] + type[i] > deckCount){
                validType = false;
            }
        }
        if(!validType){
            dist[t] = 0;
            continue;
        }
        double val = 1;
        for(int i=0; i<deckSize; i++){
            val *= choose(deckCount - type[i], t.type[i]);
        }
        dist[t] = val / choose(deckSize * deckCount - handSize, handSize);
    }
    return dist;
}