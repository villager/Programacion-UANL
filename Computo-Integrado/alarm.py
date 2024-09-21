from machine import Pin
import time

sensor_pir = Pin(15, Pin.IN)
buzzer = Pin(6, Pin.OUT)
Button_OFF = Pin(7, Pin.IN, Pin.PULL_UP)
Button_Secuencia = Pin(8, Pin.IN, Pin.PULL_UP)
leds = [(Pin(id, Pin.OUT)) for id in [0,1,2,3,4,5]]

isActive = False

def pir_handler(pin):
    global isActive
    if not isActive:
        isActive = True
        print("Se detecto movimiento")
def secuencia():
    global isActive
    if isActive:
        buzzer.toggle()
        for i in range(0,6): leds[i].value(1)
        time.sleep(5)
        for i in range(0,6): leds[i].value(0)
        time.sleep(2)     

#sensor_pir.irq(trigger = Pin.IRQ_RISING, handler = pir_handler)
try:
    while True:
        if Button_OFF.value() == 0:
            isActive = False
            print("Boton de apagado")
            for i in range(0,6): leds[i].value(0)
            buzzer.off()
        if Button_Secuencia.value() == 0:
            print("Boton de secuencia")
        time.sleep(0.1)
        if sensor_pir.value() == 1:
            if not isActive:
                isActive = True
                print("Se detecta movimiento")
                secuencia()
except KeyboardInterrupt:
    for i in range(0,6): leds[i].value(0)