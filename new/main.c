#include "lidar.h"
#include "xsens.h"

#include "raspi.h"
#include "helper.h"

#include "repository.h"

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


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
    
    int num = (int)sysconf(_SC_NPROCESSORS_CONF);
    
    debug("num = %d\n", num);
    
    ConfigurePair * pConfig = NULL;
    
    int lines = read_configuration_file("./test.ini", 5, &pConfig);
    
    for(int i = 0; i < lines; ++i) {
        debug("[%s=%s]\n", pConfig[i].key, pConfig[i].value);
    }
    
    const char * dir_name = (const char *)get_configuration(pConfig, lines, "repository.root");
    
    if(!file_exits(dir_name)) {
        mkdir(dir_name, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //Mode 755
    }
    
    char * uuid = get_uuid();
    
    debug("uuid : %s\n", uuid);
    
    char * dir_full_name = (char *) malloc(sizeof(char) * 1024);
    
    strncpy(dir_full_name, dir_name, 1024 - strlen(dir_full_name));
    
    strncat(dir_full_name + strlen(dir_full_name), uuid, 1024 - strlen(dir_full_name));
    
    debug("dir_full_name : %s\n", dir_full_name);
    
    mkdir(dir_full_name, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    
    int offset = strlen(dir_full_name);
    
    strncpy(dir_full_name + offset, "/lidar", 1024 - offset);
    
    debug("lidar file : %s\n", dir_full_name);
    
    
    free((void *) dir_full_name);
    free((void *)uuid);
    free(pConfig);
    
	pthread_t lidar_10110_thread_handler;
	pthread_t xsens_thread_handler;
	
	pthread_create(&lidar_10110_thread_handler, NULL, lidar10110Thread, (void *)NULL);
	bind_thread_cpu(lidar_10110_thread_handler, 0);
	
	pthread_create(&xsens_thread_handler, NULL, xsenThread, (void *)NULL);
	bind_thread_cpu(xsens_thread_handler, 1);
	
	debug("...\n");
	
	pthread_join(lidar_10110_thread_handler, (void *)NULL);
	pthread_join(xsens_thread_handler, (void *)NULL);

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
