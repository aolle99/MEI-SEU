#include "mbed.h"
#include <cmath>  // Para sqrt y pow

#define CAN_BAUDRATE 500000
#define DELAY_US 10000  // Intervalo de actualización (10 ms)
#define PERIOD 0.001f
#define THRESHOLD 20.0f  // Distancia mínima para terminar el juego

CAN can(p30, p29, CAN_BAUDRATE);  // CAN RX, CAN TX
AnalogOut OUTX(p18);  // Salida analógica
PwmOut OUTY(p21);  // Salida PWM
DigitalOut led(LED1);

float coords[2][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}}; // Coordenadas [jugador][x, y]
bool initialized[2] = {false, false};  // Indicador de si cada jugador ya envió datos

bool check_proximity(float x1, float y1, float x2, float y2) {
    // Calcula la distancia entre dos puntos y verifica si están cerca
    float distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance < THRESHOLD;
}

int main() {
    CANMessage msg;
    OUTY.period(PERIOD);
    int current_player = 0;  // Alterna entre jugador 0 y jugador 1

    while (1) {
        if (can.read(msg)) {
            if (msg.id == 0x00 || msg.id == 0x01) {
                int player = msg.id; // 0 para Jugador 0, 1 para Jugador 1
                float x = static_cast<float>(msg.data[0]) / 255.0f;
                float y = static_cast<float>(msg.data[1]) / 255.0f;

                coords[player][0] = x;  // Actualiza coordenada X
                coords[player][1] = y;  // Actualiza coordenada Y
                initialized[player] = true;  // Marca como inicializado
                
                printf("Jugador %d: X: %d Y: %d\n", player, (int)(x * 100), (int)(y * 100));
            }
        }

        // Solo procede si ambos jugadores han enviado datos
        if (initialized[0] && initialized[1]) {
            // Verifica proximidad
            if (check_proximity(coords[0][0], coords[0][1], coords[1][0], coords[1][1])) {
                printf("¡Juego terminado! Jugadores demasiado cerca.\n");
                OUTX.write(0.0f);
                OUTY.write(0.0f);
                led = 0;  // Apaga el LED
                break;  // Sale del juego
            } else {
                // Alterna la visualización entre los dos jugadores
                OUTX.write(coords[current_player][0]);  // Escribe X del jugador actual
                OUTY.write(coords[current_player][1]);  // Escribe Y del jugador actual
                current_player = 1 - current_player;  // Cambia entre jugador 0 y jugador 1
                led = 1;  // Enciende el LED para indicar que el juego sigue
            }
        } else {
            // Dibuja ambos puntos en (0,0) mientras no se inicialicen
            OUTX.write(0.0f);
            OUTY.write(0.0f);
            led = 0;  // Apaga el LED
        }

        wait_us(DELAY_US);
    }
}
