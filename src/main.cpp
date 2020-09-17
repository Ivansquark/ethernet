#include "main.h"


//void * __dso_handle = 0;
//void * __cxa_guard_acquire = 0;
uint8_t RxBuf[2048]; //need to be aligned on four
uint8_t TxBuf[2048];

int main()
{
    PD_led led;//RCC config
    Uart6 uart;
    SpiLcd lcd;
    Font_16x16 font;    
    font.fillScreen(0xff00);
    Eth eth(RxBuf,TxBuf);
	uint8_t x=eth.smi_read(0);
    font.intToChar(x);
    font.print(10,50,0x00ff,font.arr,0);
    eth.receive_frame();
	for(uint8_t i=0;i<60;i++)    
    
    //uint8_t y=0;
    while(1)
    {
        eth.arp_send();
        eth.arp_read();
        
        //for(uint8_t i=0;i<6;i++)
        //{uart.sendByte(eth.mac_recieve[i]);}
        //eth.arp_read();                
	    //for(uint8_t i=0;i<60;i++)
        //{uart.sendByte(RxBuf[i]);}               
        if(eth.arpReceiveFlag)
        {
            //eth.arp_read();
            //if(eth.arp_recievePtr->ip_dst[0]==eth.ip[0] && eth.arp_recievePtr->ip_dst[1]==eth.ip[1] &&
            //   eth.arp_recievePtr->ip_dst[2]==eth.ip[0] && eth.arp_recievePtr->ip_dst[1]==eth.ip[3])
            //{
            //    eth.arp_answer();
            //}
            eth.arpReceiveFlag=false;
            uart.sendStr("opa");
        }
    }
    return 0;
}
