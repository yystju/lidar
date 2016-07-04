#ifndef _HELPER_H_
#define _HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

void debug(const char *p, ...);
void info(const char *p, ...);
void error(const char *p, ...);

int file_exits(const char * file_name);

#ifdef __cplusplus
}
#endif

#endif