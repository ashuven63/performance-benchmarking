#ifndef TIMERS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define TIMERS_H
#include <sys/time.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <thread>         
#include <chrono> 

timespec diff(timespec start, timespec end);

// Rdtsc calls
void GetRdtscTime(struct timespec *ts);
void InitRdtsc();

#endif