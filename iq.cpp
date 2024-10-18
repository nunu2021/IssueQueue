#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    IQ_occupancy = 0;

}

bool IQ::isfull(){
    if (IQ_occupancy == 64) {
        return true;
    }
    return false;
}

bool IQ::insert(){
    if (IQ_occupancy < 64) {
        IQ_occupancy++;
    // TODO: insert the instruction into the IQ
        return true;
    }
    return false;
}

void IQ::wakeup(){

}

int IQ::issue(){
    return 0;
}

void IQ::flush(){

}

