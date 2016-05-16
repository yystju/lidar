#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int fd,readnum;
    char *dev="/dev/ttyAMA0",recv[1024*4];
    struct termios opt,oldopt;

    fd = open( dev, O_RDWR ); 

    tcgetattr( fd,&oldopt);

    tcgetattr( fd,&opt);
    cfsetispeed(&opt, B4800);//9600
    cfsetospeed(&opt, B4800);

    opt.c_cflag &= ~PARENB; //N
    opt.c_cflag &= ~INPCK;
    opt.c_cflag &= ~CSTOPB;//1
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8; //8

    opt.c_iflag &= ~(IXON | IXOFF | IXANY);

    opt.c_cc[VTIME] = 0; 
    opt.c_cc[VMIN] = 0; 

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
    opt.c_oflag &= ~OPOST; /*Output*/

    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &opt);

    while(1)
    {
        bzero(recv,sizeof(recv));
        while((readnum = read(fd,recv,sizeof(recv)))>0)
        {
            printf("%s",recv);
        }
    }

    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &oldopt); 
    close(fd);
}
