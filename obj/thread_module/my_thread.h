//
// Created by Vlad on 9/17/2020.
//

#ifndef UNTITLED2_MY_THREAD_H
#define UNTITLED2_MY_THREAD_H

#include <pthread.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char sint8;
typedef short sint16;
typedef int sint32;
typedef long long sint64;

typedef pthread_t Thread;

int createThread ( Thread *, void (*)(int, char** ), int, char ** ) ;

#endif //UNTITLED2_MY_THREAD_H
