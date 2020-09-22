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
    GpTimer tim(2);//500 ms

    __enable_irq();
    
    //uint8_t y=0;
    eth.arp_send();
    for(uint32_t i=0;i<90000000;i++);
    eth.frame_read();
    for(uint8_t i=0;i<6;i++)
    {uart.sendByte(eth.mac_receive[i]);/*eth.mac_receive[i]=0;*/}  
    eth.ReceiveFlag=false;
    eth.icmp_write();
    uint32_t num=0xFFFFFFF0;
    while(1)
    {
        if(GpTimer::timFlag) {
            num++;
            if(eth.TCPconnected){
                //eth.swap32()
                uart.sendByte(num);
                eth.TCP_data_transmit[0] = (uint8_t)(num>>24);
                eth.TCP_data_transmit[1] = (uint8_t)(num>>16);
                eth.TCP_data_transmit[2] = (uint8_t)(num>>8);
                eth.TCP_data_transmit[3] = (uint8_t)(num);
                //eth.TCP_received.fl = 0;
                eth.tcp_reply(4,false);
            }
            GpTimer::timFlag = false;
        }

        if(eth.ReceiveFlag) {
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
        else {}
        if(eth.TCP_received_data_len)
        {
            for(uint8_t i=0; i<eth.TCP_received_data_len; i++)
            {
                uart.sendByte(eth.TCP_data_receive[i]);
            }
            if(eth.TCP_data_receive[0] == 'o' &&
               eth.TCP_data_receive[1] == 'p' &&
               eth.TCP_data_receive[2] == 'a' && eth.TCP_received_data_len == 3) 
            {
                eth.TCP_data_transmit[0] = 'j';
                eth.TCP_data_transmit[1] = 'o';
                eth.TCP_data_transmit[2] = 'p';
                eth.TCP_data_transmit[3] = 'a';
                eth.TCP_data_transmit[4] = '\n';
                //eth.tcp_initReply(TCP_ACK,4);        
                eth.tcp_reply(5,false);
            }
            eth.TCP_received_data_len=0;
        }
    }
    return 0;
}
