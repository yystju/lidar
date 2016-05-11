#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>

int main(int argc, char * argv[]) {
	struct timeval tv;

	if(!gettimeofday(&tv, NULL)) {
		perror("gettimeofday");
	}

  unsigned long long m = (unsigned long long)(tv.tv_sec) * 1000 
                       + (unsigned long long)(tv.tv_usec) / 1000;

	printf("%s %llu\n", "HELLO", m);
	return 0;
}
