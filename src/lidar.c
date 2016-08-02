#include "lidar.h"
#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif

//const int BUFF_SIZE = 1024 * 1024 * 8;

typedef struct {
	struct sockaddr_in addr;
	int fd;
	int buff_size;
} LIDAR_IMPL;

int enableBroadCast(int fd) {
	int broadcastEnable=1;
	return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
}

LIDAR lidar_send_init(int port) {
	debug("[lidar_send_init]>>\n");
	
	LIDAR_IMPL * p = (LIDAR_IMPL *)malloc(sizeof(LIDAR_IMPL));
	
	memset(&(p->addr),0,sizeof(struct sockaddr_in));
	
    (p->addr).sin_family = AF_INET;
    (p->addr).sin_addr.s_addr = inet_addr("255.255.255.255");
    (p->addr).sin_port=htons(port);
    
    //TODO: find a way to choose buffer size...
    if(port == LIDAR_DATA_PORT) {
    	p->buff_size = LIDAR_DATA_PACKET_SIZE;
    } else if(port == LIDAR_SYNC_PORT) {
    	p->buff_size = LIDAR_SYNC_PACKET_SIZE;
    } else {
    	p->buff_size = 1024 * 4;
    }
    
    debug("buff_size : %d.\n", p->buff_size);
	
	if ((p->fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error("Failed to open socket to UDP %d.\n", port);
		free(p);
        return NULL;
    }
    
    if(enableBroadCast(p->fd) < 0) {
		error("Failed to enable broadcast.\n");
		free(p);
    	return NULL;
    }
	
    debug("[lidar_send_init]<<\n");
	
	return (LIDAR)p;
}

LIDAR lidar_receive_init(int port) {
	debug("[lidar_receive_init]>>\n");
	
	//int sin_len = sizeof(struct sockaddr_in);
	
	LIDAR_IMPL * p = (LIDAR_IMPL *)malloc(sizeof(LIDAR_IMPL));
	
	memset(&(p->addr),0,sizeof(struct sockaddr_in));
	
	(p->addr).sin_family = AF_INET;
	(p->addr).sin_addr.s_addr = htonl(INADDR_ANY);
	(p->addr).sin_port = htons(port);
	
	//TODO: find a way to choose buffer size...
    if(port == LIDAR_DATA_PORT) {
    	p->buff_size = LIDAR_DATA_PACKET_SIZE;
    } else if(port == LIDAR_SYNC_PORT) {
    	p->buff_size = LIDAR_SYNC_PACKET_SIZE;
    } else {
    	p->buff_size = 1024 * 4;
    }
	
	(p->fd) = socket(AF_INET, SOCK_DGRAM, 0);
	bind((p->fd), (struct sockaddr *)&(p->addr), sizeof(struct sockaddr_in));
	
    debug("[lidar_receive_init]<<\n");
	
	return (LIDAR)p;
}

void lidar_dispose(LIDAR lidar) {
	debug("[lidar_dispose]>>\n");
	LIDAR_IMPL * p = (LIDAR_IMPL *)lidar;
	close(p->fd);
	free(p);
	debug("[lidar_dispose]<<\n");
}

void lidar_write_data(LIDAR lidar, char * data, int start, int len) {
    debug("[lidar_write_data]>> data : %p, start : %d, len : %d\n", data, start , len);
	
	LIDAR_IMPL * p = (LIDAR_IMPL *)lidar;
	
	if (sendto(p->fd, data, len, start,(struct sockaddr *) &(p->addr), sizeof(struct sockaddr_in)) < 0) {
		debug("Failed to send UDP broadcast packet.");
		return;
	}
	
    debug("[lidar_write_data]<<\n");
}

void lidar_read_data(LIDAR lidar, LidarDataProcessor processor, void * param) {
    debug("[lidar_read_data]>>\n");
	
	LIDAR_IMPL * p = (LIDAR_IMPL *)lidar;
	
	debug("[lidar_read_data]>> buff_size : %d\n", p->buff_size);
	
    char *buff = malloc(p->buff_size);
    
    debug("[lidar_read_data]>> buff : %p\n", buff);
    
    int offset = 0;
    
	int sin_len = sizeof(struct sockaddr);
	
	while(1) {
		memset(buff, '\0', p->buff_size);
		
		debug("[lidar_read_data]>> start receive...\n");
		
		int n = recvfrom((p->fd), buff, p->buff_size, 0, (struct sockaddr *) &(p->addr), &sin_len);
		
		debug("[lidar_read_data]>> received %d bytes.\n", n);
	    
	    if(n > 0 && processor) {
	    	debug("[lidar_read_data] run processor %p.\n", processor);
	    	
	        if(!processor(buff, offset, p->buff_size, param)) {
	        	break;
	        }
	    }
	}
    
    free(buff);
    
    debug("[lidar_read_data]<<\n");
}

#ifdef __cplusplus
}
#endif
