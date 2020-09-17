//
// Created by Vlad on 9/17/2020.
//

#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "my_thread_defs.h"
//
//static void* insideCreateThread ( void* arg ) {
//    ThreadInfo * pArg = (ThreadInfo *) arg;
//
//    void (* pFuncCasted) ( int, char** ) = ( void ( * ) (int, char**) ) ( pArg->pFunc );
//    pFuncCasted( pArg->argc, pArg->argv );
//
//    free( pArg );
//    pthread_cancel( pthread_self() );
//    return NULL;
//}
//
//ThreadResult createThreadArg ( Thread * pID, void ( * pFunction )(int, char** ), int argc, char ** argv )  {
//    ThreadInfo * threadInfo = (ThreadInfo * ) malloc(sizeof(ThreadInfo) );
//
//    threadInfo->argc = argc;
//    threadInfo->argv = argv;
//    threadInfo->pFunc = (void (*)(void))pFunction;
//
//    pthread_create( pID, NULL, insideCreateThread, threadInfo );
//
//    return 0;
//}

static void * internalCreateThreadNoArgs ( void* );

typedef struct {
    Thread              ID;
    pthread_t           pthreadHandle;
    pthread_mutex_t     pthreadMutex;
    void              * createInfo;
} ThreadHandleInfo;

struct _ThreadHandleNode { // NOLINT(bugprone-reserved-identifier)
    ThreadHandleInfo data;
    struct _ThreadHandleNode * next;
};

static struct {
    uint8               threadRunning;
} adminThreadInfo;

typedef struct _ThreadHandleNode ThreadHandleNode;

typedef ThreadHandleNode * ThreadHandleLinkedList;

static pthread_mutex_t        listLock;
static pthread_mutex_t        consoleLock;

static ThreadHandleLinkedList threadListHead    = NULL;

static ThreadHandleInfo * getThreadHandleInfo ( Thread ID ) {
    pthread_mutex_lock( & listLock );

    ThreadHandleLinkedList headCopy = threadListHead;
    while ( headCopy != NULL ) {
        if ( headCopy->data.ID == ID ) {

            pthread_mutex_unlock( & listLock );
            return (&headCopy->data);
        }
        headCopy = headCopy->next;
    }

    pthread_mutex_unlock( & listLock );
    return NULL;
}

static ThreadHandleInfo * createThreadHandleInfo ( Thread ID ) {
    pthread_mutex_lock( & listLock );
    ThreadHandleLinkedList newHead = ( ThreadHandleNode * ) malloc ( sizeof ( ThreadHandleNode ) );

    newHead->data.ID            = ID;
    newHead->data.pthreadHandle = 0U;
    newHead->data.createInfo    = NULL;
    pthread_mutex_init ( & newHead->data.pthreadMutex, NULL );

    newHead->next = threadListHead;
    threadListHead = newHead;

    pthread_mutex_unlock( & listLock );

    return ( & threadListHead->data );
}

static ThreadResult removeThreadInfo ( Thread ID ) {
    pthread_mutex_lock( & listLock );
    ThreadHandleLinkedList headCopy = threadListHead;

    if ( headCopy == NULL ) {
        pthread_mutex_unlock( & listLock );
        return THREAD_NODE_NOT_EXIST;
    }

    if ( headCopy->data.ID == ID ) {
        threadListHead = headCopy->next;
        free ( headCopy->data.createInfo );
        free ( headCopy );
        pthread_mutex_unlock( & listLock );
        return THREAD_SUCCESS;
    }

    ThreadHandleNode * previous = NULL;
    while ( headCopy->next != NULL ) {
        if ( headCopy->next->data.ID == ID ) {
            ThreadHandleNode * p = headCopy->next;
            headCopy->next = headCopy->next->next;
            free ( p->data.createInfo );
            free ( p );
            pthread_mutex_unlock( & listLock );
            return THREAD_SUCCESS;
        }
        previous = headCopy;
        headCopy = headCopy->next;
    }
    pthread_mutex_unlock( & listLock );
    return THREAD_NODE_NOT_EXIST;
}

static void clearThreadInfoList () {

    pthread_mutex_lock( & listLock );
    while ( threadListHead != NULL ) {
        ThreadHandleNode * p = threadListHead;
        threadListHead = threadListHead->next;
        pthread_mutex_destroy( & p->data.pthreadMutex );
        pthread_cancel ( p->data.pthreadHandle );
        free ( p->data.createInfo );
        free ( p );
    }

    pthread_mutex_unlock( & listLock );
}

ThreadResult killThread ( Thread ID ) {
    if ( ID == THREAD_MAX )
        return THREAD_SUCCESS;
    __auto_type pThreadInfo = getThreadHandleInfo( ID );
    if ( pThreadInfo == NULL )
        return THREAD_SUCCESS;

    pthread_mutex_destroy( & pThreadInfo->pthreadMutex );
    pthread_cancel( pThreadInfo->pthreadHandle );

    removeThreadInfo( ID );

    return THREAD_SUCCESS;
}

ThreadResult stopThread ( Thread ID ) {
    if ( ID == THREAD_MAX )
        return THREAD_SUCCESS;
    __auto_type pThreadInfo = getThreadHandleInfo( ID );
    if ( pThreadInfo == NULL )
        return THREAD_SUCCESS;

    pthread_mutex_lock( & pThreadInfo->pthreadMutex );
    pthread_join( pThreadInfo->pthreadHandle, NULL );
    pthread_mutex_unlock( & pThreadInfo->pthreadMutex );

    removeThreadInfo( ID );

    return THREAD_SUCCESS;
}

ThreadResult lock ( Thread ID ) {
    __auto_type pThreadInfo = getThreadHandleInfo( ID );
    if ( pThreadInfo == NULL )
        return THREAD_SUCCESS;

    pthread_mutex_lock ( & pThreadInfo->pthreadMutex );

    return THREAD_SUCCESS;
}

ThreadResult unlock ( Thread ID ) {
    __auto_type pThreadInfo = getThreadHandleInfo( ID );
    if ( pThreadInfo == NULL )
        return THREAD_SUCCESS;

    pthread_mutex_unlock( & pThreadInfo->pthreadMutex );

    return THREAD_SUCCESS;
}

static Thread threadCounter                     = THREAD_FIRST_ID;
static Thread adminThread                       = THREAD_MAX;
static void * adminThreadMain ( void * );

ThreadResult createThread ( Thread * pID, void ( * pFunc ) ( void ) ) {
    if ( pID == NULL || pFunc == NULL )
        return THREAD_ARG_NULL;

    * pID = threadCounter++;

    if ( threadCounter == THREAD_MAX ) {
        * pID = 0;
        return THREAD_MAX_CREATED;
    }

    __auto_type pThreadInfo = createThreadHandleInfo( * pID );
    __auto_type pThreadCreateInfo = (ThreadCreateInfoNoArgs *) malloc ( sizeof ( ThreadCreateInfoNoArgs) );

    pThreadCreateInfo->sType = THREAD_STRUCTURE_TYPE_CREATE_INFO_NO_ARGS;
    pThreadCreateInfo->pFunc = pFunc;
    pThreadCreateInfo->ID    = * pID;

    pThreadInfo->createInfo = pThreadCreateInfo;

    pthread_create ( & pThreadInfo->pthreadHandle, NULL, internalCreateThreadNoArgs, pThreadCreateInfo );
}

static void printThreadInfo ( const ThreadHandleInfo * pThreadInfo, const char * prefix ) {
    printf( "%s[Thread] [ID : %llu] [Type : %s]\n", prefix, pThreadInfo->ID, (pThreadInfo->ID == THREAD_MAX) ? "Admin Thread" : "Regular Thread" );
    fflush(stdout);
}

static void printProcesses () {
    pthread_mutex_lock( & consoleLock );
    pthread_mutex_lock( & listLock );
    ThreadHandleLinkedList headCopy = threadListHead;

    printf("Active Threads : \n");

    while ( headCopy != NULL ) {
        printThreadInfo( & headCopy->data, "\t" );

        headCopy = headCopy->next;
    }
    pthread_mutex_unlock( & listLock );
    pthread_mutex_unlock( & consoleLock );
}


static uint8 treadAdminThreadRequest ( const char* request ) {
    if ( strcmp ( request, "stop\n" ) == 0 )
        return 0;

    if ( strstr ( request, "ls" ) == request ) {
        if ( strstr ( request, "-a" ) != NULL )
            printProcesses();
    }

    if ( strstr ( request, "kill " ) == request ) {
        const char * pThreadIDStr = request + strlen ("kill ");
        while ( ! (( * pThreadIDStr >= '0' ) && ( * pThreadIDStr ) <= '9') )
            pThreadIDStr++;

        killThread( atoi ( pThreadIDStr ) );
    }

    return 1;
}

static void printAdminHelpInfo () {
    pthread_mutex_lock( & consoleLock );

    printf("Welcome to Command Line Interface for admin thread!\n");
    printf("Commands : \n");
    printf("\tls Lists all threads active\n");
    printf("\t\t-A all threads\n");
    printf("\t\t-a all threads including hidden ones\n");
    printf("\tkill [ID] - Kills thread with specified ID\n");
    printf("\tstop - Stops the program\n");

    pthread_mutex_unlock( & consoleLock );
}

static void * adminThreadMain ( void * args ) {
    printAdminHelpInfo();

    char * CLIBuffer = (char*) malloc ( sizeof ( char ) * CLI_BUFFER_SIZE );
    while (1) {
        memset ( CLIBuffer, 0, CLI_BUFFER_SIZE );
        read ( 0, CLIBuffer, CLI_BUFFER_SIZE );
//        printf ( "%s\n", CLIBuffer );
        fflush( stdout );

        if ( 0 == treadAdminThreadRequest( CLIBuffer ) )
            break;
    }

    adminThreadInfo.threadRunning = 0;

    free ( CLIBuffer );
}

void threadPrintf ( const char * format, ... ) {
    pthread_mutex_lock( & consoleLock );

    va_list args;
//    va_start ( args, format );

    pthread_mutex_unlock( & consoleLock );
}

inline uint8 isAdminThreadRunning () {
    return adminThreadInfo.threadRunning;
}

ThreadResult createCLIAdminThread () {

    adminThreadInfo.threadRunning = 1;

    __auto_type pThreadInfo = createThreadHandleInfo( adminThread );
    __auto_type pThreadCreateInfo = ( ThreadCreateInfoNoArgs * ) malloc ( sizeof ( ThreadCreateInfoNoArgs ) );

    pThreadCreateInfo->sType    = THREAD_STRUCTURE_TYPE_CREATE_INFO_NO_ARGS;
    pThreadCreateInfo->pFunc    = NULL;
    pThreadCreateInfo->ID       = adminThread;

    pThreadInfo->createInfo     = pThreadCreateInfo;

    pthread_create( & pThreadInfo->pthreadHandle, NULL, adminThreadMain, pThreadCreateInfo );
}

static void * internalCreateThreadNoArgs ( void* pVoidArg ) {
    __auto_type pArg = ( ThreadCreateInfoNoArgs * ) pVoidArg;

    if ( pArg->sType != THREAD_STRUCTURE_TYPE_CREATE_INFO_NO_ARGS )
        return NULL;

    pArg->pFunc ();

    removeThreadInfo( pArg->ID );
    return NULL;
}

ThreadResult initThreadModule () {
    pthread_mutex_init( & listLock, NULL );
    pthread_mutex_init( & consoleLock, NULL );

    return THREAD_SUCCESS;
}

ThreadResult stopThreadModule() {

    clearThreadInfoList();

    pthread_mutex_destroy( & listLock );
    pthread_mutex_destroy( & consoleLock );

    return THREAD_SUCCESS;
}
