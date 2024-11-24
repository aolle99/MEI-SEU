#include "mbed.h"

CAN can(p30, p29);  // CAN RX, CAN TX
AnalogOut aout1(p18);  // Salida analógica para el primer punto
AnalogOut aout2(p19);  // Salida analógica para el segundo punto

int main() {
    can.frequency(250000);  // Configura la frecuencia del bus CAN a 250 kbps
    CANMessage msg;
    while (1) {
        
        if (can.read(msg)) {
            if (msg.id == 0x00 || msg.id == 0x01) {
                float x = static_cast<float>(msg.data[0]) / 255.0f;
                float y = static_cast<float>(msg.data[1]) / 255.0f;
                
                if (msg.id == 0x00) {
                    aout1.write(x);
                    aout2.write(y);
                } else {  // msg.id == 0x01
                    aout1.write(y);
                    aout2.write(x);
                }
            }
        }
        ThisThread::sleep_for(20ms);
    }
}
