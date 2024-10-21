#include "mbed.h"
#include "Timer.h"
#include <chrono>

using namespace std::chrono;

#define WAITING_TIME 50

AnalogIn heart_sensor(p20);

// Parámetros de configuración
float pulseMinThreshold = 520;
const int minPulseInterval = 300;  // Mínimo tiempo entre pulsos en ms (para 200 PPM)
const int maxPulseInterval = 2500;  // Máximo tiempo sin pulso antes de reiniciar en ms
const float gradualChangeThreshold = 20;  // Cambio máximo permitido entre pulsos
const int windowSize = 100;

float pulseReadings[windowSize] = {0};
int readingIndex = 0;
float sumReadings = 0;
float currentBPM = 0;
float maxBPM = 0;
float minBPM = 300;
bool validPulseDetected = false;

int lastPrintTime = 0;

Timer pulseTimer;

int IBI = 600;
int rate[10];
long runningTotal = 0;
bool primerLatido = true;
bool segundoLatido = true;

void reset() {
    currentBPM = 0;
    pulseTimer.reset();
    validPulseDetected = false;
    primerLatido = true;
    segundoLatido = true;
    runningTotal = 0;
}

void updateThreshold(float currentValue) {
    sumReadings -= pulseReadings[readingIndex];
    pulseReadings[readingIndex] = currentValue;
    sumReadings += pulseReadings[readingIndex];

    readingIndex = (readingIndex + 1) % windowSize;

    float average = sumReadings / windowSize;
    pulseMinThreshold = average + 1;
}

float lowPassFilter(float currentValue, float previousValue, float alpha) {
    return alpha * currentValue + (1.0 - alpha) * previousValue;
}

void printBPM(int bpm) {
    auto currentTime = duration_cast<milliseconds>(pulseTimer.elapsed_time()).count();
    if (currentTime - lastPrintTime > 1000) {
        printf("Pulsaciones: %d \n", bpm);
        lastPrintTime = currentTime;
    }
}

int main() {
    pulseTimer.start();
    int lastPulseTime = 0;
    float lastValidBPM = 0;
    float lastFilteredValue = pulseMinThreshold;

    while (1) {
        float sensorValue = heart_sensor.read() * 1023;
        float filteredValue = lowPassFilter(sensorValue, lastFilteredValue, 0.2);
        lastFilteredValue = filteredValue;

        updateThreshold(filteredValue);

        int currentTime = duration_cast<milliseconds>(pulseTimer.elapsed_time()).count();
        int N = currentTime - lastPulseTime;

        if (filteredValue > pulseMinThreshold && N > minPulseInterval) {
            IBI = currentTime - lastPulseTime;
            lastPulseTime = currentTime;

            if (primerLatido) {
                primerLatido = false;
            }

            if (segundoLatido) {
                segundoLatido = false;
                for (int i = 0; i < 10; i++) {
                    rate[i] = IBI;
                }
            }

            runningTotal = 0;
            for (int i = 0; i < 9; i++) {
                rate[i] = rate[i + 1];
                runningTotal += rate[i];
            }
            rate[9] = IBI;
            runningTotal += rate[9];
            runningTotal /= 10;
            currentBPM = 60000 / runningTotal;
            validPulseDetected = true;

            printBPM((int)currentBPM);
        }

        if (N > maxPulseInterval) {
            reset();
        }

        ThisThread::sleep_for(chrono::milliseconds(WAITING_TIME));
    }
}
