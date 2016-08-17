#include "lidar.h"
#include "xsens.h"

#include "raspi.h"
#include "helper.h"

#include "serial.h"

#include "repository.h"

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <signal.h>

static int done = 0;
static int is_emmit = 0; /*bool variable : 0 -- false, 1 -- true*/
static int last_second = 0;
static char grmc_buff[1024];

//[xsens]
typedef struct {
    FILE * fp;
} XsensProcessorData;

int xsensDataProcessor(XsensData * pData, void * param) {
    if(near_zero(pData->euler_roll) && near_zero(pData->euler_pitch) && near_zero(pData->euler_yaw)) return !done;
    
    debug("[xsensDataProcessor]>> param : %p\n", param);
    
    XsensProcessorData * data = (XsensProcessorData *)param;
    
    debug("quaternion : [%f %f %f %f]\n", pData->quaternion_w, pData->quaternion_x, pData->quaternion_y, pData->quaternion_z);
    
    debug("euler : [%f %f %f]\n", pData->euler_roll, pData->euler_pitch, pData->euler_yaw);
    
    debug("date : %04d-%02d-%02d %02d:%02d:%02d\n", pData->year, pData->month, pData->day, pData->hour, pData->minute, pData->second);
    
    if(pData->year > 0) {

		debug("## year > 0 :: is_emmit : %s, second : %d, last_second : %d \n", is_emmit ? "true" : "false", pData->second, last_second);

		if(!is_emmit && pData->second != last_second) {//need to emmit;
		  debug("## NEED TO EMMIT\n");
			
			memset(grmc_buff, '\0', sizeof(char) * 1024);

			format_gprmc(grmc_buff, 1024, pData->year, pData->month, pData->day, pData->hour, pData->minute, pData->second);
			
			strncat(grmc_buff, "\n", 1);
			
			is_emmit = 1;
		}

		last_second = pData->second;
    }
    
    debug("## pData : %p\n", pData, sizeof(XsensData));
    
    if(data->fp && pData) {
        fwrite((char *)pData, sizeof(XsensData), 1, data->fp);
        fflush(data->fp);
    }
    
    debug("[xsensDataProcessor]<<\n");
    
    return !done;
}

typedef struct {
    Configuration * configurations;
    FILE * fp;
} XsensThreadData;

void * xsenThread (void * p) {
    debug("[xsenThread] p : %p\n", p);
    
    XsensThreadData * data = (XsensThreadData *)p;
    
    const char * ins_device = (const char *)get_configuration(data->configurations, "ins.device");
    
    debug("ins_device : %s\n", ins_device);
    
    XsensProcessorData processorData;
		
		processorData.fp = data->fp;

    debug("## processorData.fp : %p\n", processorData.fp);
    
    readXsensData(ins_device, xsensDataProcessor, &processorData);
    
    pthread_exit(NULL);
}

//[lidar 2368]
int lidar2368DataProcessor(char *p, int start, int len, void * param) {
    debug("[lidar2368DataProcessor]>> param : %p\n", param);
    
    FILE * fp = (FILE *) param;
    
    debug("p : %p, start : %d, len %d\n", p, start, len);
    
    if(fp && p) {
        fwrite((char *)(p + start), sizeof(char), len, fp);
        fflush(fp);
    }
    
    debug("[lidar2368DataProcessor]<<\n");
    
    return !done;
}

//[lidar 10110]
typedef struct {
    Configuration * configurations;
} LiDAR10110ThreadData;

void * lidar10110Thread (void * p) {
    debug("[lidar10110Thread] p : %p\n", p);
	
    LiDAR10110ThreadData * data = (LiDAR10110ThreadData *)p;
	
    const char * lidar_type = (const char *)get_configuration(data->configurations, "lidar.type");
    
    debug("lidar_type : %s\n", lidar_type);
    
    const char * lidar_serial_device = (const char *)get_configuration(data->configurations, "lidar.serial.device");
    
    debug("lidar_serial_device : %s\n", lidar_serial_device);
    
    const char * lidar_udp_adapter = (const char *)get_configuration(data->configurations, "lidar.udp.adapter");
    
    debug("lidar_udp_adapter : %s\n", lidar_udp_adapter);
	
    LIDAR lidar = NULL;
    SERIAL serial = NULL;

		debug("## Start to init serial or lidar time udp port.\n");
	
    if(strcmp("serial", lidar_type) == 0) {
        debug("[will send GPRMC via serial]\n");
        
        serial = serial_open(lidar_serial_device);
        
        lidar = NULL;
    } else {
        debug("[will send GPRMC via 10110 UDP port]\n");
        
        serial = NULL;
        
        lidar = lidar_send_init(lidar_udp_adapter, LIDAR_TIME_PORT);
    }
	
	debug("## start 10110 loop\n");
	
	while(1) {
		if(done) break;

		if(is_emmit) {
			debug("## Start to emmit.\n");

			usleep(100);
			
			if(serial) {
				serial_write(serial, grmc_buff, strlen(grmc_buff));
			}
			
			if(lidar) {
				lidar_write_data(lidar, grmc_buff, 0, strlen(grmc_buff));
			}
			
			is_emmit = 0;
		}
		usleep(100);
	}

	debug("## Out of 10110 loop.\n");
	
	if(serial) {
        serial_close(serial);
    }
    
    if(lidar) {
        lidar_dispose(lidar);
    }
	
    pthread_exit(NULL);
}

//[lidar 2368]
void * lidar2368Thread (void * p) {
	debug("[lidar2368Thread] p : %p\n", p);
	
	FILE * fp = (FILE *)p;
    
	LIDAR lidar = lidar_receive_init(LIDAR_DATA_PORT);
	
	lidar_read_data(lidar, lidar2368DataProcessor, (void *)fp);
	
	lidar_dispose(lidar);
    
	pthread_exit(NULL);
}

//[lidar 8308]
int lidar8308DataProcessor(char *p, int start, int len, void * param) {
    debug("[lidar8308DataProcessor]>> param : %p\n", param);
    
    FILE * fp = (FILE *) param;
    
    debug("p : %p, start : %d, len %d\n", p, start, len);
    
    if(fp && p) {
        fwrite((char *)(p + start), sizeof(char), len, fp);
        fflush(fp);
    }
    
    debug("[lidar8308DataProcessor]<<\n");
    
    return !done;
}


void * lidar8308Thread (void * p) {
    debug("[lidar8308Thread] p : %p\n", p);
    
    FILE * fp = (FILE *)p;
    
	LIDAR lidar = lidar_receive_init(LIDAR_SYNC_PORT);
	
    lidar_read_data(lidar, lidar8308DataProcessor, (void *)fp);
	
	lidar_dispose(lidar);
    
    pthread_exit(NULL);
}

/**
 * Signal Handler for deamon mode.
 * When SIGQUIT | SIGINT, the flag "done" will be set.
 */
void daemon_exit_handler(int sig) {
    switch (sig) {
    case SIGQUIT:
        debug("[daemon_exit_handler] SIGQUIT ...\n");
        done = 1;
        break;
    case SIGINT:
        debug("[daemon_exit_handler] SIGINT ...\n");
        done = 1;
        break;
    default:
        debug("[daemon_exit_handler] unhandled signal {%d}\n", sig);
        break;
    }
}

int start(int is_daemon) {
    debug("[START]>>\n");
    
    //Init...
    
    if(is_daemon) {
        init_deamon_process();
    }
    
    int cpu_core_count = (int)sysconf(_SC_NPROCESSORS_CONF);
    
    debug("cpu_core_count = %d\n", cpu_core_count);
    
    Configuration * configurations = read_configuration_file("./main.ini", 10);
    
    const char * repo_root = (const char *)get_configuration(configurations, "repository.root");
    
    debug("repo_root : %s\n", repo_root);
    
    REPOSITORY * repo = repository_init(repo_root);
    
    REPO_ITEM * item = repository_new_item(repo);
    
    debug("item : %s\n", item->uuid);
    
    FILE * fp_lidar_2368 = repository_open_write_file(item, "lidar.raw");
    FILE * fp_lidar_8308 = repository_open_write_file(item, "sync.raw");
    FILE * fp_ins = repository_open_write_file(item, "ins.bin");
    
    debug("fp_lidar_2368 : %p\n", fp_lidar_2368);
    debug("fp_lidar_8308 : %p\n", fp_lidar_8308);
    debug("fp_ins : %p\n", fp_ins);
    
    //Start processing threads.
    
//  pthread_t main_thread_handler = pthread_self();
	pthread_t xsens_thread_handler;
	
	pthread_t lidar_10110_thread_handler;
    pthread_t lidar_2368_thread_handler;
    pthread_t lidar_8308_thread_handler;
	
	XsensThreadData xsensThreadData;
	xsensThreadData.configurations = configurations;
	xsensThreadData.fp = fp_ins;
	
	LiDAR10110ThreadData lidar10110ThreadData;
	lidar10110ThreadData.configurations = configurations;
	
	pthread_create(&xsens_thread_handler, NULL, xsenThread, (void *)(&xsensThreadData));
	bind_thread_cpu(xsens_thread_handler, cpu_core_count - 4);
	
	pthread_create(&lidar_10110_thread_handler, NULL, lidar10110Thread, (void *)&lidar10110ThreadData);
	bind_thread_cpu(lidar_10110_thread_handler, cpu_core_count - 1);
	
	pthread_create(&lidar_2368_thread_handler, NULL, lidar2368Thread, (void *)fp_lidar_2368);
	bind_thread_cpu(lidar_2368_thread_handler, cpu_core_count - 2);
	
	pthread_create(&lidar_8308_thread_handler, NULL, lidar8308Thread, (void *)fp_lidar_8308);
	bind_thread_cpu(lidar_8308_thread_handler, cpu_core_count - 3);
	
	debug("Waiting for any key...\n");
	
	if(!is_daemon) {
    	fputs("Press any key...", stdout);
    	fgetc(stdin);
    	
    	done = 1;
    	
    	debug("Key has been pressed.\n");
    	debug("Start to dispose...\n");
	} else {
	    signal(SIGQUIT, daemon_exit_handler);
	}
	
	pthread_join(lidar_2368_thread_handler, (void *)NULL);
	pthread_join(lidar_8308_thread_handler, (void *)NULL);
	pthread_join(xsens_thread_handler, (void *)NULL);
 	pthread_join(lidar_10110_thread_handler, (void *)NULL);
	
	//Dispose...
    
    fclose(fp_lidar_8308);
    fclose(fp_lidar_2368);
    fclose(fp_ins);
    
    repository_close_item(item);
    
    repository_dispose(repo);
    
    dispose_configuration(configurations);
    
    pthread_exit(NULL);
    
    debug("[START]<<\n");
    
    return 0;
}

int main(int argc, char * argv[]) {
    return start(1); /*1 -- deamon, 0 -- console application.*/
}
