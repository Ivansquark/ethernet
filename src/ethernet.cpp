#include "ethernet.hpp"

uint32_t Eth::ReceiveDL[4]={0};
uint32_t Eth::TransmitDL[4]={0};

Eth* Eth::pThis = nullptr;

Eth::Eth(uint8_t* rxB,uint8_t* txB)
{
    for(uint8_t i=0; i<6;i++)
    {frameTx.mac_dest[i] = mac_broadcast[i];}
    for(uint8_t i=0; i<6;i++)
    {frameTx.mac_src[i] = mac[i];}
    frameTx.type = swap16(0x0806);   
    
    RxBuf=rxB; TxBuf=txB;
    pThis=this;
    eth_init();
    descr_init();
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
		
    /*-------------------- PHY initialization and configuration ----------------*/
    /*!<SMI configuration>*/
    ETH->MACMIIAR|=(1<<11);//PHY address = 1 !!!!
    ETH->MACMIIAR|=ETH_MACMIIAR_CR_Div102; /* CSR Clock Range between 150-183 MHz */ 
    /*-------------------------------- MAC Initialization ----------------------*/
    /*!< ethernet mac configuration register>*/
    uint8_t smiReg0 = smi_read(0);
    if(smiReg0>>5) //if 100 Mb/s   /*!<suppose that always full duplex>*/
    {
        ETH->MACCR|=ETH_MACCR_FES; //1 - 1: 100 Mbit/s
    }else {ETH->MACCR&=~ETH_MACCR_FES;} /* 0 - (0: 10 Mbit/s)*/
    
    ETH->MACCR|=ETH_MACCR_DM; // 1 - full duplex mode
	
    //ETH->MACCR&=~ETH_MACCR_IPCO; // 0 - IPv4 checksums disabled
    
    /*!<Ethernet MAC MII address register (ETH_MACMIIAR)>*/
    /*!<Ethernet MAC address >*/
    ETH->MACA0HR=0x3412; //16 bits (47:32) of the 6-byte MAC address0
    ETH->MACA0LR=0x56789abc; //32 bits of the 6-byte MAC address0 	
	ETH->MACFFR|=ETH_MACFFR_RA; //receive all
	/*!<Descriptor lists initialization>*/
	/*!<The Receive descriptor list address register points to the start of the receive descriptor list.>*/
	ETH->DMARDLAR = (uint32_t)ReceiveDL; //write start address of recieve descriptor list array in special reg
	/*!<Transmit descriptor list address register points to the start of the transmit descriptor list.>*/
	ETH->DMATDLAR = (uint32_t)TransmitDL; //write start address of recieve descriptor list array in special reg
	
    /*!<for hardware counting of checksums transmit FIFO must configured for Store-and-forward mode (not in treashold control mode)>*/
	/*!< operation mode register >*/
    ETH->DMAOMR |= ETH_DMAOMR_TSF;//transmission starts when a full frame resides in the Transmit FIFO. TTC - ignored
	ETH->DMAOMR |= ETH_DMAOMR_RSF;//frame is read from the Rx FIFO after the complete frame has been written to it; RTC - ignored  
	
	/*!<Descriptors lists configuration>*/
	/*!<Ethernet DMA bus mode register (ETH_DMABMR)>*/
	 
	/*<____________interrupts____________________________>*/
    ETH->MACIMR|=ETH_MACIMR_PMTIM | ETH_MACIMR_TSTIM;//disable interrupts PMT and time stamps
	ETH->DMAIER|= ETH_DMAIER_NISE; //Normal interrupt enable
	ETH->DMAIER|= ETH_DMAIER_RIE; //Receive interrupt enable
	ETH->DMAIER|= ETH_DMAIER_TIE; //Transmit interrupt enable
	
	ETH->MACCR|=ETH_MACCR_TE; // transmit enable 
    ETH->MACCR|=ETH_MACCR_RE; // receive enable 
	
	ETH->DMABMR&=~ETH_DMABMR_PBL;
	ETH->DMABMR|=ETH_DMABMR_PBL_1Beat;// 1: 1-byte
    NVIC_EnableIRQ(ETH_IRQn);
}
//--------------------------------------- SMI methods ----------------------------------------
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
//--------------------------------------------------------------------------------------------------------
void Eth::descr_init()
{
	ReceiveDL[3] = (uint32_t)ReceiveDL; //sets address of new descriptor (its the same)
	TransmitDL[3] = (uint32_t)TransmitDL; //sets address of new descriptor (its the same)
	ReceiveDL[2] = (uint32_t)RxBuf; //sets address of new descriptor (its the same)
	TransmitDL[2] = (uint32_t)TxBuf; //sets address of new descriptor (its the same)
	/*!< Two descriptors indicates on data two buffers >*/
	ReceiveDL[0] = 0;
	ReceiveDL[1] = 0;
	ReceiveDL[1] |= (2048); //size of Tx first buffer
	
	TransmitDL[0] = 0;
	TransmitDL[0] |= (3<<28); //sets LS and FS - first and simultaniously last descriptor
	TransmitDL[0] |= (3<<22); //11: IP Header checksum and payload checksum calculation and insertion are enabled, and pseudo-header checksum is calculated in hardware.
	TransmitDL[0] |= (1<<21); //TER descriptor list reached its final descriptor
	TransmitDL[1] = 0;
	TransmitDL[1] |= (2048); //size of Tx first buffer
}
//-----------------------------------------------------------------------------------------------------------
void Eth::receive_frame()
{	
	ReceiveDL[0] |= (1<<31); //sets OWN bit to DMA
	ETH->DMAOMR |= ETH_DMAOMR_SR; //start reception (starts DMA polling)
	//need timer		
    for(uint32_t i=0;i<5000;i++);
	ETH->DMAOMR &=~ ETH_DMAOMR_SR; //(ends DMA polling)
    ReceiveDL[0] &=~ (1<<31); //0: sets OWN bit to CORE 
}

void Eth::transmit_frame(uint16_t size)
{
	TransmitDL[0] |= (1<<31); //sets OWN bit to DMA
    TransmitDL[1] = 0;
    TransmitDL[1] |= (size); //size of Tx first buffer
	ETH->DMAOMR |= ETH_DMAOMR_ST; //start transmittion (starts DMA polling)
	//need timer
    for(uint32_t i=0;i<50000000;i++);
	ETH->DMAOMR &=~ ETH_DMAOMR_ST; //(ends DMA polling)
}

void Eth::arp_read()
{
    receive_frame();
    FrameRx* fRx = (FrameRx*)RxBuf;
    arp_recievePtr = (ARP*)(fRx+1);
    if(fRx->type == swap16(0x0806)) //ARP packet
    {
        if( fRx->mac_dest[0]==0xff && fRx->mac_dest[1]==0xff && fRx->mac_dest[2]==0xff &&
            fRx->mac_dest[3]==0xff &&fRx->mac_dest[4]==0xff &&fRx->mac_dest[5]==0xff && 
            arp_recievePtr->ip_dst[0]==ip[0] && arp_recievePtr->ip_dst[1]==ip[1] &&
            arp_recievePtr->ip_dst[2]==ip[2] && arp_recievePtr->ip_dst[3]==ip[3]) //recieve broadcast need to send back mac
        {
            for(uint8_t i=0;i<6;i++)
            {
                mac_recieve[i] = fRx->mac_src[i];
                
                arp_recievePtr->macaddr_dst[i] = mac_recieve[i]; 
                //fRx->mac_src[i] = mac_recieve[i];                
            }
            for(uint8_t i=0;i<4;i++)
            {ip_receive[i] = arp_recievePtr->ip_src[i];} //write incoming ip address of requesting PC
            arp_answer();
            //RxBuf[0]=0;
        }
        else if(fRx->mac_dest[0]==mac[0] && fRx->mac_dest[1]==mac[1] &&
                fRx->mac_dest[2]==mac[2] && fRx->mac_dest[3]==mac[3] &&
                fRx->mac_dest[4]==mac[4] && fRx->mac_dest[5]==mac[5]) //if answer (after send arp from micro) 
        {
            for(uint8_t i=0;i<6;i++)
            {mac_recieve[i] = arp_recievePtr->macaddr_src[i];}                                    
        }
    }    
}
void Eth::arp_send()
{
    for(uint8_t i=0; i<sizeof(frameTx);i++)
    {TxBuf[i] = *((uint8_t*)(&frameTx)+i);}
    for(uint8_t i=0; i<sizeof(arpInit);i++)
    {
        TxBuf[i + sizeof(frameTx)] = *((uint8_t*)(&arpInit)+i);
    }
    transmit_frame(44);
}

void Eth::arp_answer() //TODO:
{
    for(uint8_t i=0;i<6;i++)
    {frameTx.mac_dest[i] = mac_recieve[i];}

    //FrameTx* fTx = (FrameTx*)&frameTx;
    arp_recievePtr = &arpInit;
    arp_recievePtr->op = swap16(0x0002);
    for(uint8_t i=0;i<6;i++)
    {arp_recievePtr->macaddr_src[i] = mac[i];} // set source mac in ARP
    for(uint8_t i=0;i<4;i++)
    {arp_recievePtr->ip_src[i] = ip[i];} // set source mac in ARP
    for(uint8_t i=0;i<6;i++)
    {arp_recievePtr->macaddr_dst[i] = mac_recieve[i];} // set dest mac in ARP
    for(uint8_t i=0;i<4;i++)
    {arp_recievePtr->ip_dst[i] = ip_receive[i];} // set dest ip in ARP answer

    for(uint8_t i=0; i<sizeof(frameTx);i++)
    {TxBuf[i] = *((uint8_t*)(&frameTx)+i);}
    for(uint8_t i=0; i<sizeof(ARP); i++)
    {TxBuf[i+sizeof(frameTx)] = *((uint8_t*)(arp_recievePtr)+i);}
    transmit_frame(44);
}

void ETH_IRQHandler(void)
{
    //ETH->DMASR|=ETH_DMASR_RS; //clear bit of interrupt
    Eth::pThis->arpReceiveFlag=true;
}

