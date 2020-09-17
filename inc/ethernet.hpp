#ifndef ETHERNET_HPP
#define ETHERNET_HPP
#include "stm32f4xx.h"

extern "C" ETH_IRQHandler();

struct ARP
{
	uint16_t net;
	uint16_t protocol;
	uint8_t mac_len;
	uint8_t ip_len;
	uint16_t op; //1-request, 2-answer
	uint8_t macaddr_src[6];
	uint8_t ip_src[4];
	uint8_t macaddr_dst[6];
	uint8_t ip_dst[4];
};
struct FrameRx
{
	uint8_t mac_src[6];
	uint8_t mac_dest[6];
	uint16_t type;
};
struct FrameTx
{
	uint8_t mac_source[6];
	uint8_t mac_destination[6];
	uint16_t type;
};

class Eth
{
public:
    Eth(uint8_t* rxB,uint8_t* txB);	
	ARP arp_send={swap16(0x0001),swap(0x0800),0x06,0x04,swap(0x0001),
			 mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ip[0],ip[1],ip[2],ip[3],ip[4],
			 0xff,0xff,0xff,0xff,0xff,0xff, /*ip_dest*/ 192,168,0,103};
	FrameRx frameRx{0};
	FrameTx frameTx{0};
	ARP* arp_recievePtr{nullptr};
	void arp_read();
	void arp_send();
	void arp_answer();	
    uint8_t smi_read(uint8_t reg_num);
    void smi_write(uint8_t reg_num, uint8_t val);
	void receive_frame();
	void transmit_frame(uint16_t size);
	void descr_init(uint8_t* TxAddr,uint8_t* RxAddr);
	static uint32_t ReceiveDL[4];
	static uint32_t TransmitDL[4];
	bool arpReceiveFlag=0;
	bool arpSendAnswer=0;
	static Eth* pThis;
	static constexpr uint8_t ip[4] = {192,168,0,200};
	static constexpr uint8_t mac[6] = {0x32,0x12,0x56,0x78,0x9a,0xbc};
	static constexpr uint8_t mac_broadcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
private:
    void eth_init();
	constexpr uint16_t swap16(uint16_t val) //__attribute((always_inline))
	{
		return ((val>>8)&0xFF) | ((val<<8)&0xFF00);
	}
	
	uint8_t mac_recieve[6]={0};
	uint8_t ip_receive[4];
	uint8_t* RxBuf;
	uint8_t* TxBuf;
};

#endif //ETHERNET_HPP