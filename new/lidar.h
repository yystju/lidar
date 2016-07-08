#ifndef _LIDAR_H_
#define _LIDAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LIDAR_DATA_PORT 2368
#define LIDAR_SYNC_PORT 8308
#define LIDAR_TIME_PORT 10110

typedef void (* LidarDataProcessor)(char *p, int start, int len);

typedef void * LIDAR;

LIDAR lidar_send_init(int port);
LIDAR lidar_receive_init(int port);
void lidar_dispose(LIDAR p);

void lidar_read_data(LIDAR p, LidarDataProcessor processor);
void lidar_write_data(LIDAR p, char * data, int start, int len);

#ifdef __cplusplus
}
#endif

#endif