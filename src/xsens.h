#ifndef _XSENS_H_
#define _XSENS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    
    double quaternion_w;
    double quaternion_x;
    double quaternion_y;
    double quaternion_z;
    
    double euler_roll;
    double euler_pitch;
    double euler_yaw;
} XsensData;

typedef int (* XsensDataProcessor)(XsensData * pData, void * param);

void readXsensData(const char * device_name, XsensDataProcessor processor, void * param);

#ifdef __cplusplus
}
#endif

#endif