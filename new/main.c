#include "lidar.h"
#include "xsens.h"

#include "raspi.h"
#include "helper.h"

#include "repository.h"

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>

static int done = 0;

int xsensDataProcessor(XsensData * pData, void * param) {
    if(near_zero(pData->euler_roll) && near_zero(pData->euler_pitch) && near_zero(pData->euler_yaw)) return !done;
    
    debug("[xsensDataProcessor]>> param : %p\n", param);
    
    LIDAR lidar = (LIDAR)param;
    
    debug("quaternion : [%f %f %f %f]\n", pData->quaternion_w, pData->quaternion_x, pData->quaternion_y, pData->quaternion_z);
    
    debug("euler : [%f %f %f]\n", pData->euler_roll, pData->euler_pitch, pData->euler_yaw);
    
    debug("date : %04d-%02d-%02d %02d:%02d:%02d\n", pData->year, pData->month, pData->day, pData->hour, pData->minute, pData->second);
    
    if(pData->year > 0) {
        char * buff = (char *)malloc(sizeof(char) * 1024);
        
        memset(buff, '\0', sizeof(char) * 1024);
        
        format_gprmc(buff, 1024, pData->year, pData->month, pData->day, pData->hour, pData->minute, pData->second);
        
        lidar_write_data(lidar, buff, 0, strlen(buff));
        
        free(buff);
    }
    
    debug("[xsensDataProcessor]<<\n");
    
    return !done;
}

int lidar2368DataProcessor(char *p, int start, int len) {
    debug("[lidar2368DataProcessor]>>\n");
    
    debug("p : %p, start : %d, len %d\n", p, start, len);
    
    debug("[lidar2368DataProcessor]<<\n");
    
    return !done;
}

int lidar8308DataProcessor(char *p, int start, int len) {
    debug("[lidar8308DataProcessor]>>\n");
    
    debug("p : %p, start : %d, len %d\n", p, start, len);
    
    debug("[lidar8308DataProcessor]<<\n");
    
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
    
    LIDAR lidar = lidar_send_init(LIDAR_TIME_PORT);
    
    readXsensData(ins_device, xsensDataProcessor, lidar);
    
    lidar_dispose(lidar);
    
    pthread_exit(NULL);
}

// void * lidar10110Thread (void * p) {
//     debug("[lidar10110Thread] p : %p\n", p);
    
//     Configuration * configurations = (Configuration *)p;
    
// 	LIDAR lidar = lidar_send_init(LIDAR_TIME_PORT);
	
// 	char * str = "HELLO";
	
//     while(1) {
// 		lidar_write_data(lidar, str, 0, strlen(str));
// 		sleep(1);
// 	}
	
// 	lidar_dispose(lidar);
    
//     pthread_exit(NULL);
// }

void * lidar2368Thread (void * p) {
    debug("[lidar2368Thread] p : %p\n", p);
    
    Configuration * configurations = (Configuration *)p;
    
	LIDAR lidar = lidar_receive_init(LIDAR_DATA_PORT);
	
    while(1) {
		lidar_read_data(lidar, lidar2368DataProcessor);
	}
	
	lidar_dispose(lidar);
    
    pthread_exit(NULL);
}

void * lidar8308Thread (void * p) {
    debug("[lidar8308Thread] p : %p\n", p);
    
    Configuration * configurations = (Configuration *)p;
    
	LIDAR lidar = lidar_receive_init(LIDAR_SYNC_PORT);
	
    while(1) {
		lidar_read_data(lidar, lidar8308DataProcessor);
	}
	
	lidar_dispose(lidar);
    
    pthread_exit(NULL);
}

int start(int argc, char * argv[]) {
    debug("[START]>>\n");
    
    //Init...
    
    int cpu_core_count = (int)sysconf(_SC_NPROCESSORS_CONF);
    
    debug("cpu_core_count = %d\n", cpu_core_count);
    
    Configuration * configurations = read_configuration_file("./test.ini", 5);
    
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
    pthread_t lidar_2368_thread_handler;
    pthread_t lidar_8308_thread_handler;
	pthread_t xsens_thread_handler;
//    pthread_t lidar_10110_thread_handler;
	
	pthread_create(&lidar_2368_thread_handler, NULL, lidar2368Thread, (void *)fp_lidar_2368);
	bind_thread_cpu(lidar_2368_thread_handler, cpu_core_count - 0);
	
	pthread_create(&lidar_8308_thread_handler, NULL, lidar8308Thread, (void *)fp_lidar_8308);
	bind_thread_cpu(lidar_8308_thread_handler, cpu_core_count - 1);
	
	XsensThreadData xsensThreadData;
	
	xsensThreadData.configurations = configurations;
	xsensThreadData.fp = fp_ins;
	
	pthread_create(&xsens_thread_handler, NULL, xsenThread, (void *)(&xsensThreadData));
	bind_thread_cpu(xsens_thread_handler, cpu_core_count - 2);
	
// 	pthread_create(&lidar_10110_thread_handler, NULL, lidar10110Thread, (void *)configurations);
// 	bind_thread_cpu(lidar_10110_thread_handler, cpu_core_count - 3);
    
	debug("Waiting for any key...\n");
	
	fputs("Press any key...", stdout);
	fgetc(stdout);
	
	done = 1;
	
	debug("Start to dispose.\n");
	
	pthread_join(lidar_2368_thread_handler, (void *)NULL);
	pthread_join(lidar_8308_thread_handler, (void *)NULL);
	pthread_join(xsens_thread_handler, (void *)NULL);
// 	pthread_join(lidar_10110_thread_handler, (void *)NULL);
	
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

int test_repository (int argc, char * argv[]) {
     Configuration * configurations = read_configuration_file("./test.ini", 5);
    
    const char * ins_device = (const char *)get_configuration(configurations, "repository.root");
    
    debug("ins_device : %s\n", ins_device);
    
    REPOSITORY * repo = repository_init(ins_device);
    
    REPO_ITEM * item = repository_new_item(repo);
    
    repository_close_item(item);
    
    FILE * fp = repository_open_write_file(item, "lidar");
    
    debug("fp : %p\n", fp);
    
    fprintf(fp, "%s\n", repo->root_pathname);
    
    fclose(fp);
    
    repository_dispose(repo);
    
    dispose_configuration(configurations);
    
    return 0;
}

int test_encoder_decoder(int argc, char * argv[]) {
    //char buff[1024];
    
    //format_gprmc(buff, sizeof(buff), 2016, 6, 5, 10, 59, 00);
    
    //puts(buff);
    
    //blink(10);
    
    gen_tabs();
    
    char * src = "Hello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\n";
    
    debug("strlen(src) : %d\n", strlen(src));
    
    char encoded[1024];
    
    char decoded[1024];
    
    int len = encode_data(encoded, src, strlen(src));
    
    debug("len : %d\n", len);
    
    int len2 = decode_data(decoded, encoded, len);
    
    debug("len2 : %d\n", len2);
    
    decoded[len2] = '\0';
    
    debug("--1--\n");
    
    debug("decoded : %s\n", decoded);
    
    debug("strlen(decoded) : %d\n", strlen(decoded));
    
    debug("src and decoded are %s.\n", strcmp(src, decoded) == 0 ? "equal" : "not equal");
    
    return 0;
}

int test_xsens(int argc, char * argv[]) {
    Configuration * configurations = read_configuration_file("./test.ini", 5);
    
    // for(int i = 0; i < configurations->length; ++i) {
    //     debug("[%s=%s]\n", (configurations->pConfig)[i].key, (configurations->pConfig)[i].value);
    // }
    
    const char * ins_device = (const char *)get_configuration(configurations, "ins.device");
    
    debug("ins_device : %s\n", ins_device);
    
    readXsensData(ins_device, xsensDataProcessor, (void *) NULL);
    
    dispose_configuration(configurations);
}

int main(int argc, char * argv[]) {    
    //return test_encoder_decoder(argc, argv);
    //return test_xsens(argc, argv);
    //return test_repository(argc, argv);
    return start(argc, argv);
}
