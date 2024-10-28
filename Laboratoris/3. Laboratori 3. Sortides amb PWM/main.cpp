#include "mbed.h"
#include <cstdio>


#define STEPS 500  // Nombre de punts que es dibuixen a cada línea    
#define DELAY_US 150   // Temps entre el dibuixat de cada punt
#define SIZE 20.f // Tamany del dibuix (per facilitar-ne el traçat

#define PERIOD 0.001f // Període del PWM

using namespace std::chrono;

PwmOut CH1(p21);
PwmOut CH2(p22);
AnalogIn an1(p15);
AnalogIn an2(p16);


int tree[19][2] = {
    {4,0},
    {4,2 }, 
    {0,2 }, 
    {4,5 }, 
    {1, 5}, 
    {4, 8}, 
    {2, 8}, 
    {4, 10}, 
    {3, 10}, 
    {5, 13}, 
    {7, 10}, 
    {6, 10}, 
    {8, 8}, 
    {6, 8}, 
    {9, 5}, 
    {6, 5}, 
    {10, 2}, 
    {6, 2}, 
    {6, 0}, 
};

int star[11][2] = {
    {5,13},
    {3,12},
    {4,13},
    {2,14},
    {4,15},
    {5,17},
    {6,15},
    {8,14},
    {6,13},
    {7,12},
    {5,13}
};

// Funció que ens permet dibuixar una línea entre dos punts tot interpolant els punts intermitjos
void drawLine(int x1, int y1, int x2, int y2) {
    for (int i = 0; i <= STEPS; i++) {
        int x = x1 + ((x2 - x1) * i) / STEPS;
        int y = y1 + ((y2 - y1) * i) / STEPS;
        CH1.write(x/SIZE); 
        CH2.write(y/SIZE);
        wait_us(DELAY_US);
    }
}

int main()
{
    CH1.period(PERIOD);
    CH2.period(PERIOD);
    

    float x, y;
    float pwm_x, pwm_y;

    while (1) {
        //Dibuixat dels vertexs de l'arbre de nadal
        for (int i = 0; i < 19; i++) {
            int next = (i + 1) % 19;
            drawLine(tree[i][0], tree[i][1], tree[next][0], tree[next][1]); 
        }
        // Dibuixat de l'estrella
        for (int i = 0; i < 11; i++) {
            int next = (i + 1) % 10;
            drawLine(star[i][0], star[i][1], star[next][0], star[next][1]); 
        }
    }
}
