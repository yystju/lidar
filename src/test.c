#include <stdio.h>
#include <time.h>

#include <sys/time.h>

int main(int argc, char *argv[]) {
	time_t t;
	time(&t);

	printf("%s (%d)\n", ctime(&t), t);
	
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	
	printf("Time: %ld.%06ld\n", (long int)tv.tv_sec, (long int)tv.tv_usec);

	return 0;
}

