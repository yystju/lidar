#include <stdio.h>
#include <time.h>

#include <sys/time.h>

// int main(int argc, char *argv[]) {
// 	time_t t;
// 	time(&t);

// 	printf("%s (%d)\n", ctime(&t), t);
	
// 	struct timeval tv;
	
// 	gettimeofday(&tv, NULL);
	
// 	printf("Time: %ld.%06ld\n", (long int)tv.tv_sec, (long int)tv.tv_usec);

// 	return 0;
// }

int main ()
{
	time_t t;
	struct tm tm;
	struct tm * tmp;

	//t = timegm(&tm);
	
	t = time(NULL);

	tmp = gmtime(&t);
	
	printf("%s\n", asctime(tmp));
	
	//t = mktime(&tm);

	tmp = localtime(&t);
	
	printf("%s\n", asctime(tmp));
	
  char time_buff[20];
  
  snprintf(time_buff, 20, "%02d%02d%02d", tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
  
  puts(time_buff);
  
  char date_buff[20];
  
  snprintf(date_buff, 20, "%02d%02d%02d", tmp->tm_mday, tmp->tm_mon + 1, (tmp->tm_year > 100 ? tmp->tm_year - 100 : tmp->tm_year));
  
  puts(date_buff);

  char buffer [255];
  int cx;

  cx = snprintf ( buffer, 255, "$GPRMC,%s,A,%.2f,%s,%.2f,%s,%05.1f,%05.1f,%s,%05.1f,%s*%d", "225446", 4916.451212f, "N", 12311.12324324f,"W", 0.513f, 54.7234f, date_buff, 20.312, "E", 68);

 puts (buffer);

  return 0;
}
