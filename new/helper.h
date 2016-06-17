#ifndef _HELPER_H_
#define _HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

void debug(char *p, ...);
void info(char *p, ...);
void error(char *p, ...);

int file_exits(const char * file_name);

#ifdef __cplusplus
}
#endif

#endif