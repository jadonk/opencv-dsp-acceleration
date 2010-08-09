#ifndef _BENCHMARK_TIME_H
#define _BENCHMARK_TIME_H

#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Define a structure to allow benchmarking using gettimeofday()
typedef struct Time_Object {
    struct timeval original;
    struct timeval previous;
} Time_Object;


void Time_reset(Time_Object *sTime);

void Time_delta(Time_Object* sTime, unsigned int *deltaPtr);


#ifdef __cplusplus
}
#endif

#endif
