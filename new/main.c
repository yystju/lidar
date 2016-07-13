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

    int num = (int)sysconf(_SC_NPROCESSORS_CONF);
    
    debug("num = %d\n", num);
    
    char str[255];
    uuid_t uu;
    uuid_generate(uu);
    
    uuid_unparse(uu, str);

    debug("UUID : %s\n", str);
    
	pthread_t lIDAR_10110_THREAD_HANDLER;
	PTHREAD_T XSENS_THREAD_HANDLER;
	
	PTHREAD_CREATE(&LIDAR_10110_THREAD_HANDLER, NULL, LIDAR10110THREAD, (VOID *)NULL);
	BIND_THREAD_CPU(LIDAR_10110_THREAD_HANDLER, 0);
	
	PTHREAD_CREATE(&XSENS_THREAD_HANDLER, NULL, XSENTHREAD, (VOID *)NULL);
	BIND_THREAD_CPU(XSENS_THREAD_HANDLER, 1);
	
	DEBUG("...\N");
	
	PTHREAD_JOIN(LIDAR_10110_THREAD_HANDLER, (VOID *)NULL);
	PTHREAD_JOIN(XSENS_THREAD_HANDLer, (void *)NULL);

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
