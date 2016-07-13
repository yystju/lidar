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

#include <uuid/uuid.h>

FILE * debug_fp = NULL;
FILE * info_fp = NULL;
FILE * error_fp = NULL;

const int LOG_BUFF_SZ = 1024 * 8 * sizeof(char);

const char * dump_time(const char * p) {
    if(!p) {
        return NULL;
    }
    
    time_t t;
    struct tm m;
    
    time(&t);
    
    gmtime_r(&t, &m);
    
    asctime_r(&m, (char *)p);
    
    return p;
}

void dump(FILE * fp, const char *p, va_list ap) {
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

void debug(const char *p, ...) {
    if(debug_fp == NULL) {
        debug_fp = fopen("./debug.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(debug_fp, p, ap);
    va_end(ap);
}

void info(const char *p, ...) {
    if(info_fp == NULL) {
        info_fp = fopen("./info.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(info_fp, p, ap);
    va_end(ap);
}

void error(const char *p, ...) {
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

void bind_thread_cpu(pthread_t threadId, int cpu_number) {
    //http://linux.die.net/man/3/pthread_setaffinity_np
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_number, &cpuset);
    
    pthread_getaffinity_np(threadId, sizeof(cpu_set_t), &cpuset);
}

int format_gprmc(char * buff, size_t buff_len, int year, int month, int day, int hour, int minute, int second)
{
  int cx;
  
  char date_buff[20];
  char time_buff[20];
  
  memset(date_buff, '\0', sizeof(date_buff));
  memset(time_buff, '\0', sizeof(time_buff));
  memset(buff, '\0', buff_len);
  
  year -= 1900;
  
  snprintf(date_buff, 20, "%02d%02d%02d", day, month, (year > 100 ? year - 100 : year));
  snprintf(time_buff, 20, "%02d%02d%02d", hour, minute, second);
  cx = snprintf(buff, buff_len, "$GPRMC,%s,A,%.2f,%s,%.2f,%s,%05.1f,%05.1f,%s,%05.1f,%s", time_buff, 0.0f, "N", 0.0f,"W", 0.0f, 0.0f, date_buff, 20.312, "E");
  
  puts(time_buff);
  puts(date_buff);
  puts (buff);
  
  char checksum = 0x00;
  
  for(int i = 1; i < cx; ++i) {
      checksum = checksum ^ buff[i];
  }
  
  if(buff_len - cx > 2) {
      cx += snprintf(buff + cx, buff_len - cx, "*%d", checksum);
  }
  
  return cx;
}

char * get_uuid(void) {
    char * p = (char *) malloc(sizeof(char) * 255);
    
    uuid_t uu;
    uuid_generate(uu);
    
    uuid_unparse_upper(uu, p);
    
    return p;
}

// char * get_file_name(const char * prefix, const char * surfix) {
//     char * p = (char *)malloc(sizeof(char) * 1024);
    
//     int offset = 0;
    
//     if(prefix && strlen(prefix) > 0) {
//         offset += snprintf(p + offset, 1024  - offset, "%s", prefix);
//     }
    
//     uuid_t uu;
//     uuid_generate(uu);
    
//     uuid_unparse_upper(uu, p + offset);
    
//     offset = strlen(p);
    
//     //debug("UUID : %s\n", p);
    
//     if(surfix && strlen(surfix) > 0) {
//         offset += snprintf(p + offset, 1024  - offset, "%s", surfix);
//     }
    
//     return p;
// }

int read_configuration_file(const char * file_name, int max, ConfigurePair ** pConfig) {
    int len = 0;
    
    char buff[1024];
    
    FILE * file = fopen(file_name, "r");
    
    if(*pConfig != NULL) {
        free(*pConfig);
        (*pConfig) = NULL;
    }
    
    (*pConfig) = (ConfigurePair *) malloc(sizeof(ConfigurePair) * max);
    
    while(!feof(file)) {
        //debug("len = %d, max = %d\n", len, max);
        if(len >= max) break;
        
        memset(buff, '\0', 1024);
        fgets(buff, 1024, file);
        
        if(strlen(buff) > 0 && buff[0] != '#') {
            sscanf(buff, "%[^=]=%s", (((*pConfig)[len]).key), (((*pConfig)[len]).value));
            
            ++len;
        }
    }
    
    fclose(file);
    
    return len;
}

const char * get_configuration(ConfigurePair * pConfig, int size, const char * key) {
    if(pConfig) {
        for(int i = 0; i < size; ++i) {
            if(strcmp(pConfig[i].key, key) == 0) {
                return pConfig[i].value;
            }
        }
    }
    return NULL;
}
