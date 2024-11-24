#include "mbed.h"

#define CAN_BAUDRATE 500000
#define DELAY_US 150

CAN can(p30, p29, CAN_BAUDRATE);  // CAN RX, CAN TX
AnalogOut OUTX(p18);  // Salida analógica para el primer punto
AnalogOut OUTY(p19);  // Salida analógica para el segundo punto

int main() {
    CANMessage msg;
    while (1) {
        
        if (can.read(msg)) {
            if (msg.id == 0x00 || msg.id == 0x01) {
                float x = static_cast<float>(msg.data[0]) / 255.0f;
                float y = static_cast<float>(msg.data[1]) / 255.0f;
                
                if (msg.id == 0x00) {
                    OUTX.write(x);
                    OUTY.write(y);
                } else {  // msg.id == 0x01
                    OUTX.write(y);
                    OUTY.write(x);
                }
            }
        }
        delay_us(DELAY_US);
    }
}
