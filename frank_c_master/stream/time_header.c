#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t now = time(NULL);

	printf("current timestamp: %zd\n", now);

	struct tm *local_time;

	local_time = localtime(&now); // converts a time_t to struct tm in localtime zone

	char localtime_str[128];

	// formats localtime to time string
	strftime(localtime_str, sizeof(localtime_str), "%Y-%m-%d %H:%M:%S", local_time);
	printf("localtime in string: %s\n", localtime_str);


	return 0;
}
