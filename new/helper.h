#ifndef _HELPER_H_
#define _HELPER_H_

#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/sysinfo.h>

#ifdef __cplusplus
extern "C" {
#endif

void debug(const char *p, ...);
void info(const char *p, ...);
void error(const char *p, ...);

int file_exits(const char * file_name);

void bind_thread_cpu(pthread_t threadId, int cpu_number);

#ifdef __cplusplus
}
#endif

#endif