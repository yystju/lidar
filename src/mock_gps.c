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

int TestRaspiBlinkCallback(void * param);

typedef struct {
  int fd;
  char * pData;
  struct sockaddr_in addr;
} RaspiCallbackData;

int main(int argc, char * argv[]) {
  char broadcast_addr_str[255];
  char sk_buf[1024];
  
  struct ifconf ifc;
  struct ifreq *ifr;
  
  int nInterfaces;
  int i;
  
  RaspiCallbackData data;
	
  memset(&(data),0,sizeof(RaspiCallbackData));

  if ((data.fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    error("Failed to open socket to UDP %d.\n", 10110);
    return -1;
  }
  
  ifc.ifc_len = sizeof(sk_buf);
  ifc.ifc_buf = sk_buf;
  
  if(ioctl(data.fd, SIOCGIFCONF, &ifc) < 0) {
  	error("ioctl(SIOCGIFCONF) failed %s.\n", strerror(errno));
    return -2;
  }
  
  ifr = ifc.ifc_req;
  nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
  
  memset(broadcast_addr_str, '\0', sizeof(broadcast_addr_str));
  
  for(i = 0; i < nInterfaces; i++)
  {
  	struct ifreq *item = &ifr[i];
  	
  	debug("Found Interface. [%s] %s\n", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
  	  
  	if(strcmp(argv[1], item->ifr_name) == 0) {
  		if(ioctl(data.fd, SIOCGIFHWADDR, item) < 0) {
  			error("ioctl(SIOCGIFHWADDR) failed %s.\n", strerror(errno));
  			return -3;
  		}
  		
  		if(ioctl(data.fd, SIOCGIFBRDADDR, item) >= 0) {
  			strcpy(broadcast_addr_str, inet_ntoa(((struct sockaddr_in *)&item->ifr_broadaddr)->sin_addr));
  			debug("BROADCAST %s\n", broadcast_addr_str);
  		}
  		
  		break;
  	}
  }
  
  if(enableBroadCast(data.fd) < 0) {
    error("Failed to enable broadcast.\n");
    return -2;
  }
  
  (data.addr).sin_family = AF_INET;
  (data.addr).sin_addr.s_addr = inet_addr(broadcast_addr_str);
  (data.addr).sin_port=htons(10110);
  
  data.pData = (char *)malloc(sizeof(char) * 512);
  
  memset(data.pData, '\0', sizeof(char) * 512);

  if(raspi_blink(-1, TestRaspiBlinkCallback, (void *)&data) < 0) {
    error("Failed to send blink. error : %d\n", errno);
    return -3;
  }

// 	while(1) {
// 		if (sendto(fd, buff, strlen(buff), 0,(struct sockaddr *) &(addr), sizeof(struct sockaddr_in)) < 0) {
// 			debug("Failed to send UDP broadcast packet.");
// 			return -3;
// 		}
		
// 		usleep(100);
// 	}
	
  free(data.pData);
    
  close(data.fd);
  
  return 0;
}

int TestRaspiBlinkCallback(void * param) {
  time_t now;
  struct tm *tm_now;
  
  RaspiCallbackData * p = (RaspiCallbackData *)param;
  
  debug("[TestRaspiBlinkCallback] %x - %p\n", p->fd, p->pData);

  time(&now);
  tm_now = gmtime(&now);
  memset(p->pData, '\0', sizeof(char) * 512);
	
  format_gprmc(p->pData, 512, tm_now->tm_year, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
  
  return sendto(p->fd, p->pData, 512, 0,(struct sockaddr *) &(p->addr), sizeof(struct sockaddr_in));
}
#ifdef __cplusplus
}
#endif