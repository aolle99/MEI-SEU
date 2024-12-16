#include "mbed.h"
#include <cstdio>

// Blinking rate in milliseconds
#define BLINKING_RATE 500ms
#define NUM_LEDS 4

DigitalOut leds[NUM_LEDS] = {LED1, LED2, LED3, LED4};

InterruptIn button(p21); // Botón externo para interrupciones
int led1 = 0, led2 = 1, led3 = 2, led4 = 3;
Mutex mx;
Thread t1, t2, t3;

void led_thread(int* led) {
    while (true) {
        if (mx.trylock()) {
            // Operaciones dentro de la sección crítica
            printf("\n\r thr%d_1\n\r", (*led) + 1);
            leds[*led] = 1;
            ThisThread::sleep_for(BLINKING_RATE);

            leds[*led] = 0;
            printf("\n\r thr%d_2\n\r", (*led) + 1);

            mx.unlock();
        }
    }
}

void led4_handler() {
    if (mx.trylock()) {
        // Encender LED4 y simular acción por interrupción
        leds[led4] = 1;
        printf("\n\r LED4 toggled by button interrupt\n\r");
        ThisThread::sleep_for(BLINKING_RATE);
        leds[led4] = 0;
        mx.unlock();
    } else {
        printf("\n\r LED4 interrupt ignored (mutex locked)\n\r");
    }
}

void init_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0;
    }
}

int main() {
    init_leds();

    // Configurar interrupción para el botón
    button.fall(&led4_handler); // Llamar a led4_handler cuando el botón se presiona

    // Crear hilos para los otros LEDs
    t1.start(callback(led_thread, &led1));
    t2.start(callback(led_thread, &led2));
    t3.start(callback(led_thread, &led3));

    // Unir los hilos
    t1.join();
    t2.join();
    t3.join();
}
