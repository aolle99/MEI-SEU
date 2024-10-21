#include "mbed.h"
#include <cstdio>

#define PWM_MAX 255    // Valor máximo de PWM
#define VOLTAGE_MAX 1.0 // Máxima amplitud en volts (1V por eje)
#define PWM_CENTER 128.0  // Punto central (0V)
#define SIDE_LENGTH 0.5 // Distancia desde el centro a los vértices (0.5V)

// Interpolación de pasos entre vértices (aumentar si es necesario para suavizar las líneas)
#define STEPS 300      
#define DELAY_US 1   // Tiempo entre pasos (ajustar según los límites de tiempo de 40 ms)


using namespace std::chrono;

PwmOut CH1(p21);
PwmOut CH2(p22);
AnalogIn an1(p15);
AnalogIn an2(p16);

double side_size = SIDE_LENGTH * PWM_MAX / VOLTAGE_MAX;

double vertices[4][2] = {
    {PWM_CENTER + side_size, PWM_CENTER + side_size}, 
    {PWM_CENTER - side_size, PWM_CENTER + side_size},
    {PWM_CENTER - side_size, PWM_CENTER - side_size}, 
    {PWM_CENTER + side_size, PWM_CENTER - side_size} 
};

void drawLine(int x1, int y1, int x2, int y2) {
    for (int i = 0; i <= STEPS; i++) {
        int x = x1 + ((x2 - x1) * i) / STEPS;
        int y = y1 + ((y2 - y1) * i) / STEPS;
        CH1.write(x/(PWM_MAX*1.0)); 
        CH2.write(y/(PWM_MAX*1.0));
        wait_us(DELAY_US);
    }
}

int main()
{
    CH1.period(0.0001);
    CH2.period(0.001);

    float x, y;
    float pwm_x, pwm_y;

    while (1) {
        
        for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        drawLine(vertices[i][0], vertices[i][1], vertices[next][0], vertices[next][1]); 
    }
    }
}