#include "serial.h"
#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char file_name[255];
  int fd;
} SERIAL_IMPL;

int set_interface_attribs(int fd, speed_t speed, int parity) {
  struct termios tty;
  
  memset (&tty, 0, sizeof tty);
  
  if (tcgetattr (fd, &tty) != 0) {
    error("error %d from tcgetattr\n", errno);
    return -1;
  }
  
  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);
  
  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // disable break processing
  tty.c_lflag = 0;                // no signaling chars, no echo,
                                  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
  
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
  
  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;
  
  tcflush(fd, TCIOFLUSH);
  
  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    error("error %d from tcsetattr\n", errno);
    return -1;
  }
  
  return 0;
}

void set_blocking (int fd, int should_block) {
  struct termios tty;
  
  memset (&tty, 0, sizeof tty);
  
  if (tcgetattr(fd, &tty) != 0) {
    error("error %d from tggetattr\n", errno);
    return;
  }
  
  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
  
  if (tcsetattr(fd, TCSANOW, &tty) != 0)
    error("error %d setting term attributes\n", errno);
}

SERIAL serial_open(const char * file_pathname) {
  SERIAL_IMPL * serial = NULL;
  
  if(file_exits(file_pathname)) {
    serial = (SERIAL_IMPL *)malloc(sizeof(SERIAL_IMPL));
    memset(serial, 0, sizeof(SERIAL_IMPL));
    
    serial->fd = open(file_pathname, O_RDWR | O_NOCTTY | O_SYNC);
    
    set_interface_attribs (serial->fd, B115200, 0);
    
    set_blocking (serial->fd, 0);
  }
  
  return serial;
}

void serial_close(SERIAL serial) {
  if(serial) {
    SERIAL_IMPL * p = (SERIAL_IMPL *)serial;
    
    if(p->fd) {
      close(p->fd);
    }
    
    free(p);
  }
}

void serial_write(SERIAL serial, const char * buff, int len) {
  if(serial && buff && len > 0) {
    SERIAL_IMPL * p = (SERIAL_IMPL *)serial;
    
    if(p->fd) {
      write (p->fd, buff, len);
      tcflush(p->fd, TCIOFLUSH);
    }
  }
}

#ifdef __cplusplus
}
#endif