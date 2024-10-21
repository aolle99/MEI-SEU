#include "mbed.h"
#include "Timer.h"
#include <chrono>


using namespace std::chrono;

#define WAITING_TIME     10ms

AnalogIn heart_sensor(p20);
Serial pc(USBTX, USBRX); // tx, rx

const float threshold = 0.6;
int beats_per_minute = 0;
int beat_count = 0;

Timer timer;


int main()
{
    timer.start();
    float sensor_value;
    float previous_value = 0;
    float time_interval = 0;

    while (1) {
        //legim sensor
        sensor_value = heart_sensor.read();
        pc.printf("Sensor value: '%f'\n", sensor_value);
        
        //Detectem el pic passant per l'umbral marcat en flanc de pujada.
        if(sensor_value > threshold && previous_value <= threshold) {
            float elapsed_time = duration_cast<seconds>(timer.elapsed_time()).count();
            beat_count++;
            timer.reset();

            if(elapsed_time > 0) {
                beats_per_minute = (int)(60.0 / elapsed_time);
                pc.printf("Freqüència cardíaca: %d BPM\n", beats_per_minute);
            }
        }
        previous_value = sensor_value;
        ThisThread::sleep_for(WAITING_TIME);
    }
}
