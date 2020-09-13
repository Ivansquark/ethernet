#ifndef ETHERNET_HPP
#define ETHERNET_HPP
#include "stm32f4xx.h"


class Eth
{
public:
    Eth();
private:
    void eth_init();    
};

#endif //ETHERNET_HPP