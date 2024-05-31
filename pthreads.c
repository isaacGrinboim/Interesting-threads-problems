#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define N 20

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int i = 0;
int a[N];

void* thread_f(void* ptr){
    // pthread_mutex_lock(&lock);
    a[i] = i;
    i++;
    // pthread_mutex_unlock(&lock);
    return NULL;

}


int main(){
    pthread_mutex_init(&lock, NULL);
    pthread_t threads[N];
                                                                                    // const char* print1 = "thread";
    for (int i=0; i<N; ++i){
        pthread_create(&threads[i], NULL, thread_f, (void*)a/*(void*)print1*/);
    }
    for(int i=0; i<N; ++i){
        pthread_join(threads[i], NULL);
    }
    printf("a[N-1] = %d", a[N-1]);
    pthread_mutex_destroy(&lock);
    return 0;
}


/*
thread creation:                        //חוט גרעין או משתמש ועוד תכונות
int pthread_create(pthread_t* thread, pthread_attr_t *attr, void* (*start_routine)(void*),	void *arg); // returns 0 if succeeds
*/