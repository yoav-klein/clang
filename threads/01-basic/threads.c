
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *worker(void *args) {
    printf("IM WORKING\n");

    sleep(10);
    printf("OK IM DONE\n");
}

int main() {
    pthread_t thread_id;

    if(-1 == pthread_create(&thread_id, NULL, worker, NULL)) {
        perror("createing thread");
    }

    for(int i = 0; i < 10; ++i) {
         printf("WAITING FOR THAT GOD DAMN THREAD\n");
         sleep(1);
    }

    pthread_join(thread_id, NULL);
    printf("THANK GOD\n");

    return 0;   
}
