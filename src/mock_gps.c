#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "raspi.h"
#include "helper.h"

int TestRaspiBlinkCallback(void * param);

typedef struct {
  int fd;
  char * pData;
  struct sockaddr_in addr;
} RaspiCallbackData;

int main(int argc, char * argv[]) {
  RaspiCallbackData data;
	
	memset(&(data),0,sizeof(RaspiCallbackData));
	
  (data.addr).sin_family = AF_INET;
  (data.addr).sin_addr.s_addr = inet_addr("255.255.255.255");
  (data.addr).sin_port=htons(10110);
	
	if ((data.fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    error("Failed to open socket to UDP %d.\n", 10110);
    return -1;
  }
  
  if(enableBroadCast(data.fd) < 0) {
	  error("Failed to enable broadcast.\n");
  	return -2;
  }
  
	data.pData = (char *)malloc(sizeof(char) * 1024);
	
	raspi_blink(10, TestRaspiBlinkCallback, (void *)&data);
	
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
  
  debug("[TestRaspiBlinkCallback]\n");
  RaspiCallbackData * p = (RaspiCallbackData *)param;
  
  time(&now);
  tm_now = gmtime(&now);
  memset(p->pData, '\0', sizeof(char) * 1024);
	
	format_gprmc(p->pData, 1024, tm_now->tm_year, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
  
  return sendto(p->fd, p->pData, strlen(p->pData), 0,(struct sockaddr *) &(p->addr), sizeof(struct sockaddr_in));
}
