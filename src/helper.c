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
#include <memory.h>

#include <uuid/uuid.h>

#include "aes.h"

#ifdef __cplusplus
extern "C" {
#endif

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
    
    fflush(debug_fp);
}

void info(const char *p, ...) {
    if(info_fp == NULL) {
        info_fp = fopen("./info.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(info_fp, p, ap);
    va_end(ap);
    
    fflush(info_fp);
}

void error(const char *p, ...) {
    if(error_fp == NULL) {
        error_fp = fopen("./error.log", "a");
    }
    
    va_list ap;
    
    va_start(ap, p);
    dump(error_fp, p, ap);
    va_end(ap);
    
    fflush(error_fp);
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
  int i;
  char checksum = 0x00;
  
  char date_buff[20];
  char time_buff[20];
  
  memset(date_buff, '\0', sizeof(date_buff));
  memset(time_buff, '\0', sizeof(time_buff));
  memset(buff, '\0', buff_len);
 
  if(year > 1900) year -= 1900;

  debug("## year : %d\n", year);
  
  snprintf(date_buff, 20, "%02d%02d%02d", day, month, (year > 100 ? year - 100 : year));
  snprintf(time_buff, 20, "%02d%02d%02d", hour, minute, second);
  cx = snprintf(buff, buff_len, "$GPRMC,%s,A,%.2f,%s,%.2f,%s,%05.1f,%05.1f,%s,%05.1f,%s",time_buff,0.0f,"N",0.0f,"W",0.0f,0.0f,date_buff,0.0f, "E");
  
  debug("## time_buff : %s\n", time_buff);
  debug("## date_buff : %s\n", date_buff);
  
  for(i = 1; i < cx; ++i) {
      checksum = checksum ^ buff[i];
  }
  
  //if(buff_len - cx > 3) {
      cx += snprintf(buff + cx, buff_len - cx, "*%X", checksum);
  //}
  //if(buff_len - cx > 1) {
  //    buff[++cx] = '\n';
  //}

  //strncat(buff, "\n", buff_len - cx);
  
  debug("## buff : %s\n", buff);
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

Configuration * read_configuration_file(const char * file_name, int capacity) {
    int len = 0;
    
    char buff[1024];
    
    FILE * file = fopen(file_name, "r");
    
    Configuration * configurations = (Configuration *) malloc(sizeof(Configuration));
    
    configurations->pConfig = (ConfigurePair *) malloc(sizeof(ConfigurePair) * capacity);
    
    while(!feof(file)) {
        //debug("len = %d, max = %d\n", len, max);
        if(len >= capacity) break;
        
        memset(buff, '\0', 1024);
        fgets(buff, 1024, file);
        
        if(strlen(buff) > 0 && buff[0] != '#') {
            sscanf(buff, "%[^=]=%s", (((configurations->pConfig)[len]).key), (((configurations->pConfig)[len]).value));
            
            ++len;
        }
    }
    
    configurations->length = len;
    
    fclose(file);
    
    return configurations;
}

void dispose_configuration(Configuration * configurations) {
    if(configurations) {
        free(configurations->pConfig);
        free(configurations);
    }
}

const char * get_configuration(Configuration * configurations, const char * key) {
	int i;
    if(configurations) {
        for(i = 0; i < configurations->length; ++i) {
            if(strcmp((configurations->pConfig)[i].key, key) == 0) {
                return (configurations->pConfig)[i].value;
            }
        }
    }
    return NULL;
}

/**
 * dest -- size must be larger than src size and should equal to (N * 16), N = 1, 2, 3, ...
 */
int encode_data(char * dest, char * src, int n) {
  int len = 0;
  int i;
    
  char * pkey = "1qazxsw23edcvfr4";
  
  struct aes_ctx aes;
  
  aes_set_key(&aes, pkey, 16);
  
  memset(dest, '\0', n);
  
  for(i  = 0; i < n; i = i + 16) {
      aes_encrypt(&aes, dest + i, src + i);
      len += 16;
  }
  
  if(n % 16 > 0) {//last packet.
    char buff[16];
    
    memset(buff, '\0', 16);
    
    memcpy(buff, src + (n / 16) * 16, n % 16);
    
    aes_encrypt(&aes, dest + (n / 16) * 16, buff);
    len += 16;
  }
  
  return len;
}

int decode_data(char * dest, char * src, int n) {
  int len = 0;
  
  int i;

  char * pkey = "1qazxsw23edcvfr4";
  
  struct aes_ctx aes;

  gen_tabs();
    
  aes_set_key(&aes, pkey, 16);
  
  memset(dest, '\0', n);
  
  for(i  = 0; i < n; i = i + 16) {
      aes_decrypt(&aes, dest + i, src + i);
      len += 16;
  }
  
  if(n % 16 > 0) {//last packet.
    char buff[16];
    
    memset(buff, '\0', 16);
    
    memcpy(buff, src + (n / 16) * 16, n % 16);
    
    aes_decrypt(&aes, dest + (n / 16) * 16, buff);
    
    len += 16;
  }
  
  return len;
}


int near_zero(double d) {
    return (d < 1e-2);
}

#ifdef __cplusplus
}
#endif
