#ifndef SDSPI
#define SDSPI

#include "stm32f4xx.h"
#include <stdint.h>

class SD
{
public:
    SD(){sd_spi_init();}
    bool SDHC=0;
    uint8_t send_command(uint8_t dt0, uint8_t dt1, uint8_t dt2,
                         uint8_t dt3, uint8_t dt4, uint8_t dt5)
    {
        uint32_t cnt=0; uint8_t res = 0;
        cs_set();
        send_byte(dt0);send_byte(dt1);send_byte(dt2);
        send_byte(dt3);send_byte(dt4);send_byte(dt5);
        while((res = read_byte()) == 0xFF) //записываем в res считанное число не равное 0xFF
        {
            if(cnt++ > 0xFFFF)break;//выйти по таймауту
        } 
        //проверка ответа если посылалась команда READ_OCR
        //if(res == 0x00 && dt0 == 58)     
        //{
        //  uint8_t tmp = read_byte();                      //прочитать один байт регистра OCR            
        //  if(tmp & 0x40) SDHC = 1;               //обнаружена карта SDHC 
        //  else           SDHC = 0;               //обнаружена карта SD
        //  //прочитать три оставшихся байта регистра OCR
        //  read_byte(); 
        //  read_byte(); 
        //  read_byte(); 
        //}
        //read_byte();
        cs_idle();
        return res;
    }
	uint8_t send_cmd(uint8_t cmd, uint32_t arg)
	{
		uint8_t res=0; uint32_t n = 0xFFFF;
		cs_set();
		send_byte(0x40+cmd);
		send_byte((uint8_t)arg>>24);
		send_byte((uint8_t)arg>>16);
		send_byte((uint8_t)arg>>8);
		send_byte((uint8_t)arg);
		if(cmd==0){send_byte(0x95);} // Valid CRC for CMD0(0)
		else if(cmd==8){send_byte(0x87);} // Valid CRC for CMD8(0x1AA)
        for(uint32_t i=0;i<10000;i++);
		do 
		{
			res = read_byte();
		} while ((res & 0x80) && --n);
		cs_idle();
		return res;		
	}
    uint8_t sd_init()
    {
		cs_idle();
        uint8_t cnt=0;
		for(uint32_t i=0;i<100000;i++);//delay wait 1ms
        for(uint8_t i=0;i<10;i++)
        {
            send_byte(0xFF);
        }
        //cs_set();
		//for(uint16_t i=0;i<60000;i++);//delay
        uint8_t temp = send_command(0x40,0x00,0x00,0x00,0x00,0x95); //CMD0
		//uint8_t temp = send_cmd(0,0x000000);
        return temp;
        //if(temp!=0x01) return 1; //Выйти если ответ не 0x01 
        //do
        //{
        //    temp=send_command(0x41,0x00,0x00,0x00,0x00,0x95); //CMD1 передаем также, меняется только индекс
        //    send_byte(0xFF);
        //    cnt++;
        //} while ((temp!=0x00)&&cnt<0xFFFF); //Ждёс ответа R1
        //if(cnt>=0xFFFF) return 2;
        //return 0;
    }
    static constexpr uint8_t GO_IDLE_STATE = 0; //Программная перезагрузка 
    static constexpr uint8_t SEND_IF_COND = 8; //Для SDC V2 - проверка диапазона напряжений 
    static constexpr uint8_t READ_SINGLE_BLOCK = 17; //Чтение указанного блока данных 
    static constexpr uint8_t WRITE_SINGLE_BLOCK = 24; //Запись указанного блока данных
    static constexpr uint8_t SD_SEND_OP_COND = 41; //Начало процесса инициализации 
    static constexpr uint8_t APP_CMD = 55; //Главная команда из ACMD  команд
    static constexpr uint8_t READ_OCR = 58; //Чтение регистра OCR
    
//private:
    void send_byte(uint8_t byte)
    {
        while(!(SPI2->SR&SPI_SR_TXE)){}        
        SPI2->DR=byte;
        //*(uint8_t*)SPI2->DR=byte;
        while((SPI2->SR&SPI_SR_BSY)){}
    }
    uint8_t read_byte()
    {
        uint8_t temp=0;
        SPI2->DR = 0xFF; //запускаем обмен
        while(!(SPI2->SR&SPI_SR_RXNE)); //ждем пока не появится новое значение в буффере приемника
        temp = SPI2->DR;        
        return  temp;//возвращаем значение из буффера приемника
    }
    uint16_t read_word()
    {
        SPI2->DR = 0; //запускаем обмен
        SPI2->DR = 0xFFFF; //запускаем обмен
        while(!(SPI2->SR&SPI_SR_RXNE)) //ждем пока не появится новое значение в буффере приемника
        return SPI2->DR; //возвращаем значение из буффера приемника
    }
    inline void cs_idle() //__attribute ((always_inline))
    {
        GPIOB->ODR|=GPIO_ODR_ODR_12;
    }
    inline void cs_set() //__attribute ((always_inline))
    {
        GPIOB->ODR&=~GPIO_ODR_ODR_12;
    }
    void sd_spi_init()
    {
        //------- SPI2 b12-NSS b13-SCK b14-MISO b15-MOSI ------------

        //--------PB12 - programm NSS push-pull output-----------
        GPIOB->MODER|=GPIO_MODER_MODER12_0;//
        GPIOB->MODER&=~GPIO_MODER_MODER12_1;// 0:1 output push-pull
        GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR12;
        //-------b13-SCK b14-MISO b15-MOSI - alt func SPI2 --------------------
        GPIOB->MODER|=(GPIO_MODER_MODER13_1|GPIO_MODER_MODER14_1|GPIO_MODER_MODER15_1);
        GPIOB->MODER&=~(GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0|GPIO_MODER_MODER15_0);
        //GPIOB->AFR[0]=0;
        GPIOB->AFR[1]|=(5<<20)|(5<<24)|(5<<28);//alt func 5 (SPI2)
        GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR13;//
        GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR15;//max speed on pins

        //------------- тактируем SPI-2  ---------------------------
        RCC->APB1ENR|=RCC_APB1ENR_SPI2EN; //clock on fast SPI-1
        //------------- предделитель SPI-1 -----------------------------
        SPI2->CR1|=SPI_CR1_BR; // (000=>psc=1) 1:1:1  => 42000000/256 = 164,0625 kHz - SPI-2 clk
        //------------- настройка SPI - 1 -----------------------------------------
        SPI2->CR1&=~SPI_CR1_BIDIMODE; // двухпроводной режим работы
        SPI2->CR1|=SPI_CR1_BIDIOE; //1: Output enabled (transmit-only mode)
        SPI2->CR1&=~SPI_CR1_CRCEN; // 0: CRC calculation disabled
        SPI2->CR1&=~SPI_CR1_CRCNEXT; //0: Data phase (no CRC phase)
        SPI2->CR1&=~SPI_CR1_DFF; //0: 8-bit data frame format is selected for transmission/reception
        SPI2->CR1&=~SPI_CR1_RXONLY; //0: Full duplex (Transmit and receive)
        SPI2->CR1|=SPI_CR1_SSM; // программное управление ведомым включено 
        SPI2->CR1|=SPI_CR1_SSI; // нужно чтобы эти два бита были в 1 для правильной инициализации мастера
        SPI2->CR1&=~SPI_CR1_LSBFIRST; //1: LSB first //0: MSB transmitted first
        SPI2->CR1|=SPI_CR1_MSTR; //1: Master configuration
        SPI2->CR1|=SPI_CR1_CPOL; //1: CK to 1 when idle (смотреть в datasheet slave) 
        SPI2->CR1&=~SPI_CR1_CPHA; //1: The second clock transition is the first data capture edge (тоже)
        
        //NVIC_EnableIRQ(SPI1_IRQn);
        //----------- включаем SPI-1 --------------------------------------------
        SPI2->CR1|=SPI_CR1_SPE;
    }    
};

#endif //SDSPI