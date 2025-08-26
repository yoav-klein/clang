
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>

void protect(int code, const char *msg) {
    if(code != 0) {
        perror(msg);
    }
}

void *worker2(void *args) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)args;
    int time = 0;

    pid_t pid = gettid();
    
    for(int i = 0; i < 5; ++i) {
        printf("%d I'LL WAIT...\n", pid);
        time = rand() % 10;
        pthread_mutex_lock(mutex);
   
        printf("%d I GOT THE MUTEX, WORKING FOR %d SECONDS\n", pid, time);
        sleep(time);
    
        printf("%d UNLOCKING IT\n", pid);
        pthread_mutex_unlock(mutex);
    }

}

void *worker1(void *args) {
    printf("IM WORKING\n");

    sleep(10);
    printf("OK IM DONE\n");
}

int main() {
    srand(time(NULL));

    pthread_t thread_id1, thread_id2;
    pthread_mutex_t mutex;
    
    protect(pthread_mutex_init(&mutex, NULL), "mutex_init");
    
    protect(pthread_create(&thread_id1, NULL, worker2, &mutex), "pthread_create");
    protect(pthread_create(&thread_id2, NULL, worker2, &mutex), "pthread_create");

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);


    return 0;   
}
