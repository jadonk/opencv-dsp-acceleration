/*
 * Copyright (c) 2010, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H

 #include "timeObj.h"
 extern FILE* benchmarkFd;


 extern Time_Object sTime;
 extern UInt32      time;

 #define OPEN_LOG_FILE(Name)                                    \
 if ((benchmarkFd = fopen(Name,"wb")) == NULL) {                \
       printf("Failed to open benchmarkFd on %s\n",Name);      \
       }

 #define CLOSE_LOG_FILE() fclose(benchmarkFd);

 #define START_BENCHMARK()                                     \
           Time_delta(&sTime,&time);

 #define LOG_STRING(str)                                        \
       fprintf(benchmarkFd,"%s",str);

 #define LOG_STRING_P1(str,p1)                                  \
       fprintf(benchmarkFd,str,(unsigned int)p1);

 #define LOG_STRING_P2(str,p1,p2)                               \
       fprintf(benchmarkFd,str,(unsigned int)p1,(unsigned int)p2);

 #define LOG_STRING_P3(str,p1,p2,p3)                                   \
       fprintf(benchmarkFd,str,(unsigned int)p1,(unsigned int)p2, \
                                    (unsigned int)p3);

 #define LOG_STRING_P4(str,p1,p2,p3,p4)                                \
       fprintf(benchmarkFd,str,(unsigned int)p1,(unsigned int)p2, \
                                    (unsigned int)p3,(unsigned int)p4);


 #define END_AND_WRITE_BENCHMARK()                             \
       Time_delta(&sTime,&time);                                \
       fprintf(benchmarkFd,"%d",(unsigned int)time);



#endif
