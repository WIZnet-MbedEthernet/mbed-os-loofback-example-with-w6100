#include "WIZnetInterface.h"
#include "mbed.h"
#include "ip6string.h"
// Network interface
NetworkInterface *net;

// Socket demo
int main() {
  nsapi_size_or_error_t result;
  
  NetworkInterface *net1;
  nsapi_size_or_error_t send_result;
  nsapi_size_or_error_t r_result;

  #if MBED_CONF_WIZNET_INTERFACE == W5500
        printf("W5500  \r\n");
  #elif MBED_CONF_WIZNET_INTERFACE == W6100
        printf("W6100  \r\n");
  #endif
  // wiznet init
  static uint8_t WIZNET_DEFAULT_TESTMAC[6] = {0x00, 0x08, 0xdc,
                                              0x19, 0x85, 0xa8};
  char Wiz_ip[24] = "192.168.0.6";
  char Wiz_gateway[24] = "192.168.0.1";
  char Wiz_netmask[24] = "255.255.255.0";
  char Wiz_dnsaddr[24] = "8.8.8.8";
  char Wiz_lla[40] = "fe80:0000:0000:0000:0208:dcff:feff:ffff";
  char Wiz_gua[40] = "2001:02b8:0010:0001:0208:dcff:feff:ffff";
  char Wiz_sn6[40] = "ffff:ffff:ffff:ffff:0000:0000:0000:0000";
  char Wiz_gw6[40] = "fe80:0000:0000:0000:0200:87ff:fe08:4c81";
  uint16_t Wiz_TCP_PORT = 5000;
  uint16_t Wiz_TCP_PORT1 = 5001;
  
  static WIZnetInterface eth1(WIZNET_MOSI, WIZNET_MISO, WIZNET_SCK, WIZNET_CS,
                              WIZNET_RESET);

  printf("Mbed OS Echo ipV4, ipV6 example\n");
  #ifdef MBED_MAJOR_VERSION
  printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION,
         MBED_MINOR_VERSION, MBED_PATCH_VERSION);
  #endif

  printf("Network info \r\n");
  printf("MAC     : %2X:%2X:%2X:%2X:%2X:%2X \r\n", WIZNET_DEFAULT_TESTMAC[0], WIZNET_DEFAULT_TESTMAC[1],
   WIZNET_DEFAULT_TESTMAC[2], WIZNET_DEFAULT_TESTMAC[3], WIZNET_DEFAULT_TESTMAC[4], WIZNET_DEFAULT_TESTMAC[5]);
  printf("ip      : %s \r\n", Wiz_ip);
  printf("gateWay : %s \r\n", Wiz_gateway);
  printf("netmask : %s \r\n", Wiz_netmask);
  printf("dns     : %s \r\n", Wiz_dnsaddr);
  printf("LLA     : %s \r\n", Wiz_lla);
  printf("GUA     : %s \r\n", Wiz_gua);
  printf("SN6     : %s \r\n", Wiz_sn6);
  printf("GW6     : %s \r\n", Wiz_gw6);
  eth1.init(WIZNET_DEFAULT_TESTMAC, Wiz_ip, Wiz_netmask, Wiz_gateway);
  eth1.setDnsServerIP(Wiz_dnsaddr);
  eth1.init_ipv6(Wiz_lla, Wiz_gua, Wiz_sn6, Wiz_gw6);

  net1 = &eth1;

  TCPSocket TCP_socketV4;
  TCPSocket TCP_socketV6;

  printf("TCP Socket ipV6 open \n");
  eth1.setMode(TCP6);
  TCP_socketV6.open(net1);
  printf("TCP Socket ipV4 open \n");
  TCP_socketV4.open(net1);
  
  printf("TCP Socket ipV6 bind port[%d] \n", Wiz_TCP_PORT);
  TCP_socketV6.bind(Wiz_TCP_PORT);

  printf("TCP Socket ipV4 bind port[%d] \n", Wiz_TCP_PORT1);
  TCP_socketV4.bind(Wiz_TCP_PORT1);

  int TCP_backlog;
  TCP_socketV6.listen(TCP_backlog);
  TCP_socketV4.listen(TCP_backlog);

  char TCP_bufferV4[256];
  char TCP_bufferV6[256];
  while (1) {
    r_result = TCP_socketV4.recv(TCP_bufferV4, sizeof(TCP_bufferV4));
    if(r_result > 0 )
    {
      TCP_socketV4.send(TCP_bufferV4, r_result);
      printf("_____________________________ \r\n");
      printf("TCP V4 Recv[%d]: %s \r\n",r_result, TCP_bufferV4);
    }
    r_result = TCP_socketV6.recv(TCP_bufferV6, sizeof(TCP_bufferV6));
    if(r_result > 0 )
    {
      TCP_socketV6.send(TCP_bufferV6, r_result);
      printf("_____________________________ \r\n");
      printf("TCP V6 Recv[%d]: %s \r\n",r_result, TCP_bufferV6);
    }
  }
}
