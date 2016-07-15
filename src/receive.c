#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <time.h>

#include "vlp16.h"

int port = 2368;


int main(int argc, char * argv[]) {
	int sin_len;
	
	char buffer[1249];
	
	int fd;
	
	struct sockaddr_in sin;
	
	bzero(&sin, sizeof(sin));
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin_len = sizeof(sin);
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(fd, (struct sockaddr *)&sin, sizeof(sin));
	
	while(1) {
		memset(buffer, '\0', sizeof(buffer));
		
		recvfrom(fd, buffer, 1248, 0, (struct sockaddr *) &sin, &sin_len);
		
		//printf("Received packet from %s:%d\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
		
		buffer[1248] = '\0';
		
		printf("RECEIVED [%d]\"%s\" \n", strlen(buffer), buffer);
		
		if(strncmp(buffer, "STOP", 4) == 0) {
			printf("DONE.\n");
			break;
		}
	}
	
	close(fd);
	
	return 0;
}
