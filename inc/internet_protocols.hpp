struct FrameRx
{
	uint8_t mac_dest[6];
	uint8_t mac_src[6];
	uint16_t type;
};
struct FrameTx
{
	uint8_t mac_dest[6];
	uint8_t mac_src[6];
	uint16_t type;
};
struct ARP
{
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

constexpr uint8_t IP_ICMP= 1;
constexpr uint8_t IP_TCP= 6;
constexpr uint8_t IP_UDP=17;

struct IP_pkt 
{
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
  uint8_t data[]; //data
};

