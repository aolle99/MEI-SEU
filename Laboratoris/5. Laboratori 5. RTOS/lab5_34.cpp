#include "mbed.h"
#include "Timer.h"
#include <chrono>
#include <cstdio>
#include <ratio>

using namespace std::chrono;

// Blinking rate in milliseconds
#define BLINKING_RATE 500ms
#define NUM_LEDS 4
DigitalOut leds[NUM_LEDS]={LED1,LED2,LED3,LED4};
int led1=0,led2=1,led3=2,led4=3;
Mutex mx;
Thread t1, t2, t3, t4;
//Thread t1(osPriorityNormal), t2(osPriorityNormal), t3(osPriorityNormal), t4(osPriorityHigh);
Timer timer;
int start_time;

void led_thread(int* led) {
    while (true) {
        int time_elapsed =duration_cast<microseconds>(timer.elapsed_time()).count();

        timer.stop();
        printf("\n Temps canvi context al led %d en: %d microsegons", (*led) + 1,time_elapsed);
        
        mx.lock();
        printf("\n\r thr%d_1\n\r", (*led) + 1);
        leds[*led] = 1;
        ThisThread::sleep_for(BLINKING_RATE);
        
        printf("\n\r thr%d_2\n\r", (*led) + 1);
        leds[*led] = 0;
        
        timer.reset();
        timer.start();

        mx.unlock();
    }
}
void init_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0;
    }
}

int main (void) {
    timer.start();
    init_leds();
    t1.start(callback(led_thread, &led1));
    t2.start(callback(led_thread, &led2));
    t3.start(callback(led_thread, &led3));
    t4.start(callback(led_thread, &led4));
    t1.join();
    t1.join();
    t3.join();
    t4.join();
}
