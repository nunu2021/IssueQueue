#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }

    arbitration_head = initializeArbiters(0, NUM_ENTRIES);
    arbiter_depth = calculateDepth(arbitration_head);
}

bool IQ::isfull(){
    if (IQ_occupancy == 64) {
        return true;
    }
    return false;
}

bool IQ::insert(bool in_rob_idx[7], uint8_t in_src1, uint8_t in_src2,
            bool in_rdy1, bool in_rdy2){
    if (IQ_occupancy < 64) {
        IQ_occupancy++;
        // TODO: insert the instruction into the IQ
        IQ::arbiter* current = arbitration_head;
        for (int i = 0; i < arbiter_depth - 1; i++) {
            for (int j = 0; j < ARBITER_WIDTH; j++) {
                if (current->children[j]->free > 0) {
                    current->free--;
                    current = current->children[j];
                    break;
                }
            }
        }

        int start = current->start_index;
        for (int i = 0; i < ARBITER_WIDTH; i++) {
            if (!entries[start + i].valid) {
                entries[start + i].valid = true;
                for (int j = 0; j < 7; j++) {
                    entries[start + i].rob_index[j] = in_rob_idx[j];
                }
                entries[start + i].src1 = in_src1;
                entries[start + i].src2 = in_src2;
                entries[start + i].rdy1 = in_rdy1;
                entries[start + i].rdy2 = in_rdy2;

                current->free--;
                break;
            }
        }
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
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }
    IQ_occupancy = 0;
}

IQ::arbiter* IQ::initializeArbiters(int start, int end) {
    IQ::arbiter* arbiter = new IQ::arbiter;
    arbiter->start_index = start;
    arbiter->end_index = end;
    arbiter->free = end - start;
    arbiter->ready = 0;
    if (end - start > ARBITER_WIDTH) {
        int interval = (end - start) / ARBITER_WIDTH; // Assume it divides
        for (int i = 0; i < ARBITER_WIDTH; i++) {
            arbiter->children[i] = initializeArbiters(start + i * interval,
                                                start + (i + 1) * interval);
        }
    }else {
        for (int i = 0; i < ARBITER_WIDTH; i++) {
            arbiter->children[i] = nullptr;
        }
    }
}

int IQ::calculateDepth(IQ::arbiter* head) {
    if (head == nullptr) {
        return 0;
    }
    int depth = 1;
    while (head->children[0] != nullptr) {
        head = head->children[0];
        depth++;
    }
    return depth;
}
