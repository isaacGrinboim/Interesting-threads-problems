#include <pthread.h>
#include <ios>
#include <streambuf>
#include <istream>
#include <ostream>
#include <iostream>

int i = 0;
int j = 0;

void* worker_function(void* nothing){
    for(j=0; j<10000; ++j){
        i++;
    }
    return NULL;
}

int main(){
    pthread_t t1 = pthread_create(&t1, NULL, worker_function, NULL);
    pthread_t t2 = pthread_create(&t2, NULL, worker_function, NULL);
    std::cout << "i is now: " << i << std::endl;

    return 0;

}