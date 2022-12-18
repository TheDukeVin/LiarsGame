
#include "liar.h"

Type::Type(int* t){
    for(int i=0; i<deckSize; i++){
        type[i] = t[i];
    }
}

void const Type::print() const{
    cout<<"TYPE: ";
    for(int i=0; i<deckSize; i++){
        cout<<type[i]<<' ';
    }
    cout<<'\n';
}

