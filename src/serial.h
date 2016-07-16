#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void * SERIAL;

SERIAL serial_open(const char * file_pathname);

void serial_close(SERIAL serial);

void serial_write(SERIAL serial, const char * buff, int len);
    
#ifdef __cplusplus
}
#endif

#endif