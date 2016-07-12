#include "lidar.h"
#include "xsens.h"

#include "raspi.h"
#include "helper.h"

#include <string.h>
#include <unistd.h>

#include <uuid/uuid.h>

void xsensDataProcessor(XsensData * pData) {
    debug("[xsensDataProcessor]>>\n");
    
    debug("pData->year : %d\n", pData->year);
    
    debug("[xsensDataProcessor]<<\n");
}

void lidarDataProcessor(char *p, int start, int len) {
    debug("[lidarDataProcessor]>>\n");
    
    debug("p : %p, start : %d, len %d\n", p, start, len);
    
    debug("[lidarDataProcessor]<<\n");
}

void * xsenThread (void * p) {
    debug("[xsenThread] p : %p\n", p);
    
    //readXsensData(xsensDataProcessor);
    
    pthread_exit(NULL);
}

void * lidar10110Thread (void * p) {
    debug("[lidarThread] p : %p\n", p);
    
// 	LIDAR lidar = lidar_send_init(LIDAR_TIME_PORT);
	
// 	char * str = "HELLO";
	
//     while(1) {
// 		lidar_write_data(lidar, str, 0, strlen(str));
// 		sleep(1);
// 	}
	
// 	lidar_dispose(lidar);
    
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    debug("[START]>>\n");
    
    //http://linux.die.net/man/3/pthread_setaffinity_np

    int num = sysconf(_SC_NPROCESSORS_CONF);
    
    printf("num = %d\n", num);
    
    char str[255];
    uuid_t uu;
    uuid_generate(uu);
    
    uuid_unparse(uu, str);

    printf("UUID : %s\n", str);
    
// 	pthread_t lidar_10110_thread_handler;
// 	pthread_t xsens_thread_handler;
	
// 	pthread_create(&lidar_10110_thread_handler, NULL, lidar10110Thread, (void *)NULL);
// 	bind_thread_cpu(lidar_10110_thread_handler, 0);
	
// 	pthread_create(&xsens_thread_handler, NULL, xsenThread, (void *)NULL);
// 	bind_thread_cpu(xsens_thread_handler, 1);
	
// 	debug("...\n");
	
// 	pthread_join(lidar_10110_thread_handler, (void *)NULL);
// 	pthread_join(xsens_thread_handler, (void *)NULL);

    debug("[START]<<\n");
    
    pthread_exit(NULL);
    return 0;
}

int mainx(int argc, char * argv[]) {
    char buff[1024];
    
    format_gprmc(buff, sizeof(buff), 2016, 6, 5, 10, 59, 00);
    
    puts(buff);
    
    blink(10);
    
    return 0;
}
