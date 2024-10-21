#include "mbed.h"

AnalogIn heart_sensor(p20);  
Serial pc(USBTX, USBRX);      
Timer timer;

float min_value = 1.0; // Inicialment al valor màxim possible (sensor normalitzat)
float max_value = 0.0; // Inicialment al valor mínim possible
float threshold = 0.0; // Llindar calculat automàticament
const int samples_for_threshold = 100; // Nombre de mostres abans de recalcular el llindar
int sample_count = 0; 

int beats_per_minute = 0;
int beat_count = 0;

void update_threshold(float sample) {
    // Actualitza els valors mínims i màxims
    if (sample < min_value) {
        min_value = sample;
    }
    if (sample > max_value) {
        max_value = sample;
    }

    // Quan tinguem suficients mostres, calculem el llindar
    if (sample_count >= samples_for_threshold) {
        threshold = (max_value + min_value) / 2.0; // Mitjana entre mínim i màxim
        pc.printf("Nou llindar calculat: %f\n", threshold);
        
        // Reinicialitzem per la següent ronda de càlculs
        min_value = 1.0;
        max_value = 0.0;
        sample_count = 0; // Reinicia el comptador de mostres
    }
}

int main() {
    timer.start();
    float signal_value;
    float previous_value = 0;

    while (1) {
        // Llegeix el valor del sensor
        signal_value = heart_sensor.read();  // Valor normalitzat entre 0.0 i 1.0
        
        // Actualitza el llindar automàticament després de cert nombre de mostres
        update_threshold(signal_value);
        sample_count++;

        // Detecció de pic a partir del llindar calculat
        if (signal_value > threshold && previous_value <= threshold) {
            float elapsed_time = timer.read();
            beat_count++;
            timer.reset();
            
            // Calcula BPM (Batecs per minut)
            if (elapsed_time > 0) {
                beats_per_minute = (int)(60.0 / elapsed_time);
                pc.printf("Freqüència cardíaca: %d BPM\n", beats_per_minute);
            }
        }

        previous_value = signal_value;
        wait_us(10000); // Espera per evitar sobrecàrrega de la CPU (10 ms)
    }
}
