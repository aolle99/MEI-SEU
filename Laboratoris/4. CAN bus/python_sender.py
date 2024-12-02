import time

import pyautogui
from gs_usb.gs_usb import GsUsb
from gs_usb.gs_usb_frame import GsUsbFrame


# gs_usb general also can import from gs_usb_structures.py
GS_USB_ECHO_ID = 0
GS_USB_NONE_ECHO_ID = 0xFFFFFFFF



GS_CAN_MODE_NORMAL = 0
GS_CAN_MODE_LISTEN_ONLY = (1 << 0)
GS_CAN_MODE_LOOP_BACK = (1 << 1)
#GS_CAN_MODE_TRIPLE_SAMPLE = (1 << 2)
GS_CAN_MODE_ONE_SHOT = (1 << 3)
GS_CAN_MODE_HW_TIMESTAMP = (1 << 4)

def main():
    # Inicializar el dispositivo CAN
    devs = GsUsb.scan()
    if len(devs) == 0:
        print("No se puede encontrar el dispositivo gs_usb")
        return
    dev = devs[0]
    print(dev)

    # Configurar el dispositivo CAN
    if not dev.set_bitrate(500000):
        print("No se puede establecer el bitrate para gs_usb")
        return
    dev.start(GS_CAN_MODE_LOOP_BACK)

    player_id=0

    # Obtener las dimensiones de la pantalla
    screen_width, screen_height = pyautogui.size()
    posx = 0
    posy=0
    direction = 1

    while True:
        iframe = GsUsbFrame()
        dev.read(iframe, 1)
        if player_id == 0:
            # Obtener la posición actual del ratón
            x, y = pyautogui.position()

            # Convertir las coordenadas a valores entre 0 y 254
            x_scaled = int((x / screen_width) * 254)
            y_scaled = int((1 - (y / screen_height)) * 254)

            # Crear el mensaje CAN
            data = bytes([x_scaled, y_scaled, 0, 0, 0, 0, 0, 0])
            frame = GsUsbFrame(can_id=0x00, data=data)

            # Enviar el mensaje CAN
            if dev.send(frame):
                print(f"TX  CAN ID: 0x00, X: {x_scaled}, Y: {y_scaled}")
        else:
            posx += direction
            posy += direction
            # Convertir las coordenadas a valores entre 0 y 254
            if posx >= 254 or posx <= 0:
                direction *= -1
            # Crear el mensaje CAN
            data = bytes([posx, posy, 0, 0, 0, 0, 0, 0])
            frame = GsUsbFrame(can_id=1, data=data)

            # Enviar el mensaje CAN
            if dev.send(frame):
                print(f"TX  CAN ID: 0x01, X: {posx}, Y: {posy}")

        # Esperar un poco antes de la siguiente actualización
        time.sleep(0.001)
        #player_id = 1 - player_id

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
