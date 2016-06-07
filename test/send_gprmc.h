#ifndef _SEND_GPRMC_H_
#   define _SEND_GPRMC_H_

#include <stddef.h>

#define RMC_PORT 10110

#ifdef __cplusplus
extern "C" {
#endif
    int format(char * buff, size_t buff_len, int year, int month, int day, int hour, int minute, int second);
#ifdef __cplusplus
}
#endif


#endif
