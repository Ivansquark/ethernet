#include "main.h"


//void * __dso_handle = 0;
//void * __cxa_guard_acquire = 0;

int main()
{
    //static const uint8_t flashBuff[500000]={0xAB};   
    PD_led led;
    Uart6 uart;
    SpiLcd lcd;
    Font_16x16 font;    
    font.fillScreen(0xff00);
    while(1)
    {  
    }
    return 0;
}
