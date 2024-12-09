#include "mbed.h"
#include "Timer.h"
#include <chrono>

using namespace std::chrono;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

Mutex mx;
Thread t1,t2,t3;

// Comptador compartit
int shared_counter = 0;

void led_1_blinking(){
    Timer time1;
    time1.start();
    while (true) {
        int currentTime = duration_cast<milliseconds>(time1.elapsed_time()).count();
        if(currentTime >=300) {
            led1 = !led1;
            time1.reset();
            time1.start();
        }
    }
}

void led_2_3_blink(DigitalOut *led) {
    while (true) {
        mx.lock();
        *led = 1;
        ThisThread::sleep_for(1000ms); 
        *led=0;
        mx.unlock();
    }
}


int main() {
    t1.start(callback(led_1_blinking));
    t2.start(callback(led_2_3_blink, &led2));
    t3.start(callback(led_2_3_blink, &led3));
    while (1) {
        wait_us(1000000);
    }
}