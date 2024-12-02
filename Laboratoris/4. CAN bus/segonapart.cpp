#include "mbed.h"
#include <cstdio>

#define CAN_BAUDRATE 500000
#define DELAY_US 10
#define PERIOD 0.001f

CAN can(p30, p29, CAN_BAUDRATE);  // CAN RX, CAN TX
AnalogOut OUTX(p18);  // Salida analógica para el primer punto
PwmOut OUTY(p21);  // Salida analógica para el segundo punto
DigitalOut led(LED1);
int main() {
    CANMessage msg;
    OUTY.period(PERIOD);
    while (1) {
        if (can.read(msg)) {
            if (msg.id == 0x00 || msg.id == 0x01) {
                float x = static_cast<float>(msg.data[0]) / 255.0f;
                float y = static_cast<float>(msg.data[1]) / 255.0f;
                if(msg.id == 0x00) {
                    printf("Jugador 0: X: %d Y: %d \n", (int)(x*100),(int)(y*100));

                    OUTX.write(x);
                    OUTY.write(y);
                }
                else if(msg.id == 0x01) {
                    printf("Jugador 0: X: %d Y: %d \n", (int)(x*100),(int)(y*100));

                    OUTX.write(x);
                    OUTY.write(y);
                }
                
                
            }
        }
        wait_us(DELAY_US);
    }
}
