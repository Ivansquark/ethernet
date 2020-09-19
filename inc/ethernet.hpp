#ifndef ETHERNET_HPP
#define ETHERNET_HPP
#include "stm32f4xx.h"
#include "internet_protocols.hpp"

extern "C" void ETH_IRQHandler(void);

class Eth
{
public:
    Eth(uint8_t* rxB,uint8_t* txB);		
	FrameX frameRx{0};//memory allocation on stack
	FrameX frameTx{0};
	FrameX* fRx{nullptr};
	ARP* arp_receivePtr{nullptr};
	IP* ip_receivePtr{nullptr};
	ICMP* icmp_receivePtr{nullptr};
	uint8_t smi_read(uint8_t reg_num);
    void smi_write(uint8_t reg_num, uint8_t val);
	void frame_read(); //read frame to understand type of packet
	void arp_read(); //read arp packet
	void arp_send(); //sending arp request
	void arp_answer(); //sending arp answer on arp request	
	void ip_read(); //read IP packet
	void icmp_read(); // read icmp packet
    void receive_frame(); //depricated
	void transmit_frame(uint16_t size); 
	
	static uint32_t ReceiveDL[4];
	static uint32_t TransmitDL[4];
	bool ReceiveFlag=0;
	bool IPflag=0;
	bool arpSendAnswer=0;
	static Eth* pThis;
	
	/*!< memory allocation for ARP struct >*/
	ARP arpInit={swap16(0x0001),swap16(0x0800),0x06,0x04,swap16(0x0001),
			 	 mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ip[0],ip[1],ip[2],ip[3],
			 	 0xff,0xff,0xff,0xff,0xff,0xff, /*ip_dest*/ 192,168,0,103};	
	ARP arpSend = arpInit;			  
	uint8_t mac_recieve[6]={0};
private:
    void eth_init();
	void descr_init();	
	
	constexpr uint16_t swap16(uint16_t val) //__attribute((always_inline))
	{return ((val>>8)&0xFF) | ((val<<8)&0xFF00);}
	
	uint8_t ip_receive[4]={0};
	uint8_t* RxBuf;
	uint8_t* TxBuf;

	const uint8_t ip[4] = {192,168,0,200};
	const uint8_t mac[6] = {0x32,0x12,0x56,0x78,0x9a,0xbc};
	const uint8_t mac_broadcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
};

#endif //ETHERNET_HPP