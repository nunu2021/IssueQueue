#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    IQ_occupancy = 0;

    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
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

void IQ::wakeup(uint8_t produced_reg){
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (entries[i].src1 == produced_reg) {
            entries[i].rdy1 = true;
        }
        if (entries[i].src2 == produced_reg) {
            entries[i].rdy2 = true;
        }
    }
}

int IQ::issue(){
    int issued = 0;

    IQ_occupancy-=issued;
    return 0;
}

void IQ::flush(){
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
    IQ_occupancy = 0;
}

