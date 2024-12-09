#include "mbed.h"


typedef struct {
  int value;
} message_t;
Mutex mutex;
const int queue_size = 10;
ConditionVariable cond(mutex);
MemoryPool<message_t, queue_size> mpool;
Queue<message_t, queue_size> queue;
Thread thread_producer1, thread_producer2;
//Thread thread_consumer1, thread_consumer2, thread_consumer3;
Thread thread_consumer1(osPriorityHigh), thread_consumer2(osPriorityNormal), thread_consumer3(osPriorityNormal);


int pointer = 0;

void producer (int *prod) {
    while (true) {
        wait_us(1000000);
        mutex.lock();
        int rand_value;
        do {
            rand_value = rand(); 
        }
        while (rand_value % 2 == *prod);

        message_t *message = mpool.alloc();
        message->value = rand_value;
        queue.put(message);
        pointer++;
        printf("\nProducer value %d \n", message->value);
        while (pointer == queue_size){
            printf("\nThe queue is full, wait a consumer\n");
            cond.wait();
            }
        cond.notify_all();
        mutex.unlock();
    }
}

void consumer(int *cons){
     while (true) {
        wait_us(1000000);
        mutex.lock();
          while (pointer == 0){
           printf("\nThe queue is empty, wait for a producer.\n");
           cond.wait();
            }
        osEvent evt = queue.get();
        pointer--;
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            printf("\nConsumer %d: got value %.d .\n"   , *cons, message->value);
            mpool.free(message);
        }
        cond.notify_all();
        mutex.unlock();
    }

 }

int main () {
    int prod1 = 0;
    int prod2 = 1;
    thread_producer1.start(callback(&producer, &prod1));
    thread_producer2.start(callback(&producer, &prod1));

    int cons1 = 1;
    int cons2 = 2;
    int cons3 = 3;
    thread_consumer1.start(callback(&consumer, &cons1));
    thread_consumer2.start(callback(&consumer, &cons2));
    thread_consumer3.start(callback(&consumer, &cons3));

}