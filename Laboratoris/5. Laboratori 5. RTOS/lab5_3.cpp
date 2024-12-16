#include "mbed.h"
// Blinking rate in milliseconds
#define BLINKING_RATE 500ms
#define NUM_LEDS 4
DigitalOut leds[NUM_LEDS]={LED1,LED2,LED3,LED4};
int led1=0,led2=1,led3=2,led4=3;
Mutex mx;
Thread t1, t2, t3, t4;

void led_thread(int* led) {
    while (true) {
        mx.lock();
        printf("\n\r thr%d_1\n\r", (*led) + 1);
        leds[*led] = 1;
        ThisThread::sleep_for(BLINKING_RATE);
        printf("\n\r thr%d_2\n\r", (*led) + 1);
        leds[*led] = 0;
        mx.unlock();
    }
}
void init_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0;
    }
}
int main (void) {
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
