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
    Eth eth;
	eth.descr_init(TxBuf,RxBuf);
	Eth::RecieveDL[1]=RxBuf;//sets buffer address to second descriptor
	Eth::TransmitDL[1]=TxBuf;//sets buffer address to second descriptor
    uint8_t x=eth.smi_read(0);
    font.intToChar(x);
    font.print(10,50,0x00ff,font.arr,0);
	
    while(1)
    {
                 
    }
    return 0;
}
