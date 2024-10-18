#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    // Currently iterative, but pretend it's parallel
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
    // Currently iterative, but pretend it's parallel
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
    // Software implementation, doesn't really work in hardware
    iq_entry to_issue[ISSUE_WIDTH];
    int count = 0;
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (entries[i].valid && entries[i].rdy1 && entries[i].rdy2) {
            to_issue[count] = entries[i];
            count++;
            if (count >= ISSUE_WIDTH) {
                break;
            }
        }
    }
    return count;
}

void IQ::flush(){
    // Currently iterative, but pretend it's parallel
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
}

