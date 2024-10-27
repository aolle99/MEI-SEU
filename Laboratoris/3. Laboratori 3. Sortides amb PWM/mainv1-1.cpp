#include "mbed.h"

// Pines PWM para X e Y
PwmOut pwm_x(p21);  // Eje X
PwmOut pwm_y(p22);  // Eje Y

// Definir puntos para la Estrella de David
const float triangle1[6][2] = {
    {0.5, 1.0},  // Vértice superior
    {0.0, 0.0},  // Vértice inferior izquierdo
    {1.0, 0.0},  // Vértice inferior derecho
    {0.5, 1.0},  // Volver al vértice superior para cerrar el triángulo
};

const float triangle2[6][2] = {
    {0.5, 0.0},  // Vértice inferior
    {0.0, 1.0},  // Vértice superior izquierdo
    {1.0, 1.0},  // Vértice superior derecho
    {0.5, 0.0},  // Volver al vértice inferior para cerrar el triángulo
};

void draw_triangle(const float points[6][2], int num_points) {
    for (int i = 0; i < num_points; i++) {
        pwm_x.write(points[i][0]);  // Configurar PWM para X
        pwm_y.write(points[i][1]);  // Configurar PWM para Y
        wait(0.1);  // Espera para estabilizar el trazo
    }
}

int main() {
    pwm_x.period(0.001f);  // Período de la señal PWM
    pwm_y.period(0.001f);

    while (true) {
        // Dibujar el primer triángulo
        draw_triangle(triangle1, 4);
        
        // Dibujar el segundo triángulo invertido
        draw_triangle(triangle2, 4);
        
        wait(1);  // Espera entre ciclos de dibujo
    }
}
