
#include "liar.h"

long getTime(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

Policy::Policy(){
    for(auto t : all_types()){
        // t.print();
        for(int i=0; i<(1 << (deckSize * deckCount)); i++){
            State s;
            // Initialize state info
            s.type = t;
            s.endState = false;
            int numBets = 0;
            for(int j=0; j<deckSize * deckCount; j++){
                bool bit = ((i & (1 << j)) != 0);
                s.bets[j / deckCount][j % deckCount] = bit;
                numBets += bit;
            }
            if(numBets % 2 == 0){
                s.currPlayer = 1;
            }
            else{
                s.currPlayer = 2;
            }
            long start_time = getTime();
            states[numBets].insert(s);
            // if(getTime() - start_time > 100){
            //     cout<<"LONG ONE\n";
            // }
        }
    }
}