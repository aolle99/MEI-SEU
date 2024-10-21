#include "mbed.h"
#include "Timer.h"
#include <chrono>


using namespace std::chrono;

#define WAITING_TIME     10ns

AnalogIn heart_sensor(p20);
Serial pc(USBTX, USBRX); // tx, rx

const float threshold = 0.6;

const int window_size = 10; // Nombre de mostres per a la mitjana mòbil
float buffer[window_size];
int index = 0;
float sum = 0;

int beats_per_minute = 0;
int beat_count = 0;

Timer timer;


void update_moving_average(float new_sample) {
    sum -= buffer[index]; // Resta la mostra més antiga
    buffer[index] = new_sample; // Actualitza la mostra més antiga amb la nova
    sum += new_sample; // Afegeix la nova mostra
    index = (index + 1) % window_size; // Mou l'índex circularment
}

float get_moving_average() {
    return sum / window_size;
}

int main()
{
    timer.start();
    float filtered_value;
    float previous_value = 0;
    float time_interval = 0;

    while (1) {
        //legim sensor
        float sensor_value = heart_sensor.read();
        pc.printf("Sensor value: '%f'\n", sensor_value);

        update_moving_average(sensor_value);
        filtered_value = get_moving_average();
        
        //Detectem el pic passant per l'umbral marcat en flanc de pujada.
        if(filtered_value > threshold && previous_value <= threshold) {
            float elapsed_time = duration_cast<seconds>(timer.elapsed_time()).count();
            beat_count++;
            timer.reset();

            if(elapsed_time > 0) {
                beats_per_minute = (int)(60.0 / elapsed_time);
                pc.printf("Freqüència cardíaca: %d BPM\n", beats_per_minute);
            }
        }
        previous_value = filtered_value;
        ThisThread::sleep_for(WAITING_TIME);
    }
}
