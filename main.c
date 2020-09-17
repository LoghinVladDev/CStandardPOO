#include <stdio.h>
#include <pthread.h>
#include <thread_module/my_thread.h>
#include <unistd.h>

Thread thread1;
Thread thread2;

void threadFunction () {
    printf("Coaie\n");

    fflush(stdout);
    printf("eh\n");

    while ( 1 ) {
        lock(thread1);
        unlock(thread1);
    }
}

void thread2f () {
    sleep( 1);

    lock( thread1 );

    printf("stop\n");
    fflush(stdout);

    sleep(1);

    unlock ( thread1 );
}

int main() {

    initThreadModule();

    createThread ( & thread1, threadFunction );

    createCLIAdminThread();

    while ( isAdminThreadRunning() );

    stopThreadModule();

}