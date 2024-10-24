#ifndef IQ_H // include guard
#define IQ_H

#include <cstdint>

#define ISSUE_WIDTH 4
#define NUM_ENTRIES 64
#define ARBITER_WIDTH 4

class IQ {
    typedef struct iq_entry {
        bool valid;
        bool rob_index[7];
        uint8_t src1;
        uint8_t src2;
        bool rdy1;
        bool rdy2;
    } iq_entry;

    typedef struct arbiter {
        unsigned int free;
        unsigned int ready;
        int start_index; // inclusive
        int end_index; // exclusive
        int ready_indices[ISSUE_WIDTH];
        struct arbiter* children[ARBITER_WIDTH];
    } arbiter;

private:
    iq_entry entries[NUM_ENTRIES];
    arbiter* arbitration_head;
    
    arbiter* initializeArbiters(int start, int end);
    void clearArbiters(arbiter* head);
    void updateArbiters(arbiter* head);

public:
    IQ();

    /**
     * Checks whether the queue is full
     * 
     * Return: true if no free entries exist in the IQ
     */
    bool isfull();

    /**
     * Adds an element to the IQ
     * 
     * Parameters:
     *  - in_rob_idx: rob index of the instruction to insert
     *  - in_src1: first source register number
     *  - in_src2: second source register number
     *  - in_rdy1: whether the first source is ready
     *  - in_rdy2: whether the second source is ready
     * 
     * Return: false if insertion was unsuccessful
     */
    bool insert(bool in_rob_idx[7], uint8_t in_src1, uint8_t in_src2,
            bool in_rdy1, bool in_rdy2);

    /**
     * Wakes up entries when a physical register value has been produced
     * 
     * Parameters:
     *  - produced_reg: address of register whose value has been produced
     * 
     */
    void wakeup(uint8_t produced_reg);

    /**
     * Selects up to 4 entries for execution and issues them
     * 
     * Attempts to extend the design from the paper to issue 4 at a time
     * using a modification of the arbiter, which passes a count of free
     * and ready instructions instead of if any request is ready, as well
     * as up to 4 indices of entries which can be issued. At every level,
     * the arbiter attempts to pick the first four ready indices of the 16
     * it receives from its children to pass towards the root.
     * 
     * Return: number of instructions successfully selected
     */
    int issue();

    /**
     * Flushes all pending instructions in the IQ
     */
    void flush();

};

#endif /* IQ_H */
