#include <pthread.h>
int readers_inside = 0;
int writers_inside = 0;
int writers_waiting = 0;
pthread_cond_t read_allowed = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_allowed = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void readers_lock(){
    pthread_mutex_lock(&lock);
    while(writers_inside+writers_waiting > 0){
        pthread_cond_wait(&read_allowed, &lock);
    }
    readers_inside++;
    pthread_mutex_unlock(&lock);
}

void readers_unlock(){
    pthread_mutex_lock(&lock);
    readers_inside--;
    if(readers_inside == 0){
        pthread_cond_signal(&write_allowed);
    }
    pthread_mutex_unlock(&lock);
}

void writers_lock(){
    pthread_mutex_lock(&lock);
    writers_waiting++;
    while(readers_inside + writers_inside > 0){
        pthread_cond_wait(&write_allowed, &lock);
    }
    writers_inside++;
    writers_waiting--;
    pthread_mutex_lock(&lock);
}

void writers_unlock(){
    pthread_mutex_lock(&lock);
    writers_inside--;
    pthread_cond_signal(&write_allowed);
    pthread_cond_broadcast(&read_allowed);
    pthread_mutex_lock(&lock);
}