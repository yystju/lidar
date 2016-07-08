#include "lidar.h"
#include "xsens.h"

#include "raspi.h"
#include "helper.h"

#include <pthread.h>

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
    
    readXsensData(xsensDataProcessor);
    
    pthread_exit(NULL);
}

void * lidarThread (void * p) {
    debug("[lidarThread] p : %p\n", p);
    
    //lidar_read_data(lidarDataProcessor);
    
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    debug("[START]>>\n");
    /*
    pthread_t thread[255];
    
    for(int i = 0; i < 255; ++i) {
        if(i % 2 == 0) {
            pthread_create(&thread[i], NULL, lidarThread, (void *)NULL);
        } else {
            pthread_create(&thread[i], NULL, xsenThread, (void *)NULL);
        }
    }
    
    readXsensData(xsensDataProcessor);
    
    lidar_read_2368_data(lidarDataProcessor);
    
    for(int i = 0; i < 255; ++i) {
        pthread_join(thread[i], (void *)NULL);
    }
    */

    //for(int i = 0; i < 10; ++i) xsens_wrapper_test();   


    readXsensData(xsensDataProcessor);

    debug("[START]<<\n");
    
    //pthread_exit(NULL);
    return 0;
}
