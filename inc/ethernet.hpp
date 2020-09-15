#ifndef ETHERNET_HPP
#define ETHERNET_HPP
#include "stm32f4xx.h"


class Eth
{
public:
    Eth();
    uint8_t smi_read(uint8_t reg_num);
    void smi_write(uint8_t reg_num, uint8_t val);
private:
    void eth_init();    
};

#endif //ETHERNET_HPP