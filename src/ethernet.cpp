#include "ethernet.hpp"

uint32_t Eth::ReceiveDL[4]={0};
uint32_t Eth::TransmitDL[4]={0}; //static for creating not in stack

Eth* Eth::pThis = nullptr;

Eth::Eth(uint8_t* rxB,uint8_t* txB)
{
    for(uint8_t i=0; i<6;i++)
    {frameTx.mac_dest[i] = mac_broadcast[i];}
    for(uint8_t i=0; i<6;i++)
    {frameTx.mac_src[i] = mac[i];}
    frameTx.type = swap16(0x0806); //for sending arp request   
    
    RxBuf=rxB; TxBuf=txB;
    pThis=this;
    eth_init();
    descr_init();
}

void Eth::eth_init()
{
    /*GPIOs RCC*/
    RCC->AHB1ENR|=(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);
    /*!< PA1-ETH_RMII_REF_CLK  PA2-ETH_MDIO SMI PA7-ETH_RMII_CRS_DV >*/    
    GPIOA->MODER|=(GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_1 | GPIO_MODER_MODER7_1);
    GPIOA->MODER&=~(GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER7_0); //1:0 alt func
    GPIOA->OSPEEDR|=(GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR7); //1:1 max speed
    GPIOA->AFR[0] |= (11<<4)|(11<<8)|(11<<28); //ethernet    
    /*! < PB11-ETH _RMII_TX_EN PB12-ETH _RMII_TXD0 PB13-ETH_RMII_TXD1 >*/
    GPIOB->MODER|=(GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1 | GPIO_MODER_MODER13_1);
    GPIOB->MODER&=~(GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0); //1:0 alt func
    GPIOB->OSPEEDR|=(GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12 | GPIO_OSPEEDER_OSPEEDR13); //1:1 max speed
    GPIOB->AFR[1] |= (11<<12)|(11<<16)|(11<<20); //ethernet    
    /*!< PC1-ETH_MDC SMI PC4-ETH_RMII_RXD0 PC5-ETH_RMII_RXD1 >*/    
    GPIOC->MODER|=(GPIO_MODER_MODER1_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1);
    GPIOC->MODER&=~(GPIO_MODER_MODER1_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0); //1:0 alt func
    GPIOC->OSPEEDR|=(GPIO_OSPEEDER_OSPEEDR1|GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5); //1:1 max speed
    GPIOC->AFR[0] |= (11<<4)|(11<<16)|(11<<20); //ethernet
    /*!< enable SYSCFG for RMII PHY interface >*/    
    RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN; /* Enable SYSCFG Clock for RMII select*/
	SYSCFG->PMC|=SYSCFG_PMC_MII_RMII_SEL; //RMII PHY interface is selected	
    /*< Ethernet RCC (Rx, Tx, MAC) >*/
    RCC->AHB1ENR|=(RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN | RCC_AHB1ENR_ETHMACEN); 
    //RCC->AHB1ENR|=RCC_AHB1ENR_ETHMACPTPEN; // Ethernet PTP clock enable
    
	/*!<The bus mode register establishes the bus operating modes for the DMA.>*/
	/* Set the SWR bit: resets all MAC subsystem internal registers and logic */
	ETH->DMABMR|=ETH_DMABMR_SR;//Software reset
    for(uint32_t i=0;i<1000000;i++); //wait for software reset		
    /*-------------------- PHY initialization and configuration ----------------*/
    /*!<SMI configuration>*/
    ETH->MACMIIAR|=(1<<11);//PHY address = 1 !!!!
    ETH->MACMIIAR|=ETH_MACMIIAR_CR_Div102; /* CSR Clock Range between 150-183 MHz */ 
    /*-------------------------------- MAC Initialization ----------------------*/
    /*!< ethernet mac configuration register>*/
    uint8_t smiReg0 = smi_read(0);
    if(smiReg0>>5) //if 100 Mb/s   /*!<suppose that always full duplex>*/
    {ETH->MACCR|=ETH_MACCR_FES;} //1 - 1: 100 Mbit/s    
    else {ETH->MACCR&=~ETH_MACCR_FES;} /* 0 - (0: 10 Mbit/s)*/    
    ETH->MACCR|=ETH_MACCR_DM; // 1 - full duplex mode
	
    //ETH->MACCR&=~ETH_MACCR_IPCO; // 0 - IPv4 checksums disabled

    /*!<Ethernet MAC MII address register (ETH_MACMIIAR) for checking DA in frames>*/    
    ETH->MACA0HR=0x3412; //16 bits (47:32) of the 6-byte MAC address0
    ETH->MACA0LR=0x56789abc; //32 bits of the 6-byte MAC address0 	
	//ETH->MACFFR|=ETH_MACFFR_RA; //receive all
    /*!<Destination address inverse filtering>*/
    ETH->MACFFR|=ETH_MACFFR_DAIF;

	/*!<Descriptor lists initialization>*/
	/*!<The Receive descriptor list address register points to the start of the receive descriptor list.>*/
	ETH->DMARDLAR = (uint32_t)ReceiveDL; //write start address of recieve descriptor list array in special reg
	/*!<Transmit descriptor list address register points to the start of the transmit descriptor list.>*/
	ETH->DMATDLAR = (uint32_t)TransmitDL; //write start address of recieve descriptor list array in special reg
	
    /*!<for hardware counting of checksums transmit FIFO must configured for Store-and-forward mode (not in treashold control mode)>*/
	/*!< operation mode register >*/
    ETH->DMAOMR |= ETH_DMAOMR_TSF;//transmission starts when a full frame resides in the Transmit FIFO. TTC - ignored
	ETH->DMAOMR |= ETH_DMAOMR_RSF;//frame is read from the Rx FIFO after the complete frame has been written to it; RTC - ignored  
	
	ETH->MACCR|=ETH_MACCR_TE; // transmit enable 
    ETH->MACCR|=ETH_MACCR_RE; // receive enable 
	/*!< DMA configuration >*/
	ETH->DMABMR&=~ETH_DMABMR_PBL;
	ETH->DMABMR|=ETH_DMABMR_PBL_1Beat;// 1: 1-byte
    /*<____________interrupts____________________________>*/
    ETH->MACIMR|=ETH_MACIMR_PMTIM | ETH_MACIMR_TSTIM;//disable interrupts PMT and time stamps
	ETH->DMAIER|= (ETH_DMAIER_NISE|ETH_DMAIER_RIE|ETH_DMAIER_TIE); //Normal and RxTx interrupt enable
    NVIC_EnableIRQ(ETH_IRQn);
}
//--------------------------------------- SMI methods ----------------------------------------
uint8_t Eth::smi_read(uint8_t reg_num)
{
    uint16_t x=0;
    ETH->MACMIIAR&=~ETH_MACMIIAR_MW;//read ready    
    ETH->MACMIIAR|=(reg_num&0x1F)<<6; //write number of PHY register in reg
    ETH->MACMIIAR|=ETH_MACMIIAR_MB;//to start
    for(uint8_t i=0;i<2;i++) //two times because for one is not initialized
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
	/*!< Two descriptors indicates on data two buffers (use one descriptor) >*/
	ReceiveDL[0] = 0;
	ReceiveDL[1] = 0;
    ReceiveDL[1] |= (1<<14);
	ReceiveDL[1] |= (2048); //size of Rx first buffer
	
	TransmitDL[0] = 0;
	TransmitDL[0] |= (3<<28); //sets LS and FS - first and simultaniously last descriptor
	TransmitDL[0] |= (1<<23);
    TransmitDL[0] |= (1<<22); //11: IP Header checksum and payload checksum calculation and insertion are enabled, and pseudo-header checksum is calculated in hardware.
	TransmitDL[0] |= (1<<21); //TER descriptor list reached its final descriptor
	TransmitDL[1] = 0;    
	TransmitDL[1] |= (2048); //size of Tx first buffer
    ETH->DMAOMR |= ETH_DMAOMR_SR; //start reception (starts DMA polling)	
    ReceiveDL[0] |= (1<<31); //sets OWN bit to DMA    
}
//-----------------------------------------------------------------------------------------------------------
void Eth::receive_frame()
{	
	//ReceiveDL[0] |= (1<<31); //sets OWN bit to DMA	
}
void Eth::transmit_frame(uint16_t size)
{
    TransmitDL[1] = (size); //size of Tx first buffer
	TransmitDL[0] |= (1<<31); //sets OWN bit to DMA
    ETH->DMAOMR |= ETH_DMAOMR_ST; //start transmittion (starts DMA polling)	
	//need timer
    //for(uint32_t i=0;i<100;i++);
	ETH->DMAOMR &=~ ETH_DMAOMR_ST; //(ends DMA polling)
}
//--------------------------------------------------------------
void Eth::frame_read()
{
    fRx = (FrameX*)RxBuf;
    if(fRx->type == swap16(ARP_i)) //ARP packet
    {
        arp_read();
    }
    else if(fRx->type == swap16(IPv4_i)) //IPv4 packet
    {
        ip_read();
    }
}

void Eth::arp_read()
{
    arp_receivePtr = (ARP*)(fRx+1);    
    if( fRx->mac_dest[0]==0xff && fRx->mac_dest[1]==0xff && fRx->mac_dest[2]==0xff &&
        fRx->mac_dest[3]==0xff &&fRx->mac_dest[4]==0xff &&fRx->mac_dest[5]==0xff && 
        arp_receivePtr->ip_dst[0]==ip[0] && arp_receivePtr->ip_dst[1]==ip[1] &&
        arp_receivePtr->ip_dst[2]==ip[2] && arp_receivePtr->ip_dst[3]==ip[3]) //recieve broadcast need to send back mac
    {
        for(uint8_t i=0;i<6;i++)
        {
            mac_recieve[i] = fRx->mac_src[i];                
            arp_receivePtr->macaddr_dst[i] = mac_recieve[i]; 
            //fRx->mac_src[i] = mac_recieve[i];                
        }
        for(uint8_t i=0;i<4;i++)
        {ip_receive[i] = arp_receivePtr->ip_src[i];} //write incoming ip address of requesting PC
        arp_answer();
    }
    else if(fRx->mac_dest[0]==mac[0] && fRx->mac_dest[1]==mac[1] &&
            fRx->mac_dest[2]==mac[2] && fRx->mac_dest[3]==mac[3] &&
            fRx->mac_dest[4]==mac[4] && fRx->mac_dest[5]==mac[5]) //if answer (after send arp from micro) 
    {
        for(uint8_t i=0;i<6;i++)
        {mac_recieve[i] = arp_receivePtr->macaddr_src[i];}                                    
    }      
}
void Eth::arp_send()
{
    FrameX arpTx = {0xff,0xff,0xff,0xff,0xff,0xff,0x32,0x12,0x56,0x78,0x9a,0xbc,swap16(0x0806)};    
    for(uint8_t i=0; i<sizeof(arpTx);i++)
    {TxBuf[i] = *((uint8_t*)(&arpTx)+i);}
    for(uint8_t i=0; i<sizeof(arpSend);i++)
    {
        TxBuf[i + sizeof(arpTx)] = *((uint8_t*)(&arpSend)+i);
    }
    transmit_frame(44);
}

void Eth::arp_answer() //TODO:
{
    for(uint8_t i=0;i<6;i++)
    {frameTx.mac_dest[i] = mac_recieve[i];}
    arp_receivePtr = &arpInit;
    arp_receivePtr->op = swap16(0x0002);
    for(uint8_t i=0;i<6;i++)
    {arp_receivePtr->macaddr_src[i] = mac[i];} // set source mac in ARP
    for(uint8_t i=0;i<4;i++)
    {arp_receivePtr->ip_src[i] = ip[i];} // set source mac in ARP
    for(uint8_t i=0;i<6;i++)
    {arp_receivePtr->macaddr_dst[i] = mac_recieve[i];} // set dest mac in ARP
    for(uint8_t i=0;i<4;i++)
    {arp_receivePtr->ip_dst[i] = ip_receive[i];} // set dest ip in ARP answer

    for(uint8_t i=0; i<sizeof(frameTx);i++)
    {TxBuf[i] = *((uint8_t*)(&frameTx)+i);}
    for(uint8_t i=0; i<sizeof(ARP); i++)
    {TxBuf[i+sizeof(frameTx)] = *((uint8_t*)(arp_receivePtr)+i);}
    transmit_frame(44);
}

void Eth::ip_read()
{
    ip_receivePtr = (IP*)(fRx+1);
    if(ip_receivePtr->prt == IP_ICMP)
    {
        icmp_read();
    }
    //IPflag=true;    
}
void Eth::icmp_read()
{
    uint16_t len = swap16(ip_receivePtr->len) - sizeof(IP) - sizeof(ICMP);
    uint16_t total_len = sizeof(FrameX) + swap16(ip_receivePtr->len);
    icmp_receivePtr = (ICMP*)(ip_receivePtr+1);
    icmp_receivePtr->header_checksum = 0x0000;
    if(icmp_receivePtr->msg_type==ICMP_REQ)
    {       
        /*! need to exchange mac SA and DA*/
        for(uint8_t i=0;i<6;i++)
        {
            fRx->mac_dest[i]=fRx->mac_src[i];
            fRx->mac_src[i] = mac[i];
        }
        /*!<need to reply same message with message_type=ICMP_REPLY and new checksum>*/
        icmp_receivePtr->msg_type = ICMP_REPLY;
        /*!< write eth frame in Tx buf >*/
        for(uint8_t i=0; i<sizeof(FrameX);i++)
        {TxBuf[i]=*((uint8_t*)(fRx)+i);} 
        /*!< need to exchange ip SA and DA>*/
        for(uint8_t i=0;i<4;i++)
        {
            ip_receivePtr->ip_dst[i] = ip_receivePtr->ip_src[i];
            ip_receivePtr->ip_src[i] = ip[i];
        }
        /*!< write ip header in Tx buf >*/
        for(uint8_t i=0; i<sizeof(IP);i++)
        {TxBuf[i+sizeof(FrameX)]=*((uint8_t*)(ip_receivePtr)+i);}
        /*!< write ICMP header in Tx buf >*/ 
        for(uint8_t i=0; i<sizeof(ICMP);i++)
        {TxBuf[i+sizeof(FrameX)+sizeof(IP)]=*((uint8_t*)(icmp_receivePtr)+i);}
        /*!< write ICMP data in Tx buf >*/ 
        for(uint8_t i=0; i<(len);i++)
        {TxBuf[i+sizeof(FrameX)+sizeof(IP)+sizeof(ICMP)]=*((uint8_t*)(icmp_receivePtr+1)+i);}
        transmit_frame(total_len);
    }
    else if(icmp_receivePtr->msg_type==ICMP_REPLY)
    {

    }    
}


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void ETH_IRQHandler(void)
{
    /*!< receive descriptor automatically starts belonging to host >*/
    ETH->DMASR|=ETH_DMASR_RS; //clear bit of interrupt
    Eth::pThis->ReceiveFlag=true;
}

