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

    private:
        iq_entry entries[64];
    public:
        IQ();
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

#endif /* IQ_H */
