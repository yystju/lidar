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
    int nano;
    
    double euler_roll;
    double euler_pitch;
    double euler_yaw;
    
    double latitude;
    double longitude;
    double altitude;
    
    double verlocity_x;
    double verlocity_y;
    double verlocity_z;
    
    double quaternion_w;
    double quaternion_x;
    double quaternion_y;
    double quaternion_z;
} XsensData;

typedef int (* XsensDataProcessor)(XsensData * pData, void * param);

void readXsensData(const char * device_name, XsensDataProcessor processor, void * param);

#ifdef __cplusplus
}
#endif

#endif