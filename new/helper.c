#include "helper.h"

#ifdef WIN32
#   include <io.h>
#   define access _access
#   define F_OK 0
#else
#   include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

FILE * debug_fp = NULL;
FILE * info_fp = NULL;
FILE * error_fp = NULL;

const int LOG_BUFF_SZ = 1024 * 8 * sizeof(char);

char * dump_time(char * p) {
    if(!p) {
        return NULL;
    }
    
    time_t t;
    struct tm m;
    
    time(&t);
    
    gmtime_r(&t, &m);
    
    asctime_r(&m, p);
    
    return p;
}

void dump(FILE * fp, char *p, va_list ap) {
    char * buff = (char *) malloc(LOG_BUFF_SZ);
    
    memset(buff, '\0', LOG_BUFF_SZ);
    
    buff[0] = '[';
    
    dump_time(buff + 1);
    
    buff[strlen(buff) - 1] = ']';
    buff[strlen(buff)] = ' ';
    
    int offset = strlen(buff);
    
    vsnprintf(buff + offset, LOG_BUFF_SZ - offset, p, ap);
    
    fputs(buff, fp);
    fflush(fp);
    
    free(buff);
}

void debug(char *p, ...) {
    if(debug_fp == NULL) {
        debug_fp = fopen("./debug.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(debug_fp, p, ap);
    va_end(ap);
}

void info(char *p, ...) {
    if(info_fp == NULL) {
        info_fp = fopen("./info.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(info_fp, p, ap);
    va_end(ap);
}

void error(char *p, ...) {
    if(error_fp == NULL) {
        error_fp = fopen("./error.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(error_fp, p, ap);
    va_end(ap);
}

int file_exits(const char * file_name) {
    return access(file_name, F_OK ) != -1;
}
