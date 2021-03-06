#ifndef ETHERNET_HPP
#define ETHERNET_HPP
#include "stm32f4xx.h"
#include "internet_protocols.hpp"

extern "C" void ETH_IRQHandler(void);

class Eth {
public:
    Eth(uint8_t* rxB,uint8_t* txB);		
	//-----------------------------------------------
	FrameX frameRx{0};//memory allocation on stack
	FrameX frameTx{0};	
	IP IP_received{0};
	UDP UDP_received{0};	 
	uint8_t UDP_data[200];// for deferred reply
	TCP TCP_received{0};
	uint8_t TCP_data_transmit[1500-sizeof(IP)];
	uint8_t TCP_window_receive[0x2000]; //8192 on stack (window buffer)
	uint16_t bytesInWindow=0;
	uint16_t TCP_received_data_len{0}; //size of received data in segment
	//-----------------------------------------------
	FrameX* fRx{nullptr};
	ARP* arp_receivePtr{nullptr};
	IP* ip_receivePtr{nullptr};
	ICMP* icmp_receivePtr{nullptr};
	UDP* udp_receivePtr{nullptr};	
	TCP* tcp_receivePtr{nullptr};
//------------- SMI (PHY interface)-----------------------------------------
	uint8_t smi_read(uint8_t reg_num);
    void smi_write(uint8_t reg_num, uint8_t val);
//----------------------------------------------------------------	
	void frame_read(); //read frame to understand type of packet
//----------------- ARP ------------------------------------------
	void arp_read(); //read arp packet
	void arp_send(); //sending arp request and fill arp table
	void arp_answer(); //sending arp answer on arp request	
	void ip_read(); //read IP packet
//----------------- ICMP ---------------------------------------
	void icmp_read(); // read icmp packet
	void icmp_write(); //write to external device
//------------------- UDP ----------------------------------------	
	void udp_read();
	void udp_writeReply(uint8_t* data, uint16_t len);
	void udp_initReply();
	void udp_write(uint8_t* data, uint16_t len, uint16_t port);
//-------------------- TCP ----------------------------------
	void tcp_read();
	void tcp_initReply(uint8_t flags, uint16_t TCP_data_len);
	void tcp_reply(uint16_t TCP_data_len,bool reply);
	bool TCPconnected=false;
//---------------------------------------------------------
    void receive_frame(); //depricated
	void transmit_frame(uint16_t size); 
	
	static uint32_t ReceiveDL[4]; //descriptor list
	static uint32_t TransmitDL[4];
	bool ReceiveFlag=false;
	bool IPflag=false;
	bool arpSendAnswer=false;
	bool UDPflag = false;
	static Eth* pThis;
	
	/*!< memory allocation for ARP struct >*/
	ARP arpInit={swap16(0x0001),swap16(0x0800),0x06,0x04,swap16(0x0001),
			 	 mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ip[0],ip[1],ip[2],ip[3],
			 	 0xff,0xff,0xff,0xff,0xff,0xff, /*ip_dest*/ 5,255,255,50};	
	ARP arpSend = arpInit;			  
	uint8_t mac_receive[6]={0};
	constexpr uint16_t swap16(uint16_t val) //__attribute((always_inline))
	{return ((val>>8)&0xFF) | ((val<<8)&0xFF00);}
	constexpr uint32_t swap32(uint32_t val)
	{return (val & 0xFF)<<24 | ((val>>8) & 0xFF)<<16 | ((val>>16) & 0xFF)<<8 | ((val>>24) & 0xFF);}

private:
    void eth_init();
	void descr_init();	
		
	uint8_t ip_receive[4]={0};
	uint8_t* RxBuf;
	uint8_t* TxBuf;

	const uint8_t ip[4] = {10,42,0,200};
	const uint8_t mac[6] = {0x32,0x12,0x56,0x78,0x9a,0xbc};
	const uint8_t mac_broadcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
	const uint8_t mac_gate[6] =  {0x20,0x1a,0x06,0x7f,0xd6,0xb6};
	const uint16_t udp_port = 55555;
};

#endif //ETHERNET_HPP