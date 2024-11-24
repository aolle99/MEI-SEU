import time
import pyautogui
from gs_usb.gs_usb import GsUsb
from gs_usb.gs_usb_frame import GsUsbFrame

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
    dev.start(GS_CAN_MODE_NORMAL)

    # Obtener las dimensiones de la pantalla
    screen_width, screen_height = pyautogui.size()

    while True:
        # Obtener la posición actual del ratón
        x, y = pyautogui.position()

        # Convertir las coordenadas a valores entre 0 y 254
        x_scaled = int((x / screen_width) * 254)
        y_scaled = int((y / screen_height) * 254)

        # Crear el mensaje CAN
        data = bytes([x_scaled, y_scaled, 0, 0, 0, 0, 0, 0])
        frame = GsUsbFrame(can_id=0x00, data=data)

        # Enviar el mensaje CAN
        if dev.send(frame):
            print(f"TX  CAN ID: 0x00, X: {x_scaled}, Y: {y_scaled}")

        # Esperar un poco antes de la siguiente actualización
        time.sleep(0.1)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
