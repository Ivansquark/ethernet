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
    __enable_irq();
    
    //uint8_t y=0;
    eth.arp_send();
    for(uint32_t i=0;i<50000000;i++);
    eth.frame_read();
    for(uint8_t i=0;i<6;i++)
    {uart.sendByte(eth.mac_recieve[i]);eth.mac_recieve[i]=0;}  
    eth.ReceiveFlag=false;
    while(1)
    {
        //eth.arp_send();
        //for(uint32_t i=0;i<150000000;i++);
        //eth.arp_read();
        
        //for(uint8_t i=0;i<6;i++)
        //{uart.sendByte(eth.mac_recieve[i]);}
        //eth.arp_read();                
	           
        //Eth::pThis->ReceiveDL[0] |= (1<<31); //sets OWN bit to DMA     
        if(eth.ReceiveFlag)
        {
            eth.frame_read();
            if(eth.UDPflag)
            {                
                for(uint8_t i=0;i<(eth.swap16(eth.udp_receivePtr->len)-sizeof(UDP));i++)
                {
                    uart.sendByte(*((uint8_t*)(eth.udp_receivePtr+1)+i));
                }
                eth.UDPflag=false;
            }
            eth.ReceiveFlag=false;
            Eth::pThis->ReceiveDL[0] |= (1<<31); //sets OWN bit to DMA
            //uart.sendStr("opa");            
        }
        else
        {
             
        }

            
        
    }
    return 0;
}
