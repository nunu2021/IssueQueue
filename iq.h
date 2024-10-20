#ifndef IQ_H // include guard
#define IQ_H

#include <iostream>
#include <string>
#include <cstdint>

#define ISSUE_WIDTH 4
#define NUM_ENTRIES 64

class IQ {
    typedef struct iq_entry {
        bool valid;
        bool rob_index[7];
        uint8_t src1;
        uint8_t src2;
        bool rdy1;
        bool rdy2;
    } iq_entry;

private:
    iq_entry entries[NUM_ENTRIES];
    long long int IQ_occupancy; // the occupancy of the IQ


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
     * 
     * 
     * Return: false if insertion was unsuccessful
     */
    bool insert();

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
     * Parameters:
     * 
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
