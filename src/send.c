#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LOOP_COUNT 20

int port = 1234;

int main(int argc, char * argv[]) {
	int fd;
	
	char buffer[1024 * 8];
	
	struct sockaddr_in address;
	
	int i;
	
	bzero(&address, sizeof(address));
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port=htons(port);
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	for(i = 0; i < LOOP_COUNT; ++i) {
		sprintf(buffer, "DATA %d\n", i);
		
		sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, sizeof(address));
	}
	
	sprintf(buffer, "STOP\n");
	sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, sizeof(address));
	
	close(fd);
	
	printf("Done.\n");
	
	return 0;
}