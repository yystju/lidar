#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "raspi.h"
#include "helper.h"
#include "serial.h"

typedef struct {
  char * adapter_name;
  int fd;
  char * pData;
  struct sockaddr_in addr;
} RaspiUDPCallbackData;

typedef struct {
  char * device_name;
  SERIAL serial;
  char * pData;
} RaspiSerialCallbackData;

int UDPRaspiBlinkCallback(void * param);
int SerialRaspiBlinkCallback(void * param);

int runSerial(char * device_name);
int runUDP(char * adapter_name);

int main(int argc, char * argv[]) {
  if(argc < 2) {
    fprintf(stderr, "Usage:\n\t%s serial <serial_device>\n\t%s udp <adapter>\n", argv[0], argv[0]);
    return 1;
  }
	
	if(strcmp("serial", argv[1]) == 0) {//serial
	  return runSerial(argv[2]);
	} else {//udp
	  return runUDP(argv[2]);
	}
	
  return 0;
}

int runSerial(char * device_name) {
  RaspiSerialCallbackData serial_data;
  
  serial_data.device_name = device_name;
	  
  serial_data.serial = serial_open(serial_data.device_name);
  
  if(raspi_blink(-1, SerialRaspiBlinkCallback, (void *)&serial_data) < 0) {
    error("Failed to send blink. error : %s\n", strerror(errno));
    return -3;
  }
  
  serial_close(serial_data.serial);
  
  return 0;
}

int runUDP(char * adapter_name) {
  char broadcast_addr_str[255];
  char sk_buf[1024];
  
  struct ifconf ifc;
  struct ifreq *ifr;
  
  int nInterfaces;
  int i;
  
  RaspiUDPCallbackData udp_data;
  
  memset(&(udp_data),0,sizeof(RaspiUDPCallbackData));
	
  udp_data.adapter_name = adapter_name;
  
  if ((udp_data.fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    error("Failed to open socket to UDP %d.\n", 10110);
    return -1;
  }
  
  ifc.ifc_len = sizeof(sk_buf);
  ifc.ifc_buf = sk_buf;
  
  if(ioctl(udp_data.fd, SIOCGIFCONF, &ifc) < 0) {
  	error("ioctl(SIOCGIFCONF) failed %s.\n", strerror(errno));
    return -2;
  }
  
  ifr = ifc.ifc_req;
  
  debug("nInterfaces = %d / %d \n", ifc.ifc_len, sizeof(struct ifreq));
  
  nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
  
  memset(broadcast_addr_str, '\0', sizeof(broadcast_addr_str));
  
  for(i = 0; i < nInterfaces; i++)
  {
  	struct ifreq *item = &ifr[i];
  	
  	debug("Found Interface. [%s] %s\n", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
  	  
  	if(strcmp(adapter_name, item->ifr_name) == 0) {
  		if(ioctl(udp_data.fd, SIOCGIFHWADDR, item) < 0) {
  			error("ioctl(SIOCGIFHWADDR) failed %s.\n", strerror(errno));
  			return -3;
  		}
  		
  		if(ioctl(udp_data.fd, SIOCGIFBRDADDR, item) >= 0) {
  			strcpy(broadcast_addr_str, inet_ntoa(((struct sockaddr_in *)&item->ifr_broadaddr)->sin_addr));
  			debug("BROADCAST %s\n", broadcast_addr_str);
  		}
  		
  		break;
  	}
  }
  
  if(enableBroadCast(udp_data.fd) < 0) {
    error("Failed to enable broadcast.\n");
    return -2;
  }
  
  (udp_data.addr).sin_family = AF_INET;
  (udp_data.addr).sin_addr.s_addr = inet_addr(broadcast_addr_str);
  (udp_data.addr).sin_port=htons(10110);
  
  udp_data.pData = (char *)malloc(sizeof(char) * 512);
  
  memset(udp_data.pData, '\0', sizeof(char) * 512);

  if(raspi_blink(-1, UDPRaspiBlinkCallback, (void *)&udp_data) < 0) {
    error("Failed to send blink. error : %d\n", errno);
    return -3;
  }
	
  free(udp_data.pData);
    
  close(udp_data.fd);
  
  return 0;
}

int UDPRaspiBlinkCallback(void * param) {
  time_t now;
  struct tm *tm_now;
  
  RaspiUDPCallbackData * p = (RaspiUDPCallbackData *)param;
  
  debug("[UDPRaspiBlinkCallback] %x - %p\n", p->fd, p->pData);

  time(&now);
  tm_now = gmtime(&now);
  memset(p->pData, '\0', sizeof(char) * 512);
	
  format_gprmc(p->pData, 512, tm_now->tm_year, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
  
  return sendto(p->fd, p->pData, strlen(p->pData), 0,(struct sockaddr *) &(p->addr), sizeof(struct sockaddr_in));
}

int SerialRaspiBlinkCallback(void * param) {
  time_t now;
  struct tm *tm_now;
  
  RaspiSerialCallbackData * p = (RaspiSerialCallbackData *)param;
  
  debug("[SerialRaspiBlinkCallback] %p - %p\n", p->serial, p->pData);

  time(&now);
  tm_now = gmtime(&now);
  memset(p->pData, '\0', sizeof(char) * 512);
	
  format_gprmc(p->pData, 512, tm_now->tm_year, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
  
  serial_write(p->serial, p->pData, strlen(p->pData));
  
  return 0;
}

#ifdef __cplusplus
}
#endif