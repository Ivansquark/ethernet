#ifndef UART_HPP
#define UART_HPP

#include "stm32f4xx.h"

class Uart6
{
public:
    Uart6()
    {
        uart6_ini();
    }
    void sendByte(uint8_t byte)
    {
        while(!(USART6->SR & USART_SR_TXE)){};

        USART6->DR=byte;
    }
    void sendStr(const char* str)
    {
        uint8_t i=0;
        while(str[i]!='\0')
        {
            sendByte(str[i++]);
        }
    }
private:
    void uart6_ini()
    {
        //------------------- pin C ------------------
        RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN; // тактирование на порт C
        //**************************Tx portC-6
        GPIOC->MODER &=~ (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
        GPIOC->MODER |= (GPIO_MODER_MODER7_1 | GPIO_MODER_MODER6_1); // 1:0 -alternate mode
        GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6; // 1:1 - very high speed
        //**************************Rx portC-7
        GPIOC->AFR[0] |= (8<<28)|(8<<24); //alt func 7-uart1 to 6 pin and to 7 pin
        //------------------- USART -------------------------------------
        //настраиваем альтернативные пины usart1
        RCC->APB2ENR|=RCC_APB2ENR_USART6EN; //подаем тактирование
        USART6->CR1&=~USART_CR1_M; //0 - 8 Data bits
        USART6->CR1|=USART_CR1_TE; // Transmission enabled
        USART6->CR1|=USART_CR1_RE; // Recieving enabled
        USART6->CR1|=USART_CR1_RXNEIE; //enable interrupt on Rx from usart1
        //USART1->BRR=0x445C; //2400 (APB2CLK - baudrate/2)/baudrate 
        // APB2 clk =84000000 
        USART6->BRR=0x222D; //9600 //(84000000-4800)/9600
        //USART1->BRR=0x9C3; //9600 //(48000000-4800)/9600 !!! какогото хрена частота шины в ДВА раза МЕНЬШЕ!!!!
	    //USART1->BRR=0xEA5; //9600 //(36000000-4800)/9600        
        //USART1->BRR=0x16C; //115200
        USART6->CR1|=USART_CR1_UE; //USART EN
        //NVIC_EnableIRQ(USART1_IRQn);
    }
};

#endif //UART_HPP