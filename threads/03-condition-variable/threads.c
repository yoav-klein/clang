
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>


struct package {
    int *data;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
};

void protect(int code, const char *msg) {
    if(code != 0) {
        perror(msg);
    }
}


void *consumer(void *args) {
    struct package package = *((struct package *)args);
    int time = 0;
    int random_number = 0;

    pthread_mutex_t *mutex = package.mutex;
    pthread_cond_t *cond = package.cond;
    int *data = package.data;

    pid_t pid = gettid();
    for(int i = 0; i < 5; ++i) { 
        pthread_mutex_lock(mutex);
        
        while(*data == 0) {
            printf("CONSUMER WAITING FOR DATA\n");
            pthread_cond_wait(cond, mutex);
        }
        printf("CONSUMER CONSUMED: %d\n", *data);
        *data = 0;

        pthread_cond_signal(cond); 

        pthread_mutex_unlock(mutex);
    }

}

void *producer(void *args) {
    struct package package = *((struct package *)args);
    int time = 0;
    int random_number = 0;

    pthread_mutex_t *mutex = package.mutex;
    pthread_cond_t *cond = package.cond;
    int *data = package.data;

    pid_t pid = gettid();
    
    for(int i = 0; i < 5; ++i) {
        sleep(rand() % 10);
        pthread_mutex_lock(mutex);
        
        while(*data != 0) {
            pthread_cond_wait(cond, mutex);
        }
        random_number = rand() % 100;
        printf("PRODUCER PRODUCES: %d\n", random_number);
        *data = random_number;

        pthread_cond_signal(cond);

        pthread_mutex_unlock(mutex);
    }
}

int main() {
    srand(time(NULL));
    int data = 0;

    pthread_t thread_id1, thread_id2;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    struct package package;

    package.data = &data;
    package.mutex = &mutex;
    package.cond = &cond;
    
    protect(pthread_cond_init(&cond, NULL), "cond_init");
    protect(pthread_mutex_init(&mutex, NULL), "mutex_init");
    
    protect(pthread_create(&thread_id1, NULL, producer, &package), "pthread_create");
    protect(pthread_create(&thread_id2, NULL, consumer, &package), "pthread_create");

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);


    return 0;   
}
