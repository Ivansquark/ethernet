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
    
    SYSCFG->PMC|=SYSCFG_PMC_MII_RMII_SEL; //RMII PHY interface is selected

    RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACTXEN; // Ethernet Transmission clock enable
    RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACRXEN; //Ethernet Reception clock enable
    //RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACPTPEN; // Ethernet PTP clock enable
    RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACEN; // MAC enabled

    /*!<Write to the ETH_DMAIER register to mask unnecessary interrupt causes.>*/
    //ETH->DMAIER|=

    /*!<Write to the MAC ETH_MACCR register to configure and enable the transmit and receive operating modes.>*/
    //ETH->MACCR|=
    /*!<Write to the ETH_DMAOMR register to set bits 13 and 1 and start transmission and reception.>*/
    //ETH->DMAOMR|=

    /*!< ethernet mac configuration register>*/
    ETH->MACCR|=ETH_MACCR_FES; //1 - 1: 100 Mbit/s   (0: 10 Mbit/s)
    ETH->MACCR|=ETH_MACCR_DM; // 1 - full duplex mode
    ETH->MACCR&=~ETH_MACCR_IPCO; // 0 - IPv4 checksums disabled
    ETH->MACCR&=~ETH_MACCR_TE; // transmit enable 
    ETH->MACCR&=~ETH_MACCR_RE; // receive enable 
    /*!<Ethernet MAC MII address register (ETH_MACMIIAR)>*/
    ETH->MACMIIAR|=ETH_MACMIIAR_CR_Div26; //011 35-60 MHz HCLK/26
    ETH->MACMIIAR|=ETH_MACMIIAR_MW; //0-write 1-read  operation using the MII Data register
    /*!<Ethernet MAC address >*/
    ETH->MACA0HR=0x1234; //16 bits (47:32) of the 6-byte MAC address0
    ETH->MACA0LR=0x56789012; //32 bits of the 6-byte MAC address0 
}