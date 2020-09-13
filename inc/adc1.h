#ifndef ADC1_H
#define ADC1_H_

#include "main.h"

class Adc1
{
public:
    Adc1(uint8_t x=0):adc_num(x)
    {adc1_ini(x);}
    uint16_t readADC()
    {
        ADC1->CR2|=ADC_CR2_SWSTART; // by software
        while(!(ADC1->SR&ADC_SR_EOC)){}
        return ADC1->DR;
    }
    float getTemp()
    {
        if(adc_num==0){return 0.0F;}        
        else if(adc_num==16)
        {
            float x = 25.0F + ((float)(readADC())*3.0F/4095.F - 0.76F)/0.0025F;
            return x;
        } 
        return 1.0F;
    }    
private:
    uint8_t adc_num;
    void adc1_ini(uint8_t numChannel)
    {
        switch(numChannel)
        {
            // PA0
            case 0:
            // //////////////// PIN definition //////////////////
            RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
            GPIOA->MODER|=GPIO_MODER_MODER0; // 1:1 - analog mode
            //---------------------------------------------------
            RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;
            ADC1->CR2|=ADC_CR2_SWSTART; // by software
            ADC1->CR2&=~ADC_CR2_CONT; // by single conversion mode
            ADC1->SMPR2&=~ADC_SMPR2_SMP0;  // 000 - 3 cycles
            ADC1->SQR3 &=~ADC_SQR3_SQ1; // 00000 - 0 chanel 1-conversion
            ADC1->CR2|=ADC_CR2_ADON; //ADC on
            break;
            // Temp Sensor
            case 16: 
            RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;
            ADC->CCR|=ADC_CCR_TSVREFE; //for temp sens
            ADC1->CR2|=ADC_CR2_SWSTART; // by software
            ADC1->CR2&=~ADC_CR2_CONT; // by single conversion mode
            ADC1->SMPR1|=ADC_SMPR1_SMP16;  // 000 - 3 cycles
            ADC1->SQR3 |=ADC_SQR3_SQ1_4; // 10000 - 16 chanel 1-conversion
            ADC1->CR2|=ADC_CR2_ADON; //ADC on
            break;
            default:break;
        }        
    }
};
class DAC1:public iTaskFR
{
public:
    DAC1(){
        dac1_ini();
    }
    void run()
    {                        
        while(1)
        {
            for(uint8_t i=0;i<40;i++)
            {
                DAC->DHR12R1=buf_exp[i];
                //for(int i=0;i<4;i++){;}
            }
            cRtos::sleep(1);
        }
    }
    void exp()
    {
        for(uint8_t i=0;i<50;i++)
            {
                DAC->DHR12R1=buf_exp[i];
                //for(int i=0;i<4;i++){;}
            }
    }
    void sin()
    {
        for(uint8_t i=0;i<60;i++)
            {
                DAC->DHR12R1=buf_sin[i];
                //for(int i=0;i<4;i++){;}
            }
    }

private:
    uint16_t dacVal=0;
    uint16_t buf_sin[60]={
    11,45,100,177,274,391,526,678,
    844,1024,1215,1415,1622,1834,2048,2262,
    2474,2681,2881,3072,3252,3418,3570,3705,
    3822,3919,3996,4051,4085,4095,4085,4051,
    3996,3919,3822,3705,3570,3418,3252,3072,
    2881,2681,2474,2262,2048,1834,1622,1415,
    1215,1024,844,678,526,391,274,177,
    100,45,11,0
    };
    uint16_t buf_exp[50]={
        4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
        4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
        3800, 3400, 3000, 2600, 2200, 1800, 1500, 1200, 1000, 900,
        800, 700, 600, 500, 400, 300, 200, 100, 50, 40,
        20, 10, 8, 6, 5, 4, 3, 2, 1, 0
    };
    void dac1_ini()
    {
        RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER|=GPIO_MODER_MODER4_1;
        GPIOA->MODER&=~GPIO_MODER_MODER4_0; // 1:0 - alt function
        GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR4; //1:1 highest speed       

        RCC->APB1ENR|=RCC_APB1ENR_DACEN;
        DAC->CR|=DAC_CR_EN1; //DAC channel 1 enabled
        DAC->DHR12R1=0;        
    }
};
#endif //ADC1_H_