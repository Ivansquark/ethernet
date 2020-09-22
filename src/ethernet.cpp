#include "ethernet.hpp"

uint32_t Eth::ReceiveDL[4]={0};
uint32_t Eth::TransmitDL[4]={0}; //static for creating not in stack

Eth* Eth::pThis = nullptr;

Eth::Eth(uint8_t* rxB,uint8_t* txB) {
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

void Eth::eth_init() {
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
uint8_t Eth::smi_read(uint8_t reg_num) {
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
void Eth::smi_write(uint8_t reg_num, uint8_t val) {
    ETH->MACMIIAR|=ETH_MACMIIAR_MW;//write ready
    ETH->MACMIIAR|=(reg_num&0x1F)<<6; //write number of PHY register in reg 
    ETH->MACMIIAR|=ETH_MACMIIAR_MB;//to start
    ETH->MACMIIDR=val<<8;
    while(ETH->MACMIIAR & ETH_MACMIIAR_MB);    
}
//--------------------------------------------------------------------------------------------------------
void Eth::descr_init() {
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
void Eth::receive_frame() {	
	//ReceiveDL[0] |= (1<<31); //sets OWN bit to DMA	
}
void Eth::transmit_frame(uint16_t size) {
    TransmitDL[1] = (size); //size of Tx first buffer
	TransmitDL[0] |= (1<<31); //sets OWN bit to DMA
    ETH->DMAOMR |= ETH_DMAOMR_ST; //start transmittion (starts DMA polling)	
	ETH->DMAOMR &=~ ETH_DMAOMR_ST; //(ends DMA polling)
}
//--------------------------------------------------------------
void Eth::frame_read() {
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
//------------------------------------------------------------------------
void Eth::arp_read() {
    arp_receivePtr = (ARP*)(fRx+1);    
    if((fRx->mac_dest[0]==0xff || fRx->mac_dest[0]==mac[0]) && 
       (fRx->mac_dest[1]==0xff || fRx->mac_dest[1]==mac[1]) &&
       (fRx->mac_dest[2]==0xff || fRx->mac_dest[2]==mac[2]) &&
       (fRx->mac_dest[3]==0xff || fRx->mac_dest[3]==mac[3]) &&
       (fRx->mac_dest[4]==0xff || fRx->mac_dest[4]==mac[4]) &&
       (fRx->mac_dest[5]==0xff || fRx->mac_dest[5]==mac[1]) && 
        arp_receivePtr->ip_dst[0]==ip[0] && arp_receivePtr->ip_dst[1]==ip[1] &&
        arp_receivePtr->ip_dst[2]==ip[2] && arp_receivePtr->ip_dst[3]==ip[3]) //recieve broadcast need to send back mac
    {
        for(uint8_t i=0;i<6;i++)
        {
            mac_receive[i] = fRx->mac_src[i];                
            arp_receivePtr->macaddr_dst[i] = mac_receive[i]; 
            //fRx->mac_src[i] = mac_receive[i];                
        }
        for(uint8_t i=0;i<4;i++)
        {ip_receive[i] = arp_receivePtr->ip_src[i];} //write incoming ip address of requesting PC
        arp_answer();
    }
    else if(fRx->mac_dest[0]==mac[0] && fRx->mac_dest[1]==mac[1] &&
            fRx->mac_dest[2]==mac[2] && fRx->mac_dest[3]==mac[3] &&
            fRx->mac_dest[4]==mac[4] && fRx->mac_dest[5]==mac[5]) //if answer (after send arp from micro) 
    { 
        for(uint8_t i=0; i<6; i++)
        { mac_receive[i] =  fRx->mac_dest[i];}
        arp_answer();                       
    }      
}
void Eth::arp_send() {
    FrameX arpTx = {0xff,0xff,0xff,0xff,0xff,0xff,0x32,0x12,0x56,0x78,0x9a,0xbc,swap16(0x0806)};    
    for(uint8_t i=0; i<sizeof(arpTx);i++)
    {TxBuf[i] = *((uint8_t*)(&arpTx)+i);}
    for(uint8_t i=0; i<sizeof(arpSend);i++)
    {
        TxBuf[i + sizeof(arpTx)] = *((uint8_t*)(&arpSend)+i);
    }
    transmit_frame(44);
}
void Eth::arp_answer() {
    for(uint8_t i=0;i<6;i++)
    {frameTx.mac_dest[i] = mac_receive[i];}
    arp_receivePtr = &arpInit;
    arp_receivePtr->op = swap16(0x0002);
    for(uint8_t i=0;i<6;i++)
    {arp_receivePtr->macaddr_src[i] = mac[i];} // set source mac in ARP
    for(uint8_t i=0;i<4;i++)
    {arp_receivePtr->ip_src[i] = ip[i];} // set source mac in ARP
    for(uint8_t i=0;i<6;i++)
    {arp_receivePtr->macaddr_dst[i] = mac_receive[i];} // set dest mac in ARP
    for(uint8_t i=0;i<4;i++)
    {arp_receivePtr->ip_dst[i] = ip_receive[i];} // set dest ip in ARP answer

    for(uint8_t i=0; i<sizeof(frameTx);i++)
    {TxBuf[i] = *((uint8_t*)(&frameTx)+i);}
    for(uint8_t i=0; i<sizeof(ARP); i++)
    {TxBuf[i+sizeof(frameTx)] = *((uint8_t*)(arp_receivePtr)+i);}
    transmit_frame(44);
}
//------------------------------------------------------------------------
void Eth::ip_read() {
    ip_receivePtr = (IP*)(fRx+1);
    if(ip_receivePtr->prt == IP_ICMP)
    {
        icmp_read();
    }
    else if (ip_receivePtr->prt == IP_UDP)
    {
        udp_read();
    }
    else if (ip_receivePtr->prt == IP_TCP)
    {
        tcp_read();
    }
    //IPflag=true;    
}
//----------------------------------------------------------------------------
void Eth::icmp_read() {
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
    {    }    
}
void Eth::icmp_write() {
    FrameX* fTx = (FrameX*)TxBuf;
    //for(uint8_t i=0;i<6;i++){fTx->mac_dest[i]=mac_receive[i];}
    fTx->mac_dest[0]=0x20;fTx->mac_dest[1]=0x1a; fTx->mac_dest[2]=0x06;
    fTx->mac_dest[3]=0x7f;fTx->mac_dest[4]=0xd6; fTx->mac_dest[5]=0xb6;    
    for(uint8_t i=0;i<6;i++){fTx->mac_src[i]=mac[i];}
    fTx->type = swap16(IPv4_i);
    IP* ipPtr = (IP*)(fTx+1);
    ipPtr->verlen = 0x45; ipPtr->ts=0;
    ipPtr->len = swap16(sizeof(IP)+sizeof(ICMP)+1);
    ipPtr->id = swap16(0x0001);
    ipPtr->fl_frg_of = swap16(0x0000);
    ipPtr->ttl = 64; ipPtr->prt = IP_ICMP;
    for(uint8_t i=0;i<4;i++){ipPtr->ip_src[i] = ip[i];}
    //for(uint8_t i=0;i<4;i++){ipPtr->ip_dst[i] = ip_receive[i];}
    ipPtr->ip_dst[0] = 5;ipPtr->ip_dst[1] = 255;
    ipPtr->ip_dst[2] = 255; ipPtr->ip_dst[3] = 50;
    ICMP* icmpPtr = (ICMP*)(ipPtr+1);
    icmpPtr->msg_type = ICMP_REQ; icmpPtr->msg_code=0;
    icmpPtr->pack_id = swap16(0x0001);icmpPtr->pack_num = swap16(0x0001);
    uint8_t* data = (uint8_t*)(icmpPtr+1);
    data[0]=0xFF;
    uint16_t len = sizeof(FrameX) + sizeof(IP)+sizeof(ICMP)+1;
    transmit_frame(len);
}
//--------------------------------------------------------------------
void Eth::udp_read() {
    udp_receivePtr = (UDP*)(ip_receivePtr+1);
    if(udp_receivePtr->port_dst == swap16(udp_port))
    {
        udp_initReply();
        UDPflag = true;
        uint8_t arr[]="jopa";
        udp_writeReply(arr,5);
    }
}
void Eth::udp_initReply() {
    /*!< save received frame information for deferred reply >*/
    for(uint8_t i=0; i<sizeof(FrameX); i++)
    {*((uint8_t*)&frameRx+i) = RxBuf[i];}
    /*! need to exchange mac SA and DA*/
    for(uint8_t i=0;i<6;i++)
    {
        frameRx.mac_dest[i]=frameRx.mac_src[i];
        frameRx.mac_src[i] = mac[i];
    }
    /*!< save received IP information for deferred reply >*/
    for(uint8_t i=0; i<sizeof(IP); i++)
    {*((uint8_t*)&IP_received+i) = RxBuf[i+sizeof(FrameX)];}
    /*!< need to exchange ip SA and DA>*/
    for(uint8_t i=0;i<4;i++)
    {
        IP_received.ip_dst[i] = IP_received.ip_src[i];
        IP_received.ip_src[i] = ip[i];
    }
    /*!< save received UDP information for deferred reply >*/
    for(uint8_t i=0; i<sizeof(UDP); i++)
    {*((uint8_t*)&UDP_received+i) = RxBuf[i+sizeof(FrameX)+sizeof(IP)];}
    /*!< need to exchange UDP ports >*/
    uint16_t bufPort = UDP_received.port_src;
    UDP_received.port_src = UDP_received.port_dst;
    UDP_received.port_dst = bufPort;
    /*!< save received UDP data information for deferred reply >*/
    for(uint8_t i=0; i<(swap16(UDP_received.len)-sizeof(UDP)); i++)
    {*((uint8_t*)((UDP*)&UDP_data)+i) = RxBuf[i+sizeof(FrameX)+sizeof(IP)+sizeof(UDP)];}
}
void Eth::udp_writeReply(uint8_t* data,uint16_t len) {
    uint16_t ip_len = sizeof(IP) + sizeof(UDP) + len;
    uint16_t udp_len = sizeof(UDP) + len;
    uint16_t length = sizeof(FrameX)+sizeof(IP)+sizeof(UDP)+len; 
    IP_received.len = swap16(ip_len); 
    UDP_received.len = swap16(udp_len);
    for (uint8_t i=0;i<sizeof(FrameX);i++)
    {TxBuf[i] = *((uint8_t*)(&frameRx)+i);}
    for (uint8_t i=0;i<sizeof(IP);i++)
    {TxBuf[i+sizeof(FrameX)] = *((uint8_t*)(&IP_received)+i);}        
    for (uint8_t i=0;i<sizeof(UDP);i++)
    {TxBuf[i+sizeof(FrameX)+sizeof(IP)] = *((uint8_t*)(&UDP_received)+i);}
    for (uint8_t i=0;i<len;i++)
    {TxBuf[i+sizeof(FrameX)+sizeof(IP)+sizeof(UDP)] = *((uint8_t*)(data)+i);}
    transmit_frame(length);    
}
void Eth::udp_write(uint8_t* data,uint16_t len, uint16_t port) {
    uint16_t ip_len = sizeof(IP) + sizeof(UDP) + len;
    uint16_t udp_len = sizeof(UDP) + len;
    uint16_t length = sizeof(FrameX)+sizeof(IP)+sizeof(UDP)+len; 
    IP_received.len = swap16(ip_len); 
    UDP_received.len = swap16(udp_len);
    //for (uint8_t i=0;i<sizeof(FrameX);i++)
    //{TxBuf[i] = *((uint8_t*)(&frameRx)+i);}
    //for (uint8_t i=0;i<sizeof(IP);i++)
    //{TxBuf[i+sizeof(FrameX)] = *((uint8_t*)(&IP_received)+i);}        
    //for (uint8_t i=0;i<sizeof(UDP);i++)
    //{TxBuf[i+sizeof(FrameX)+sizeof(IP)] = *((uint8_t*)(&UDP_received)+i);}
    //for (uint8_t i=0;i<len;i++)
    //{TxBuf[i+sizeof(FrameX)+sizeof(IP)+sizeof(UDP)] = *((uint8_t*)(data)+i);}
    transmit_frame(length);    
}



//------------------*****************----------------------------------------------------
//---------------------*** TCP ***-------------------------------------------------------
//---------------------***********-------------------------------------------------------
void Eth::tcp_read() {
    tcp_receivePtr = (TCP*)(ip_receivePtr+1);
    //uint8_t TCP_header_len = ((tcp_receivePtr->len_hdr)>>4)<<2;//shift on 4 and multiply by 4
    //uint16_t recived_TCP_data_len = swap16(ip_receivePtr->len) - sizeof(IP) - TCP_header_len;

    if((tcp_receivePtr->fl)==(TCP_SYN)) { //if start connection 
        tcp_initReply((TCP_SYN | TCP_ACK),0);
        tcp_reply(0,true);
        TCPconnected = true;
    }  
    else if (tcp_receivePtr->fl==(TCP_FIN|TCP_ACK)) { //if starts disconnection from remote host
        TCPconnected = false;
        tcp_initReply(TCP_ACK,0);
        tcp_reply(0,true);
        tcp_initReply(TCP_FIN|TCP_ACK,0);
        tcp_reply(0,true);
    }  
    else if (tcp_receivePtr->fl==(TCP_ACK)) { // if got acknowledgement packet
        //TODO: send usfull data with TCP_ACK 
        tcp_initReply(TCP_ACK,0);
    }
    else if (tcp_receivePtr->fl==(TCP_RST)) { // if reset from remote host
        //nothing here
    }
    else if (tcp_receivePtr->fl==(TCP_PSH | TCP_ACK)) { // if remote host require to handling all data now and send ACK
        tcp_initReply(TCP_ACK,0);        
        tcp_reply(0,true);
    }
}
void Eth::tcp_initReply(uint8_t flags, uint16_t TCP_data_len) {
    /*!< save received frame information for deferred reply >*/
    for(uint8_t i=0; i<sizeof(FrameX); i++)
    {*((uint8_t*)&frameRx+i) = RxBuf[i];}
    /*! need to exchange mac SA and DA*/
    for(uint8_t i=0;i<6;i++)
    {
        frameRx.mac_dest[i]=frameRx.mac_src[i];
        frameRx.mac_src[i] = mac[i];
    }    
//-----------------------------------------------------------------------------
    /*!< save received IP information for deferred reply >*/
    for(uint8_t i=0; i<sizeof(IP); i++)
    {*((uint8_t*)&IP_received+i) = RxBuf[i+sizeof(FrameX)];}
    /*!< need to exchange ip SA and DA>*/
    for(uint8_t i=0;i<4;i++) {
        IP_received.ip_dst[i] = IP_received.ip_src[i];
        IP_received.ip_src[i] = ip[i];
    }
    if(flags == (TCP_SYN | TCP_ACK) ) { // if connection flags
        //IP_received.len = swap16(swap16(ip_receivePtr->len)+4);//new IP length
        IP_received.len = swap16(sizeof(IP)+sizeof(TCP)+4);
    } else {
        if(TCP_data_len) { // if need to insert data
            IP_received.len = swap16(sizeof(IP)+sizeof(TCP)+TCP_data_len);
        } else { //if no data (ACK reply)
            IP_received.len = swap16(sizeof(IP)+sizeof(TCP)); //ACK reply len
        }        
    }
//------------------------------------------------------------------------------------------------------------
        /*!< save received TCP information for deferred reply >*/
    uint8_t tcp_header_len = (tcp_receivePtr->len_hdr>>4)<<2; // multiply by four //TODO: if receive header with options?need to examinate it first of all   
    TCP_received_data_len = swap16(ip_receivePtr->len)-sizeof(IP) - tcp_header_len;//segment len
    for(uint8_t i=0; i<tcp_header_len; i++)
    {*((uint8_t*)&TCP_received+i) = RxBuf[i+sizeof(FrameX)+sizeof(IP)];}
    /*!< need to exchange TCP ports >*/
    uint16_t bufPort = TCP_received.port_src;
    TCP_received.port_src = TCP_received.port_dst;
    TCP_received.port_dst = bufPort;
//-------------------------------------------------------------------------------------------------------------------------
    /*!< need to change bytes numbers in TCP header >*/
    uint32_t num_ackR;
    uint32_t num_seqR;
    if(flags == (TCP_SYN | TCP_ACK) ){
        num_ackR = swap32(tcp_receivePtr->num_seq) +1; //write sequence num in ack num adding one (in SYN)
        num_seqR = swap32(tcp_receivePtr->cs); //RAND (associated with working system time not nescessary in mc)
    } else if (flags == (TCP_FIN | TCP_ACK)) {
        num_ackR = swap32(tcp_receivePtr->num_seq); //write sequence num in ack num adding none (in FIN|ACK)
        num_seqR = swap32(tcp_receivePtr->num_ack); //rand (associated with working system time not nescessary in mc)
    } else if(flags == (TCP_PSH | TCP_ACK) ) { // if need to push data on remote host
        ////////////
    } else if(flags == TCP_ACK) { // if received data or reply for FIN|ACK 
        if(tcp_receivePtr->fl == (TCP_FIN|TCP_ACK) ) { //for client
            num_ackR = swap32(tcp_receivePtr->num_seq)+1; //write sequence num in ack num adding 1 (if FIN|ACK)
            num_seqR = swap32(tcp_receivePtr->num_ack); //rand (associated with working system time not nescessary in mc)
        } else {
            num_ackR = swap32(tcp_receivePtr->num_seq) + TCP_received_data_len; //write sequence num in ack num adding data bytes if exists
            num_seqR = swap32(tcp_receivePtr->num_ack); //rand (associated with working system time not nescessary in mc)
        }
    }    
    TCP_received.num_ack = swap32(num_ackR);
    TCP_received.num_seq = swap32(num_seqR); //in reply it will added by number of bytes
    TCP_received.fl = flags; // reply on SYN request
    TCP_received.size_wnd = swap16(8192); //window size (will decrease by each packet)
//-------------------------------------------------------------------------------------------------
    /*!< save received TCP data information for deferred reply >*/     
    for(uint8_t i=0; i<TCP_received_data_len; i++) {
        TCP_data_receive[i] = RxBuf[i+sizeof(FrameX)+sizeof(IP)+tcp_header_len];
        //*((uint8_t*)&TCP_data_transmit+i) = RxBuf[i+sizeof(FrameX)+sizeof(IP)+tcp_header_len];
    }
//------------------------------------------------------------------------
    /*!<--------------(NOT) sliding window ------------------->*/
    //TODO: study out sliding window and overflowing
    if((tcp_receivePtr->fl == (TCP_PSH | TCP_ACK)) || (swap16(tcp_receivePtr->size_wnd)<1410)) {
        tcp_receivePtr->size_wnd = swap16(0x2000);
    } else {
        tcp_receivePtr->size_wnd = swap16(swap16(tcp_receivePtr->size_wnd) - TCP_received_data_len);
    }      
//------------------------------------------------------------------------------------------------
/*!< + options >*/ //if no options this data will not sending because header length is not incude this data
    if(tcp_receivePtr->fl==(TCP_SYN)) {
        /*! <increasing for 4 bytes to send options> only in connection!!! */
        TCP_received.len_hdr = ((sizeof(TCP) + 4)>>2)<<4; //new header length + 4 bytes
        TCP_data_transmit[0]=2;//Maximum Segment Size (2)    
        TCP_data_transmit[1]=4;//Length
        TCP_data_transmit[2]=0x05;
        TCP_data_transmit[3]=0x82; //0x0582 = 1410 - size of segment
    }
     
}
void Eth::tcp_reply(uint16_t TCP_data_len, bool reply) {
    uint8_t TCP_header_len = (TCP_received.len_hdr>>4)<<2;
    uint16_t len = sizeof(FrameX) + sizeof(IP) + TCP_header_len + TCP_data_len;
    if(!reply) { //if not reply than just transmit
        IP_received.len = swap16(swap16(IP_received.len)+TCP_data_len);
    }    
    for (uint8_t i=0;i<sizeof(FrameX);i++)
    {TxBuf[i] = *((uint8_t*)(&frameRx)+i);}
    for (uint8_t i=0;i<sizeof(IP);i++)
    {TxBuf[i+sizeof(FrameX)] = *((uint8_t*)(&IP_received)+i);}        
    for (uint8_t i=0;i<sizeof(TCP);i++)
    {TxBuf[i+sizeof(FrameX)+sizeof(IP)] = *((uint8_t*)(&TCP_received)+i);}
    if(!reply) {
        for (uint8_t i=0;i<TCP_data_len;i++)
        {TxBuf[i+sizeof(FrameX)+sizeof(IP)+sizeof(TCP)] = *((uint8_t*)(&TCP_data_transmit)+i);}
    } else {
        for (uint8_t i=0;i<4;i++)
        {TxBuf[i+sizeof(FrameX)+sizeof(IP)+sizeof(TCP)] = *((uint8_t*)(&TCP_data_transmit)+i);}
    }
    transmit_frame(len);
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void ETH_IRQHandler(void) {
    /*!< receive descriptor automatically starts belonging to host >*/
    ETH->DMASR|=ETH_DMASR_RS; //clear bit of interrupt
    Eth::pThis->ReceiveFlag=true;
}

