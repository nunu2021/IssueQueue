#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
}

bool IQ::isfull(){
    return false;
}

bool IQ::insert(){
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
    return 0;
}

void IQ::flush(){
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
}

