
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

type_set all_types(){
    type_set types;
    for(int i=1; i<=deckSize; i++){
        types.insert(Type(i));
    }
    return types;
}

type_dist initial_distribution(){
    type_dist dist;
    for(auto t : all_types()){
        dist[t] = 1.0 / deckSize;
    }
    return dist;
}

opp_type_set all_opp_types(){
    opp_type_set opp_types;
    // 3-player game
    for(int i=1; i<=deckSize; i++){
        for(int j=1; j<=deckSize; j++){
            if(i == j || i == t.cardVal || j == t.cardVal) continue;
            OppType ot;
            if(playerID == 0){
                ot.types[1] = Type(i);
                ot.types[2] = Type(j);
            }
            if(playerID == 1){
                ot.types[0] = Type(i);
                ot.types[2] = Type(j);
            }
            if(playerID == 2){
                ot.types[0] = Type(i);
                ot.types[1] = Type(j);
            }
            opp_types.insert(ot);
        }
    }
}

opp_type_dist opposing_distribution(Type& t, int playerID){
    opp_type_dist dist;
    for(auto ot : all_opp_types()){
        dist[ot] = 1.0 / ((deckSize - 1) * (deckSize - 2));
    }
    return dist;
}