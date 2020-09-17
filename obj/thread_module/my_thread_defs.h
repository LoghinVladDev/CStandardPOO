//
// Created by Vlad on 9/17/2020.
//


#ifndef UNTITLED2_MY_THREAD_DEFS_H
#define UNTITLED2_MY_THREAD_DEFS_H

#include "my_thread.h"

#define NULL_THREAD (Thread) 0U
#define THREAD_FIRST_ID (Thread) 1U

#ifndef UINT64_MAX
#define UINT64_MAX ((uint64)~(uint64)0U)
#endif

#define THREAD_MAX (Thread) UINT64_MAX

#define CLI_BUFFER_SIZE 65536

#endif //UNTITLED2_MY_THREAD_DEFS_H