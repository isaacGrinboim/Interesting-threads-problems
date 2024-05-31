#define N 10
#include <deque>
#include <pthread.h>
#include <utility>  // for std::pair
#include <string>
using namespace std;


deque<pair<void*, int>> write_reqs;
pthread_mutex_t m;
pthread_cond_t notEmpty;
pthread_t hardWriter;

class Storage {
public:
  void init();
  void write(void *buffer, int offset); // all offsets are aligned to 4KB, buffer sizes are always 4KB.
  void read(void *buffer, int offset);

private:

  static void* threadWrite(void* arg);
  void fillBuffer(void* dest, void* source);
  void drive_write(void *buffer, int offset);
  void drive_read(void *buffer, int offset);
};


// Basic
void Storage::init(){
    pthread_create(&hardWriter, NULL, Storage::threadWrite, NULL);
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&notEmpty, NULL);
}


void* Storage::threadWrite(void* args){//dequeue:
  while(1){
    pthread_mutex_lock(&m);
    while(write_reqs.empty()){
      pthread_cond_wait(&notEmpty, &m);
    }
    pair<void*,int> request = write_reqs.front();
  	write_reqs.pop_back();
    pthread_mutex_unlock(&m);
    drive_write(request.first, request.second);
  }
}


void Storage::write(void *buffer, int offset) { // enqueue
  pthread_mutex_lock(&m);
  write_reqs.push_back({buffer,offset});
  pthread_cond_signal(&notEmpty);
  pthread_mutex_unlock(&m);
  return;
  // drive_write(buffer, offset);
}


void Storage::read(void *buffer, int offset) {

  pthread_mutex_lock(&m);
  for(auto req : write_reqs){
    if(req.second == offset){  // should be the offset
  		fillBuffer(buffer, req.first); // should be the buffer from request  	
      pthread_mutex_unlock(&m);
      return;
      }
  pthread_mutex_unlock(&m);
	drive_read(buffer, offset);
  return;
  }
}

void fillBuffer(void* dest, void* source){
  memcpy(dest, source, 4096);
}



int main() {
  char *buffer = "bla";
  Storage s;
  s.init();
  s.write(buffer, 0);
  char out_buffer[4096];
  s.read(out_buffer, 0);
  // memcmp out_buffer and buffer should be 0
}