#include <cstdint>

typedef struct iq_entry {
    bool valid;
    bool rob_index[7];
    uint8_t src1;
    uint8_t src2;
    bool rdy1;
    bool rdy2;
} iq_entry;