#include "mbed.h"

#define CAN_BAUDRATE 500000
// Configuración del puerto CAN: p9 y p10 son los pines de CAN en LPC1768
CAN can(p30, p29, CAN_BAUDRATE);

// LED de prueba para verificar la recepción de mensajes
DigitalOut led(LED1);

int main() {
    
    printf("Esperando mensajes CAN...\n");
    
    while (true) {
        // Objeto para almacenar el mensaje recibido
        CANMessage msg;
        
        // Verifica si hay un mensaje disponible
        if (can.read(msg)) {
            // Parpadea el LED para indicar recepción
            led = !led;

            can.write(msg);
            
            // Imprime el ID y el contenido del mensaje
            printf("Mensaje recibido: ID = 0x%X, Longitud = %d, Datos = ", msg.id, msg.len);
            
            for (int i = 0; i < msg.len; i++) {
                printf("0x%X ", msg.data[i]);
            }
            printf("\n");
        }
    
    }
}
