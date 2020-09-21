constexpr uint16_t ARP_i= 0x0806;
constexpr uint16_t IPv4_i= 0x0800;
constexpr uint16_t IPv6_i= 0x08dd;
//-------------------------------------------
constexpr uint8_t IP_ICMP= 1;
constexpr uint8_t IP_TCP= 6;
constexpr uint8_t IP_UDP=17;
//-------------------------------------------
constexpr uint8_t ICMP_REQ = 8;
constexpr uint8_t ICMP_REPLY = 0; 
//-------------------------------------------
struct FrameX {
	uint8_t mac_dest[6];
	uint8_t mac_src[6];
	uint16_t type;
};
struct ARP {
	uint16_t net;
	uint16_t protocol;
	uint8_t mac_len;
	uint8_t ip_len;
	uint16_t op; //1-request, 2-answer
	uint8_t macaddr_src[6];
	uint8_t ip_src[4];
	uint8_t macaddr_dst[6];
	uint8_t ip_dst[4];
};

struct IP {
  uint8_t verlen; //protocol version and header length
  uint8_t ts; //service type
  uint16_t len; //length
  uint16_t id; //packet identifier
  uint16_t fl_frg_of; //flags and fragment offset
  uint8_t ttl; //time to live
  uint8_t prt; //protocol type
  uint16_t cs; //header check sum
  uint8_t ip_src[4]; //IP-SA
  uint8_t ip_dst[4]; //IP-DA  
};
struct ICMP {
	uint8_t msg_type;
	uint8_t msg_code;
	uint16_t header_checksum;
	uint16_t pack_id;
	uint16_t pack_num;
};
struct UDP {
	uint16_t port_src;
	uint16_t port_dst;
	uint16_t len;
	uint16_t udp_checksum;
};
struct TCP {
  uint16_t port_src;
  uint16_t port_dst;
  uint32_t num_seq;//byte num (pointer to first byte)
  /*!<(first byte + quantity of bytes in segment + 1 or next byte num)>*/
  uint32_t num_ack;//ack number 
  uint8_t len_hdr;//(header length 0xF)*4 with 1 flag on last bit
  uint8_t fl;//flags
  uint16_t size_wnd;//windowsize
  uint16_t cs;//checksum
  uint16_t urg_ptr;//urgent data ptr
}; 
 //--------------------------------------------------

//TCP flags
constexpr uint8_t TCP_CWR = 0x80;
constexpr uint8_t TCP_ECE = 0x40;
constexpr uint8_t TCP_URG = 0x20;
constexpr uint8_t TCP_ACK = 0x10;
constexpr uint8_t TCP_PSH = 0x08;
constexpr uint8_t TCP_RST = 0x04;
constexpr uint8_t TCP_SYN = 0x02;
constexpr uint8_t TCP_FIN = 0x01;
//--------------------------------------------------
//TCP operations
constexpr uint8_t TCP_OP_SYNACK = 1;
constexpr uint8_t TCP_OP_ACK_OF_FIN = 2;
constexpr uint8_t TCP_OP_ACK_OF_RST = 3;
