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
	uint8_t x=eth.smi_read(0);
    font.intToChar(x);
    font.print(10,50,0x00ff,font.arr,0);
    eth.receive_frame();
	for(uint8_t i=0;i<60;i++)
    {uart.sendByte(RxBuf[i]);}
    for(uint8_t i=0;i<6;i++)
    {TxBuf[i]=0xff;}    
    TxBuf[6]=0x34;TxBuf[7]=0x12;TxBuf[8]=0x56;TxBuf[9]=0x78;TxBuf[10]=0x9a;TxBuf[11]=0xbc;
    TxBuf[12]=0x08;TxBuf[13]=0x06;
    TxBuf[14]=0x00;TxBuf[15]=0x01;
    TxBuf[16]=0x08;TxBuf[17]=0x00;
    TxBuf[18]=0x06;TxBuf[19]=0x04;
    TxBuf[20]=0x00;TxBuf[21]=0x01;
    TxBuf[22]=0x34;TxBuf[23]=0x12;TxBuf[24]=0x56;TxBuf[25]=0x78;TxBuf[26]=0x9a;TxBuf[27]=0xbc;
    TxBuf[28]=0xac;TxBuf[29]=0x16;TxBuf[30]=0x01;TxBuf[31]=0x50;
    for(uint8_t i=32;i<38;i++)
    {TxBuf[i]=0xff;}    
    TxBuf[38]=0xc0;TxBuf[39]=0xa8;TxBuf[40]=0x00;TxBuf[41]=0x67;
    
    uint8_t y=0;
    while(1)
    {
        eth.transmit_frame(42);
        //uart.sendByte(TxBuf[y++]);  
        eth.receive_frame();
	    for(uint8_t i=0;i<60;i++)
        {uart.sendByte(RxBuf[i]);}               
    }
    return 0;
}
