#include "mbed.h"
#include "Timer.h"
#include <chrono>
#include <cstdio>


using namespace std::chrono;

#define WAITING_TIME     50

AnalogIn heart_sensor(p20);

// Parámetros de configuración
float pulseMinThreshold = 520;   // Umbral mínimo para detectar un pulso
const int minPulseInterval = 300;      // Mínimo tiempo entre pulsos en ms (para 200 PPM)
const float gradualChangeThreshold = 20; // Cambio máximo permitido entre pulsos
const int windowSize = 20;               // Ventana de promedios

float pulseReadings[windowSize] = {0};
int readingIndex = 0;
float currentBPM = 0;
float minBPM = 30;
float maxBPM = 220; // Pulso muy alto como valor inicial para asegurar correcto cálculo
bool validPulseDetected = false;
float sumReadings = 0;

int lastPrintTime = 0;

Timer pulseTimer;
Timer printTimer;


float lowPassFilter(float currentValue, float previousValue, float alpha) {
    return alpha * currentValue + (1.0 - alpha) * previousValue;
}

void printBPM() {
    int currentTime = duration_cast<milliseconds>(printTimer.elapsed_time()).count();
    if (currentTime - lastPrintTime > 1000) {  // Imprimir máximo una vez por segundo
        int bpm = 0;
        int counter = 0;
        for(int num : pulseReadings) {
            if (num > 0) {
                bpm += num;
                counter++;
            }
        }
        if(counter > 0) {
            printf("Pulsaciones: %d \n", bpm / counter);
            lastPrintTime = currentTime;
        }    
    }
}

int main()
{
    pulseTimer.start();
    printTimer.start();
    int lastPulseTime = 0;
    float lastValidBPM = 0;
    float lastFilteredValue = pulseMinThreshold;

    while (1) {
        float sensorValue = heart_sensor.read() * 1023;
        if(sensorValue >= 500.f && sensorValue <= 550.f) {
            float filteredValue = lowPassFilter(sensorValue, lastFilteredValue, 0.2);
            lastFilteredValue = filteredValue;

            if(filteredValue > pulseMinThreshold) {
                int currentTime = duration_cast<milliseconds>(pulseTimer.elapsed_time()).count();
                if((currentTime - lastPulseTime) > minPulseInterval) {
                    float interval = (currentTime - lastPulseTime) / 1000.0;
                    float bpm = 60.0 / interval;
                    if(minBPM < bpm < maxBPM) {
                        pulseReadings[readingIndex % windowSize] = bpm;
                        readingIndex++;
                    }
                    lastPulseTime = currentTime; 
                }
            }
        }
        printBPM();
        wait_us(50);
    }
}