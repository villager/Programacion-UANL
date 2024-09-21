from gpiozero import DigitalInputDevice, LED
from time import sleep
#import serial

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=5)
#ser.flushInput()
# Definición de los pines
input_pin = 17  # Pin que recibe la señal del Arduino
led_red_pin = 27  # Pin donde está conectado el LED rojo

# Configuración de los pines
input_signal = DigitalInputDevice(input_pin)  # Configurar el pin de entrada como una entrada digital
led_red = LED(led_red_pin)  # Configurar el pin del LED como un LED
alreadyPrinted = 0 
lastSignal = 0
def check_signal():
    global lastSignal
    global alreadyPrinted
    #lineBytes = ser.readline()
    #line = lineBytes.decode('utf-8').strip()
    if lastSignal != input_signal.value: alreadyPrinted = 0
    lastSignal = input_signal.value
    if lastSignal == 1:
        if not alreadyPrinted:
            print("Se detecto un bajo voltaje, se enciende el LED")
            alreadyPrinted = 1
        led_red.on()
    if lastSignal == 0:
        if not alreadyPrinted:
            print("Se detecto un alto voltaje, se apaga el LED")
            alreadyPrinted = 1
        led_red.off()
try:
    while True:
        check_signal()
        sleep(1)  # Esperar 1 segundo antes de la siguiente lectura
except KeyboardInterrupt:
    print("Script interrumpido")