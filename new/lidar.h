#ifndef _LIDAR_H_
#define _LIDAR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (* LidarDataProcessor)(char *p, int start, int len);

void lidar_read_2368_data(LidarDataProcessor processor);
void lidar_read_8038_data(LidarDataProcessor processor);

void lidar_write_10110_data(char *p, int start, int len);

#ifdef __cplusplus
}
#endif

#endif