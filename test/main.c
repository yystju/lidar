#define _GNU_SOURCE

#include<stdlib.h>
#include<stdio.h>

#include <sched.h>

#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>

#include<ctype.h>
#include<string.h>

int main(int argc, char* argv[])
{
        int num = sysconf(_SC_NPROCESSORS_CONF);
        int created_thread = 0;
        int myid;
        int i;
        int j = 0;

        cpu_set_t mask;
        cpu_set_t get;

        if (argc != 2)
        {
                printf("usage : ./cpu num\n");
                exit(1);
        }

        myid = atoi(argv[1]);

        printf("system has %i processor(s). \n", num);

        CPU_ZERO(&mask);
        CPU_SET(myid, &mask);

        if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
        {
                printf("warning: could not set CPU affinity, continuing...\n");
        }
        while (1)
        {

                CPU_ZERO(&get);
                if (sched_getaffinity(0, sizeof(get), &get) == -1)
                {
                        printf("warning: cound not get cpu affinity, continuing...\n");
                }
                for (i = 0; i < num; i++)
                {
                        if (CPU_ISSET(i, &get))
                        {
                                printf("this process %d is running processor : %d\n",getpid(), i);
                        }
                }
        }
        return 0;
}

// #include "send_gprmc.h"

// int main ( int argc, char * argv[] ) {
//     char buff[1024];
//     format(buff, sizeof(buff), 2016, 6, 5, 10, 59, 00);
    
//     puts(buff);
    
//     auto a = 1.24f;
    
//     printf("%d\n", a);
    
//     return 0;
// }