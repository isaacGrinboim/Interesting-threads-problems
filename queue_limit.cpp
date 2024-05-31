#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#define QUEUE_LIMIT 10
using namespace std;

std::queue<int> q;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int queue_size = 0;


int main(){
  std::cout<<"Hello!"<<std::endl;
  return 0;
}
void enqueue(int x) {
  pthread_mutex_lock(&m);
      while(queue_size == QUEUE_LIMIT){
        pthread_cond_wait(&notFull, &m);
      }
      q.push(x);
      queue_size++;
      pthread_cond_signal(&notEmpty);
  pthread_mutex_unlock(&m);
}

int dequeue() {
  pthread_mutex_lock(&m);
      while (queue_size == 0) {
        pthread_cond_wait(&notEmpty, &m);
      }
      int res = q.front();
      q.pop();
      queue_size--;
      pthread_cond_signal(&notFull);
  pthread_mutex_unlock(&m);
  return res;
}

//  