#include <pthread.h>
#include <unordered_map>
#include <stdio.h>
#define NUM_THREADS 3
#define LETTERS_PER_THREAD 2
#define LEN 5
int index = 0; // letter index
int turn = 0;
const char* letters = "abcde";
int indexToThread[NUM_THREADS];

bool canStart = false;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t canPrint[NUM_THREADS]; 
bool myTurn();

int threadToIndex(pthread_t t){
    for(int i=0; i<NUM_THREADS; ++i){
        if(indexToThread[i] == pthread_self()){
            printf("%d\n",i);
            return i;
        }
        return -1;
    }
}

void* printer(void* arg){
    while(1){
    //critical section
        pthread_mutex_lock(&lock);
        while(!canStart || !myTurn()){
            // pthread_cond_signal(&canPrint[(threadTurn(pthread_self())+1)%NUM_THREADS]);
            pthread_cond_signal(&canPrint[turn]);
            pthread_cond_wait(&canPrint[threadToIndex(pthread_self())], &lock);
        }

        printf("thread number %d: ", turn);
        for (int i=0; i<LETTERS_PER_THREAD; ++i){
            printf("%c",letters[index++]);
            index%=LEN;
        }
        printf("\n");
        turn = (turn+1)%NUM_THREADS;
        pthread_cond_signal(&canPrint[turn]);
        pthread_mutex_unlock(&lock);
    //critical section
    }
    return NULL;
}


bool myTurn(){
    return pthread_self() == indexToThread[turn];
}

int main(){
    pthread_t threads[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; ++i){
        pthread_create(&threads[i], NULL, printer, NULL);
        indexToThread[i] = threads[i];
        canPrint[i] = PTHREAD_COND_INITIALIZER; 
    }
    // pthread_mutex_lock(&lock);
    turn = indexToThread[0];
    canStart = true;
    // pthread_cond_signal(&canPrint[0]);
    // pthread_mutex_unlock(&lock);
    // for(int i=4; i>=0; --i){
    //     pthread_cond_signal(&canPrint[0]);
    // }
    


    for(int i=0; i<NUM_THREADS; ++i){
        pthread_join(threads[i], NULL);
    }

    return 0;
}