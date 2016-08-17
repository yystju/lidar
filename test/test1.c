#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int);

int main() {
    signal(SIGQUIT, sig_handler);
    while(1) {
        printf(".");
        usleep(2000);
    }
    return 0;
}

void sig_handler(int sig) {
    switch (sig) {
    case SIGQUIT:
        fprintf(stderr, "give out a backtrace or something...\n");
        exit(0);
    case SIGINT:
        fprintf(stderr, "give out a backtrace or something...\n");
        exit(0);
    default:
        fprintf(stderr, "wasn't expecting that!\n");
        exit(1);
    }
}
