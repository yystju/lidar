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
} XsensData;

typedef void (* XsensDataProcessor)(XsensData * pData);

void readXsensData(XsensDataProcessor processor);

#ifdef __cplusplus
}
#endif

#endif