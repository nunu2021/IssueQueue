#include <cstdint>

typedef struct iq_entry {
    bool valid;
    bool rob_index[7];
    uint8_t src1;
    uint8_t src2;
    bool rdy1;
    bool rdy2;
} iq_entry;

class iq {
private:
    iq_entry entries[64];

public:
    /* Constructor: initializes new queue */
    iq();

    /**
     * Checks whether the queue is full
     * 
     * returns true if no free entries exist in the IQ
     */
    bool isfull();

    bool insert();

    void wakeup();

    int issue();

    void flush();
};
