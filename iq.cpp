#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    // Currently iterative, but pretend it's parallel
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

    IQ_occupancy -= count;
    return count;
}

void IQ::flush(){
    // Currently iterative, but pretend it's parallel
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
    IQ_occupancy = 0;
}

