#include "lidar.h"
#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif

const int BUFF_SIZE = 1024 * 1024 * 8;

int enableBroadCast(int fd) {
	int broadcastEnable=1;
	return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
}

LIDAR * lidar_send_init(int port) {
	debug("[lidar_send_init]>>\n");
	
	LIDAR * p = (LIDAR *)malloc(sizeof(LIDAR));
	
	memset(&(p->addr),0,sizeof(struct sockaddr_in));
    (p->addr).sin_family = AF_INET;
    (p->addr).sin_addr.s_addr = inet_addr("255.255.255.255");
    (p->addr).sin_port=htons(port);
	
	if ((p->fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		debug("Failed to open socket to UDP %d.", port);
		free(p);
        return NULL;
    }
    
    if(enableBroadCast(fd) < 0) {
		debug("Failed to enable broadcast.");
		free(p);
    	return NULL;
    }
	
    debug("[lidar_send_init]<<\n");
	
	return p;
}

LIDAR * lidar_receive_init(int port) {
	debug("[lidar_receive_init]>>\n");
	
	LIDAR * p = (LIDAR *)malloc(sizeof(LIDAR));
	
	memset(&(p->addr),0,sizeof(struct sockaddr_in));
	
	(p->addr).sin_family = AF_INET;
	(p->addr).sin_addr.s_addr = htonl(INADDR_ANY);
	(p->addr).sin_port = htons(port);
	sin_len = sizeof(struct sockaddr_in);
	
	(p->fd) = socket(AF_INET, SOCK_DGRAM, 0);
	bind((p->fd), (struct sockaddr *)&(p->addr), sizeof(struct sockaddr_in));
	
    debug("[lidar_receive_init]<<\n");
	
	return p;
}

void lidar_dispose(LIDAR * p) {
	debug("[lidar_dispose]>>\n");
	close(p->fd);
	free(p);
	debug("[lidar_dispose]<<\n");
}

void lidar_write_data(LIDAR * p, char * data, int start, int len) {
    debug("[lidar_write_data]>>\n");
	
	if (sendto(p->fd, data, len, start,(struct sockaddr *) &(p->addr), sizeof(struct sockaddr_in)) < 0) {
		debug("Failed to send UDP broadcast packet.");
		return;
	}
	
    debug("[lidar_write_data]<<\n");
}

void lidar_read_data(LIDAR * p, LidarDataProcessor processor) {
    debug("[lidar_read_data]>>\n");
	
    char *buff = malloc(BUFF_SIZE);
    int offset = 0;
	int sin_len = sizeof(struct sockaddr);
	
	recvfrom((p->fd), buff, BUFF_SIZE, 0, (struct sockaddr *) &(p->addr), &sin_len);
    
    if(processor) {
        processor(buff, offset, BUFF_SIZE);
    }
    
    free(buff);
    debug("[lidar_read_data]<<\n");
}

#ifdef __cplusplus
}
#endif