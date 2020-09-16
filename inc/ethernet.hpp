#ifndef ETHERNET_HPP
#define ETHERNET_HPP
#include "stm32f4xx.h"


class Eth
{
public:
    Eth();
	
	struct TCP_stack
	{
		
		
	};
	
    uint8_t smi_read(uint8_t reg_num);
    void smi_write(uint8_t reg_num, uint8_t val);
	void receive_frame();
	void transmit_frame();
	static uint32_t RecieveDL[4];
	static uint32_t TransmitDL[4];
private:
    void eth_init();
	void descr_init(uint32_t TxAddr,uint32_t RxAddr);
};

#endif //ETHERNET_HPP