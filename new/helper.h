#ifndef _HELPER_H_
#define _HELPER_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void debug(const char *p, ...);
void info(const char *p, ...);
void error(const char *p, ...);

int file_exits(const char * file_name);

int format_gprmc(char * buff, size_t buff_len, int year, int month, int day, int hour, int minute, int second);

#ifdef __cplusplus
}
#endif

#endif