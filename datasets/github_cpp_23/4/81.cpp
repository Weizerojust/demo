







#include <stdio.h>
#include <iostream>

void moveDisks(int count, int needle1, int needle3, int needle2);


int main(int argc, const char * argv[]){

     moveDisks(4, 1, 2, 3);
    
}

void moveDisks(int count, int needle1, int needle3, int needle2){
    if(count>0){
        moveDisks(count-1, needle1, needle2, needle3);
        std::cout<<"Move disk "<<count<<" from "<<needle1<<" to "<< needle3<<"."<<std::endl;
        moveDisks(count-1, needle2, needle3, needle1);
    }
}


