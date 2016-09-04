#include <sys/time.h>
#include <iostream>

int main() {
   timespec startTime, endTime;
   int temp;
   clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
   for(int i = 0; i < 242000000; i++) {
   	temp +=temp;
   }   
   clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
   timespec diff = diff(startTime, endTime)
   cout << diff.tv_sec << ":" << diff.tv_nsec <<endl;
   return 0;
}

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
