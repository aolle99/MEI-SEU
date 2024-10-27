#include "mbed.h"
#include <cstdio>


// Interpolación de pasos entre vértices (aumentar si es necesario para suavizar las líneas)
#define STEPS 4      
#define DELAY_US 1   // Tiempo entre pasos (ajustar según los límites de tiempo de 40 ms)


using namespace std::chrono;

PwmOut CH1(p21);
PwmOut CH2(p22);
AnalogIn an1(p15);
AnalogIn an2(p16);

// Definir los triángulos que forman el árbol
const float tree_top[4][2] = {
    {0.5, 0.8},  // Punto superior
    {0.3, 0.6},  // Izquierda
    {0.7, 0.6},  // Derecha
    {0.5, 0.8}   // Vuelta al superior
};

const float tree_middle[4][2] = {
    {0.5, 0.6},
    {0.25, 0.4},
    {0.75, 0.4},
    {0.5, 0.6}
};

const float tree_bottom[4][2] = {
    {0.5, 0.4},
    {0.2, 0.2},
    {0.8, 0.2},
    {0.5, 0.4}
};

// Definir el tronco
const float tree_trunk[4][2] = {
    {0.45, 0.2},  // Superior izquierdo
    {0.55, 0.2},  // Superior derecho
    {0.55, 0.1},  // Inferior derecho
    {0.45, 0.1}   // Inferior izquierdo
};

// Definir la estrella en la parte superior del árbol
const float star[10][2] = {
    {0.5, 0.9},   // Punto superior
    {0.48, 0.85}, // Punto inferior izquierdo
    {0.52, 0.88}, // Extremo derecho superior
    {0.48, 0.88}, // Extremo izquierdo superior
    {0.52, 0.85}, // Punto inferior derecho
    {0.5, 0.9},   // Volver al punto superior
};

void draw_shape(const float points[][2], int num_points) {
    for (int i = 0; i < num_points; i++) {
        pwm_x.write(points[i][0]);  // Configurar PWM para X
        pwm_y.write(points[i][1]);  // Configurar PWM para Y
        wait(DELAY_US);  // Pequeño retraso para dibujar el punto
    }
}

// Dibujar "luces" en posiciones aleatorias en el árbol
void draw_lights() {
    for (int i = 0; i < 5; i++) {  // 5 luces aleatorias
        float x = 0.3 + ((float)rand() / RAND_MAX) * 0.4;  // Posición X entre 0.3 y 0.7
        float y = 0.2 + ((float)rand() / RAND_MAX) * 0.6;  // Posición Y entre 0.2 y 0.8
        pwm_x.write(x);
        pwm_y.write(y);
        wait(DELAY_US);
    }
}

int main()
{
    CH1.period(0.001f);
    CH2.period(0.001f);

    float x, y;
    float pwm_x, pwm_y;

    while (1) {
        
        // Dibujar la estructura del árbol
        draw_shape(tree_top, 4);
        draw_shape(tree_middle, 4);
        draw_shape(tree_bottom, 4);
        draw_shape(tree_trunk, 4);

        // Dibujar la estrella en la cima
        draw_shape(star, 6);

        // Dibujar luces que parpadean
        draw_lights();

        wait(DELAY_US);
    }
}
