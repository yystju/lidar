/**
 * test.c
 */
 
#include     <stddef.h>

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/

#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/

#include     <sys/socket.h>
#include     <netinet/in.h>
#include     <arpa/inet.h>

#include     <string.h>

#include     "send_gprmc.h"

int enableBroadCast(int fd) {
	int broadcastEnable=1;
	return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
}

int send_gprmc (char * buff, size_t buff_len) {
    int fd;
    struct termios Opt;
    char recv[8*1024];
    int readnum;
    
    struct sockaddr_in rmc_addr;
    int rmc_fd;
    
    
    
    /*
     * ==== Init Serial port ====
     */
    fd = open( "/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == fd){
        perror("Failed to open serial port");
        exit(-1);
    }
    
    tcgetattr(fd, &Opt);
    
    cfsetispeed(&Opt,B115200);
    cfsetospeed(&Opt,B115200);
    
    Opt.c_cflag &= ~PARENB; 
    Opt.c_cflag &= ~CSTOPB; 
    Opt.c_cflag &= ~CSIZE; 
    Opt.c_cflag |= ~CS8;
    
    Opt.c_iflag &= ~INPCK;
    
    tcflush(fd,TCIFLUSH);
    
    Opt.c_cc[VTIME] = 150; /* Timeout 15 seconds*/   
    Opt.c_cc[VMIN] = 0;
    
    if (tcsetattr(fd,TCSANOW, &Opt) != 0) { 
    	perror("Faile to setup serial port");
    	exit(-1);  
    }
    /*
     * ---- Init Serial port ----
     */
    
    /*
     * ==== Init UDP port ====
     */
    memset(&rmc_addr,0,sizeof(rmc_addr));
    rmc_addr.sin_family = AF_INET;
    rmc_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    rmc_addr.sin_port=htons(RMC_PORT);
    
    if ((rmc_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to open UDP socket.");
        exit(-2);
    }
    
    
    if(enableBroadCast(rmc_fd) < 0) {
    	perror("Failed to enable broadcast");
        exit(1);
    }
    /*
     * ---- Init UDP port ----
     */
    
    /*
     * ==== Main loop ====
     */
    while(1) {
        memset(recv, '\0', sizeof(recv));
        while((readnum = read(fd,recv,sizeof(recv)))>0) {
            printf("%s",recv);
            
            if (sendto(rmc_fd, recv, strlen(recv), 0,(struct sockaddr *) &rmc_addr, sizeof(rmc_addr)) < 0) {
                perror("Failed to send udp broadcast data");
                exit(-3);
            }
        }
    }
    /*
     * ---- Main loop ----
     */
    
    close(rmc_fd);
    
    close(fd);
    
    return 0;
}


int format(char * buff, size_t buff_len, int year, int month, int day, int hour, int minute, int second)
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