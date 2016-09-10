#include <sys/time.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 
#include "timers.h"
using namespace std;

int main() {
   timespec startTime, endTime;
   int temp;
   InitRdtsc();
   GetRdtscTime(&startTime);
   int a = 1;
   GetRdtscTime(&endTime);
   cout << diff(startTime, endTime).tv_sec << ":" << diff(startTime, endTime).tv_nsec <<endl;
   cout << "\nTime: " << (diff(startTime, endTime).tv_nsec/pow(10,9)) << endl;
   return 0;
}