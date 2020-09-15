#include "ethernet.hpp"

Eth::Eth()
{
    eth_init();
}

void Eth::eth_init()
{
    /*GPIOs RCC*/
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;

    /*! PA1 - ETH_RMII _REF_CLK */    
    GPIOA->MODER|=GPIO_MODER_MODER1_1;
    GPIOA->MODER&=~GPIO_MODER_MODER1_0; //1:0 alt func
    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR1; //1:1 max speed
    GPIOA->AFR[0] |= (11<<4); //ethernet

    /*! < PA2 ETH _MDIO SMI data input output for control physical layer>*/
    GPIOA->MODER|=GPIO_MODER_MODER2_1;
    GPIOA->MODER&=~GPIO_MODER_MODER2_0; //1:0 alt func
    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR2; //1:1 max speed
    GPIOA->AFR[0] |= (11<<8); //ethernet

    /*! < PA7 ETH_RMII _CRS_DV >*/
    GPIOA->MODER|=GPIO_MODER_MODER7_1;
    GPIOA->MODER&=~GPIO_MODER_MODER7_0; //1:0 alt func
    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR7; //1:1 max speed
    GPIOA->AFR[0] |= (11<<28); //ethernet

    /*! < PB11  ETH _RMII_TX_EN >*/
    GPIOB->MODER|=GPIO_MODER_MODER11_1;
    GPIOB->MODER&=~GPIO_MODER_MODER11_0; //1:0 alt func
    GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR11; //1:1 max speed
    GPIOB->AFR[1] |= (11<<12); //ethernet

    /*! < PB12  ETH _RMII_TXD0 >*/
    GPIOB->MODER|=GPIO_MODER_MODER12_1;
    GPIOB->MODER&=~GPIO_MODER_MODER12_0; //1:0 alt func
    GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR12; //1:1 max speed
    GPIOB->AFR[1] |= (11<<16); //ethernet

    /*! < PB13  ETH _RMII_TXD1 >*/
    GPIOB->MODER|=GPIO_MODER_MODER13_1;
    GPIOB->MODER&=~GPIO_MODER_MODER13_0; //1:0 alt func
    GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR13; //1:1 max speed
    GPIOB->AFR[1] |= (11<<20); //ethernet
    
    /*!< PC1 - ETH _MDC SMI clock for control physical layer> */    
    GPIOC->MODER|=GPIO_MODER_MODER1_1;
    GPIOC->MODER&=~GPIO_MODER_MODER1_0; //1:0 alt func
    GPIOC->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR1; //1:1 max speed
    GPIOC->AFR[0] |= (11<<4); //ethernet

    /*!< PC4 - ETH_RMII_RXD0 > */    
    GPIOC->MODER|=GPIO_MODER_MODER4_1;
    GPIOC->MODER&=~GPIO_MODER_MODER4_0; //1:0 alt func
    GPIOC->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR4; //1:1 max speed
    GPIOC->AFR[0] |= (11<<16); //ethernet

    /*!< PC5 - ETH _RMII_RXD1 > */    
    GPIOC->MODER|=GPIO_MODER_MODER5_1;
    GPIOC->MODER&=~GPIO_MODER_MODER5_0; //1:0 alt func
    GPIOC->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR5; //1:1 max speed
    GPIOC->AFR[0] |= (11<<20); //ethernet
    
    RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN; /* Enable SYSCFG Clock */
	SYSCFG->PMC|=SYSCFG_PMC_MII_RMII_SEL; //RMII PHY interface is selected	

    RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACTXEN; // Ethernet Transmission clock enable
    RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACRXEN; //Ethernet Reception clock enable
    //RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACPTPEN; // Ethernet PTP clock enable
    RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACEN; // MAC enabled
    
	/*!<The bus mode register establishes the bus operating modes for the DMA.>*/
	/* Set the SWR bit: resets all MAC subsystem internal registers and logic */
	ETH->DMABMR|=ETH_DMABMR_SR;//Software reset
	/* Wait for software reset */
	//while (((heth->Instance)->DMABMR & ETH_DMABMR_SR) != (uint32_t)RESET)
	//{
	//	/* Check for the Timeout */
	//	if((HAL_GetTick() - tickstart ) > ETH_TIMEOUT_SWRESET)
	//	{     
	//	heth->State= HAL_ETH_STATE_TIMEOUT;	
	//	/* Process Unlocked */
	//	__HAL_UNLOCK(heth);		
	//	/* Note: The SWR is not performed if the ETH_RX_CLK or the ETH_TX_CLK are  
	//		not available, please check your external PHY or the IO configuration */
	//	return HAL_TIMEOUT;
	//	}
	//}
	
	/*-------------------------------- MAC Initialization ----------------------*/
	//uint32_t temp =0U;    
	//temp = ETH->MACMIIAR;
    /*!<Write to the ETH_DMAIER register to mask unnecessary interrupt causes.>*/
    //ETH->DMAIER|=

    /*!<Write to the MAC ETH_MACCR register to configure and enable the transmit and receive operating modes.>*/
    //ETH->MACCR|=
    /*!<Write to the ETH_DMAOMR register to set bits 13 and 1 and start transmission and reception.>*/
    //ETH->DMAOMR|=
    /*-------------------- PHY initialization and configuration ----------------*/
    /*!<SMI configuration>*/
    ETH->MACMIIAR|=(1<<11);//PHY address = 1 !!!!
    ETH->MACMIIAR|=ETH_MACMIIAR_CR_Div102; /* CSR Clock Range between 150-183 MHz */ 
    /*!< ethernet mac configuration register>*/
    uint8_t smiReg0 = smi_read(0);
    if(smiReg0>>5) //if 100 Mb/s   /*!<suppose that always full duplex>*/
    {
        ETH->MACCR|=ETH_MACCR_FES; //1 - 1: 100 Mbit/s
    }else {ETH->MACCR&=~ETH_MACCR_FES;} /* 0 - (0: 10 Mbit/s)*/
    
    ETH->MACCR|=ETH_MACCR_DM; // 1 - full duplex mode
    ETH->MACCR&=~ETH_MACCR_IPCO; // 0 - IPv4 checksums disabled
    ETH->MACCR&=~ETH_MACCR_TE; // transmit enable 
    ETH->MACCR&=~ETH_MACCR_RE; // receive enable 
    /*!<Ethernet MAC MII address register (ETH_MACMIIAR)>*/
    /*!<Ethernet MAC address >*/
    ETH->MACA0HR=0x1234; //16 bits (47:32) of the 6-byte MAC address0
    ETH->MACA0LR=0x56789012; //32 bits of the 6-byte MAC address0 	
	ETH->MACFFR|=ETH_MACFFR_RA; //receive all
}


uint8_t Eth::smi_read(uint8_t reg_num)
{
    uint16_t x=0;
    ETH->MACMIIAR&=~ETH_MACMIIAR_MW;//read ready    
    ETH->MACMIIAR|=(reg_num&0x1F)<<6; //write number of PHY register in reg
    ETH->MACMIIAR|=ETH_MACMIIAR_MB;//to start
    for(uint8_t i=0;i<2;i++)
    {
        x=ETH->MACMIIDR;
        while(ETH->MACMIIAR & ETH_MACMIIAR_MB);        
    }
    return x>>8;
}
void Eth::smi_write(uint8_t reg_num, uint8_t val)
{
    ETH->MACMIIAR|=ETH_MACMIIAR_MW;//write ready
    ETH->MACMIIAR|=(reg_num&0x1F)<<6; //write number of PHY register in reg 
    ETH->MACMIIAR|=ETH_MACMIIAR_MB;//to start
    ETH->MACMIIDR=val<<8;
    while(ETH->MACMIIAR & ETH_MACMIIAR_MB);    
}