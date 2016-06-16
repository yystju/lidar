#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <time.h>

#include "receive.h"

#ifdef __cplusplus
extern "C" {
#endif

int receive_sync_data(int argc, char * argv[]) {
	int sin_len;
	
	char buffer[1024 * 8];
	
	int fd;
	
	struct sockaddr_in sin;
	
	const int port = 8308;
	
	bzero(&sin, sizeof(sin));
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin_len = sizeof(sin);
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(fd, (struct sockaddr *)&sin, sizeof(sin));
	
	while(1) {
		recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &sin, &sin_len);
		
		printf("Received packet from %s:%d\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
		printf("RECEIVED \"%s\" \n", buffer);
		
		if(strncmp(buffer, "STOP", 4) == 0) {
			printf("DONE.\n");
			break;
		}
	}
	
	close(fd);
	
	return 0;
}

#ifdef __cplusplus
}
#endif
