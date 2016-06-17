#include "lidar.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "helper.h"

const int BUFF_SIZE = 1024 * 1024 * 8;

void lidar_read_2368_data(LidarDataProcessor processor) {
    debug("[lidar_read_2368_data]>>\n");
    
    char *p = malloc(BUFF_SIZE);
    int offset = 0;
    
    if(processor) {
        processor(p, offset, BUFF_SIZE);
    }
    
    free(p);
    debug("[lidar_read_2368_data]<<\n");
}
void lidar_read_8038_data(LidarDataProcessor processor) {
    debug("[lidar_read_8038_data]>>\n");
    char *p = malloc(BUFF_SIZE);
    int offset = 0;
    
    if(processor) {
        processor(p, offset, BUFF_SIZE);
    }
    
    free(p);
    debug("[lidar_read_8038_data]<<\n");
}

void lidar_write_10110_data(char *p, int start, int len) {
    debug("[lidar_write_10110_data]>>\n");
    debug("[lidar_write_10110_data]<<\n");
}

#ifdef __cplusplus
}
#endif