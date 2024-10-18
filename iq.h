#ifndef IQ_H // include guard
#define IQ_H

#include <iostream>
#include <string>
#include <cstdint>



class IQ {

    typedef struct iq_entry {
        bool valid;
        bool rob_index[7];
        uint8_t src1;
        uint8_t src2;
        bool rdy1;
        bool rdy2;
    } iq_entry;

public:
    IQ();

    int main(){
        return 0;
    }

};

#endif /* IQ_H */