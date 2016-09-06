#include <sys/time.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 
using namespace std;

timespec diff(timespec start, timespec end);

int main() {
   timespec startTime, endTime;
   int temp;
   clock_gettime(CLOCK_REALTIME, &startTime);
   //int a = 1;
   //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
   usleep(10 * 1000000);   
   clock_gettime(CLOCK_REALTIME, &endTime);
   //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
   //timespec d = diff(startTime, endTime);
   cout << diff(startTime, endTime).tv_sec << ":" << diff(startTime, endTime).tv_nsec <<endl;
   cout << "\nTime: " << (diff(startTime, endTime).tv_nsec/pow(10,9)) << endl;
   return 0;
}

timespec diff(timespec start, timespec end) {
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
