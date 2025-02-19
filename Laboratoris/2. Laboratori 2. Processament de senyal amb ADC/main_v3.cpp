#include "mbed.h"
#include "Timer.h"
#include <chrono>


using namespace std::chrono;

#define WAITING_TIME     50

AnalogIn heart_sensor(p20);

// Parámetros de configuración
float pulseMinThreshold = 520;   // Umbral mínimo para detectar un pulso
const int minPulseInterval = 300;      // Mínimo tiempo entre pulsos en ms (para 200 PPM)
const int maxPulseInterval = 2000;     // Máximo tiempo sin pulso antes de reiniciar en ms
const float gradualChangeThreshold = 20; // Cambio máximo permitido entre pulsos
const int windowSize = 100;               // Ventana de promedios

float pulseReadings[windowSize] = {0};
int readingIndex = 0;
float currentBPM = 0;
float maxBPM = 0;
float minBPM = 300; // Pulso muy alto como valor inicial para asegurar correcto cálculo
bool validPulseDetected = false;
float sumReadings = 0;

int lastPrintTime = 0;

Timer pulseTimer;

void reset() {
    currentBPM = 0;
    pulseTimer.reset();
    validPulseDetected = false;
}

void updateThreshold(float currentValue) {
    // Restar el valor anterior del buffer y agregar el nuevo
    sumReadings -= pulseReadings[readingIndex];
    pulseReadings[readingIndex] = currentValue;
    sumReadings += pulseReadings[readingIndex];

    readingIndex = (readingIndex + 1) % windowSize;

    // Calcular el promedio de las últimas lecturas
    float average = sumReadings / windowSize;

    // Ajustar el umbral dinámicamente basado en el promedio
    pulseMinThreshold = average + 1;  // Ajusta este valor como una tolerancia
}

float lowPassFilter(float currentValue, float previousValue, float alpha) {
    return alpha * currentValue + (1.0 - alpha) * previousValue;
}

void printBPM(int bpm) {
    int currentTime = duration_cast<milliseconds>(pulseTimer.elapsed_time()).count();
    if (currentTime - lastPrintTime > 1000) {  // Imprimir máximo una vez por segundo
        printf("Pulsaciones: %d \n", bpm);
        lastPrintTime = currentTime;
    }
}

int main()
{
    pulseTimer.start();
    int lastPulseTime = 0;
    float lastValidBPM = 0;
    float lastFilteredValue = pulseMinThreshold;

    while (1) {
        float sensorValue = heart_sensor.read() * 1023;

        float filteredValue = lowPassFilter(sensorValue, lastFilteredValue, 0.2);
        lastFilteredValue = filteredValue;

        updateThreshold(filteredValue);

        if(sensorValue > pulseMinThreshold) {
           int currentTime = duration_cast<milliseconds>(pulseTimer.elapsed_time()).count();

           if((currentTime - lastPulseTime) > minPulseInterval) {
               float interval = (currentTime - lastPulseTime) / 1000.0;
               float bpm = 60.0 / interval;

               bool outOfRange = false;
               if (validPulseDetected && fabs(bpm - lastValidBPM) > gradualChangeThreshold) {
                   outOfRange = true;
               }
               if (!outOfRange) {
                    currentBPM = bpm;
                    maxBPM = (currentBPM > maxBPM) ? currentBPM : maxBPM;
                    minBPM = (currentBPM < minBPM) ? currentBPM : minBPM;

                    lastValidBPM = currentBPM;
                    lastPulseTime = currentTime;
                    validPulseDetected = true;
                    
                    printBPM((int) bpm);
               }
           }
        }
        int currentTime = duration_cast<milliseconds>(pulseTimer.elapsed_time()).count();
        if (currentTime - lastPulseTime > maxPulseInterval) {
            reset();  // Reiniciar si se ha excedido el tiempo máximo sin pulso
        }

        ThisThread::sleep_for(chrono::milliseconds(WAITING_TIME));
    }
}