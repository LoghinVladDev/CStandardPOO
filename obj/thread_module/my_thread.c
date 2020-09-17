//
// Created by Vlad on 9/17/2020.
//

#include <malloc.h>
#include <string.h>
#include "my_thread.h"
#include "my_thread_defs.h"

typedef struct {
    int argc;
    char ** argv;
    void (* pFunc) (void);
} THREAD_INFO;

static void* insideCreateThread ( void* arg ) {
    THREAD_INFO * pArg = (THREAD_INFO *) arg;

    void (* pFuncCasted) ( int, char** ) = ( void ( * ) (int, char**) ) ( pArg->pFunc );
    pFuncCasted( pArg->argc, pArg->argv );

    free( pArg );
    pthread_cancel( pthread_self() );
    return NULL;
}

int createThread ( Thread * pID, void ( * pFunction )(int, char** ), int argc, char ** argv )  {
    THREAD_INFO * threadInfo = (THREAD_INFO * ) malloc( sizeof(THREAD_INFO) );

    threadInfo->argc = argc;
    threadInfo->argv = argv;
    threadInfo->pFunc = (void (*)(void))pFunction;

    pthread_create( pID, NULL, insideCreateThread, threadInfo );

    return 0;
}