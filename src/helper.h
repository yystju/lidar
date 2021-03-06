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

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void debug(const char *p, ...);
void info(const char *p, ...);
void error(const char *p, ...);

int file_exits(const char * file_name);

void bind_thread_cpu(pthread_t threadId, int cpu_number);

int format_gprmc(char * buff, size_t buff_len, int year, int month, int day, int hour, int minute, int second);

char * get_uuid(void);

// char * get_file_name(const char * prefix, const char * surfix);

typedef struct {
    char key[255];
    char value[255];
} ConfigurePair;

typedef struct {
    ConfigurePair * pConfig;
    int length;
} Configuration;

Configuration * read_configuration_file(const char * file_name, int capacity);
void dispose_configuration(Configuration * configuration);
const char * get_configuration(Configuration * configuration, const char * key);

int encode_data(char * dest, char * src, int n);
int decode_data(char * dest, char * src, int n);

int near_zero(double d);

void init_deamon_process(void);

#ifdef __cplusplus
}
#endif

#endif