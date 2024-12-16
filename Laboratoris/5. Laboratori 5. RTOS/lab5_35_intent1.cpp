#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_RATE 500ms
#define NUM_LEDS 4

DigitalOut leds[NUM_LEDS] = {LED1, LED2, LED3, LED4};
Timeout led4_timeout; // Timeout for LED4
int led1 = 0, led2 = 1, led3 = 2, led4 = 3;
Mutex mx;
Thread t1, t2, t3;
int context_switch_overhead = 0;
bool interrupt_triggered = false;

void led_thread(int* led) {
    while (true) {
        if(mx.trylock()) {

            // Measure start time before critical section
            printf("\n\r thr%d_1\n\r", (*led) + 1);
            leds[*led] = 1;
            ThisThread::sleep_for(BLINKING_RATE);

            // Measure end time after critical section
            leds[*led] = 0;
            printf("\n\r thr%d_2\n\r", (*led) + 1);

            mx.unlock();
        }
    }
}

void led4_handler() {
    bool already_executed = false;
    while (!already_executed) {
        if(mx.trylock()) {
            leds[led4] = 1;
            printf("\n\r LED4 toggled by timeout interrupt\n\r");
            //interrupt_triggered = false;
            ThisThread::sleep_for(BLINKING_RATE);
            leds[led4] = 0;
            mx.unlock();
            already_executed = true;
            
            led4_timeout.attach(&led4_handler, 2s);
        }
        
    }    
}

void init_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0;
    }
}

int main() {
    init_leds();
    // Attach timeout for LED4 toggle
    led4_timeout.attach(&led4_handler, 2s); // Toggle LED4 every 2 seconds

    // Create threads for other LEDs
    t1.start(callback(led_thread, &led1));
    t2.start(callback(led_thread, &led2));
    t3.start(callback(led_thread, &led3));

    // Join threads
    t1.join();
    t2.join();
    t3.join();
}
