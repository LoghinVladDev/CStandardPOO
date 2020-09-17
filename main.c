#include <stdio.h>
#include <pthread.h>
#include <thread_module/my_thread.h>

static void * thSt ( void * arg ) {
    int x = *((int * ) arg);
}

void threadStartPlusPlus ( int argc, char ** argv ) {

    for (int i = 0; i < argc; ++i) {
        printf("Argument %d is %s\n", i, argv[i]);
    }

}



int main() {

    Thread threadID;

    // createThread ( & threadID, thSt, argc, argv );

    int argc = 2;
    char * argv [] = {
            "pasta",
            "de dinti"
    };

    createThread( & threadID, threadStartPlusPlus, argc, argv );

    while(1);


}