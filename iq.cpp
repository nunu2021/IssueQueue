#include "iq.h"
#include <cstdint>
using namespace std;

IQ::IQ() {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }

    arbitration_head = initializeArbiters(0, NUM_ENTRIES);
}

bool IQ::isfull(){
    if (arbitration_head->free > 0) {
        return true;
    }
    return false;
}

bool IQ::insert(bool in_rob_idx[7], uint8_t in_src1, uint8_t in_src2,
            bool in_rdy1, bool in_rdy2){
    if (arbitration_head->free > 0) {
        IQ::arbiter* current = arbitration_head;
        // Hardware analogue: pass signal from leaves to parent
        while(current->children[0] != nullptr) {
            // Hardware analogue: priority encoders
            for (int j = 0; j < ARBITER_WIDTH; j++) {
                if (current->children[j]->free > 0) {
                    current->free--;
                    current = current->children[j];
                    break;
                }
            }
        }

        int start = current->start_index;
        // Hardware analogue: priority encoders
        for (int i = 0; i < ARBITER_WIDTH; i++) {
            if (!entries[start + i].valid) {
                entries[start + i].valid = true;
                // Hardware analogue: parallel copy
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
    } else {
        return false;
    }
}

void IQ::wakeup(uint8_t produced_reg){
    // Hardware analogue: parallel checks
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (entries[i].src1 == produced_reg) {
            entries[i].rdy1 = true;
        }
        if (entries[i].src2 == produced_reg) {
            entries[i].rdy2 = true;
        }
    }

    updateArbiters(arbitration_head);
}

int IQ::issue(){
    // Software implementation, doesn't really work in hardware
    iq_entry to_issue[ISSUE_WIDTH];
    int count = arbitration_head->ready;
    for (int i = 0; i < arbitration_head->ready; i++) {
        to_issue[i] = entries[arbitration_head->ready_indices[i]];
        entries[arbitration_head->ready_indices[i]].valid = false;
    }

    updateArbiters(arbitration_head);

    return count;
}

void IQ::flush(){
    // Hardware analogue: parallel copy
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i].valid = false;
    }

    clearArbiters(arbitration_head);
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
    return arbiter;
}

void IQ::clearArbiters(IQ::arbiter* head) {
    head->free = head->end_index - head->start_index;
    head->ready = 0;
    // Hardware analogue: pass signal from root to children
    for (int i = 0; i < ARBITER_WIDTH; i++) {
        if (head->children[i] != nullptr) {
            clearArbiters(head->children[i]);
        }
    }
}

void IQ::updateArbiters(IQ::arbiter* head) {
    int free_count = 0;
    int ready_count = 0;
    // Hardware analogue: adder units
    for (int i = 0; i < ARBITER_WIDTH; i++) {
        if (head->children[i] != nullptr) {
            // Hardware analogue: pass signal from leaves to parent
            updateArbiters(head->children[i]);
            free_count += head->children[i]->free;
            ready_count += head->children[i]->ready;

        } else {
            if (!entries[head->start_index + i].valid) {
                free_count++;
            } else if (entries[head->start_index].rdy1
                    && entries[head->start_index].rdy2) {
                // Hardware analogue: mux
                head->ready_indices[ready_count] = head->start_index + i;
                ready_count++;
            }
        }
    }

    head->free = free_count;
    head->ready = ready_count;

    // Hardware analogue: specialized circuit for leaves
    if (head->children[0] == nullptr) {
        return;
    }

    int to_issue = free_count;
    if (free_count > ISSUE_WIDTH) {
        to_issue = ISSUE_WIDTH;
    }
    // Hardware analogue: complicated circuit copied multiple times
    // Goal: compress up to 16 array entries into up to 4 entries
    for (int i = 0; i < to_issue; i++) {
        int count = 0;
        for (int j = 0; j < ARBITER_WIDTH; j++) {
            if (head->children[j]->ready + count > i) {
                head->ready_indices[i] = head->children[j]
                        ->ready_indices[i - count];
                break;
            } else {
                count += head->children[j]->ready;
            }
        }
    }

    
}
