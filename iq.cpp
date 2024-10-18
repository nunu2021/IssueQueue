#include "iq.h"
#include <iostream>
#include <cstdint>
using namespace std;

IQ::IQ() {
    std::cout << "IQ constructor" << std::endl;
}

bool IQ::isfull(){
    return false;
}

bool IQ::insert(){
    return false;
}

void IQ::wakeup(){

}

int IQ::issue(){
    return 0;
}

void IQ::flush(){

}

