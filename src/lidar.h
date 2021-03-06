#ifndef _LIDAR_H_
#define _LIDAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LIDAR_DATA_PORT 2368
#define LIDAR_SYNC_PORT 8308
#define LIDAR_TIME_PORT 10110

#define LIDAR_DATA_PACKET_SIZE 1206
#define LIDAR_SYNC_PACKET_SIZE 512

typedef int (* LidarDataProcessor)(char *p, int start, int len, void * param);

typedef void * LIDAR;

typedef struct {
  char header[42];
  char payload[1206];
} LIDAR_DATA_PACKET;

LIDAR lidar_send_init(const char * adapter, int port);
LIDAR lidar_receive_init(int port);
void lidar_dispose(LIDAR p);

void lidar_read_data(LIDAR p, LidarDataProcessor processor, void * param);
void lidar_write_data(LIDAR p, char * data, int start, int len);

#ifdef __cplusplus
}
#endif

#endif
