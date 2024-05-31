#include <pthread.h>
#include <string.h> 
#include <stdio.h>
#define NUM_THREADS 10
#define LETTERS_PER_THREAD 4
#define STRING_LEN (strlen(letters))

int letterIndex = 0; // letter letterIndex
int turnNum = 0;
const char* letters = "0123456789";
int letterIndexToThread[NUM_THREADS];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t canPrint = PTHREAD_COND_INITIALIZER; 
void init(pthread_t* threads);
void joinThreads(pthread_t* threads);
bool myTurn();
void print();



void* printer(void* args){
    while(1){
    //critical section
        pthread_mutex_lock(&lock);
        while(!myTurn()){
            // pthread_cond_signal(&canPrint);
            pthread_cond_wait(&canPrint, &lock);
        }
        print();
        
        turnNum = (turnNum+1)%NUM_THREADS;
        pthread_cond_broadcast(&canPrint);
        pthread_mutex_unlock(&lock);
    //critical section
    }
    return NULL;
}

int main(){
    pthread_t threads[NUM_THREADS];
    init(threads);
    joinThreads(threads);
    pthread_cond_destroy(&canPrint);
    pthread_mutex_destroy(&lock);
    return 0;
}


bool myTurn(){
    return pthread_self() == letterIndexToThread[turnNum];
}

void print(){
    printf("thread number %d: ", turnNum);
    for (int i=0; i<LETTERS_PER_THREAD; ++i){
        printf("%c",letters[letterIndex++]);
        letterIndex%=STRING_LEN;
    }
    printf("\n");
}

void init(pthread_t* threads){
        for(int i=0; i<NUM_THREADS; ++i){
        pthread_create(&threads[i], NULL, printer, NULL);
        letterIndexToThread[i] = threads[i];
    }
}

void joinThreads(pthread_t* threads){
    for(int i=0; i<NUM_THREADS; ++i){
        pthread_join(threads[i], NULL);
    }
}