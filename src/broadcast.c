#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

#include "vlp16.h"


int enableBroadCast(int fd) {
	int broadcastEnable=1;
	return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
}

int main(int argc, char *argv[]) {
    struct sockaddr_in data_addr;
    struct sockaddr_in gps_addr;
    
    int data_fd;
    int gps_fd;
    
    char * temp = "ABCDFGHIJKLMNOPQ";
    
    char *data_message[1249];
    
    char *gps_message = "$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
    
    int i;
    
    /* set up destination address */
    memset(&data_addr,0,sizeof(data_addr));
    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = inet_addr(BROADCAST_GROUP);
    data_addr.sin_port=htons(DATA_PORT);

    memset(&gps_addr,0,sizeof(gps_addr));
    gps_addr.sin_family = AF_INET;
    gps_addr.sin_addr.s_addr = inet_addr(BROADCAST_GROUP);
    gps_addr.sin_port=htons(GPS_PORT);

    if ((data_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    
    if(enableBroadCast(data_fd) < 0) {
    	perror("enable broadcast");
        exit(1);
    }

    if ((gps_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    
    if(enableBroadCast(gps_fd) < 0) {
    	perror("enable broadcast");
        exit(1);
    }
    
    memset(data_message, '\0', 1249);
    
    for(i = 0; i < 1248 / 16; ++i) {
        strncat(&data_message, temp, 16);
    }
    
    printf("%s\n", &data_message);

    while (1) {
        if (sendto(data_fd, data_message, strlen(data_message), 0,(struct sockaddr *) &data_addr, sizeof(data_addr)) < 0) {
            perror("data send");
            exit(1);
        }
        
        if (sendto(gps_fd, gps_message, strlen(gps_message), 0,(struct sockaddr *) &gps_addr, sizeof(gps_addr)) < 0) {
            perror("gps send");
            exit(1);
        }
    }
}
