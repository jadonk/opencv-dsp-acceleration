#include<stddef.h>
/* system time include*/
#include <sys/time.h>
#include "benchmark_time.h"

Time_Object sTime;
unsigned int time;

void Time_reset(Time_Object *sTime)
{
 struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    sTime->original = tv;
    sTime->previous = tv;

    return;
}

void Time_delta(Time_Object* sTime, unsigned int *deltaPtr)
{
    struct timeval tv;
    time_t         s;
    suseconds_t    us;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    s = tv.tv_sec - sTime->previous.tv_sec;
    us = tv.tv_usec - sTime->previous.tv_usec;

    *deltaPtr = s * 1000000l + us;

    sTime->previous = tv;

    return;
}


