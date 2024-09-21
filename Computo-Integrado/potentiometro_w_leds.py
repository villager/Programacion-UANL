from machine import ADC, Pin, PWM
import time


pot = ADC(27)
# Controlar display
a = Pin(17, Pin.OUT)
b = Pin(16, Pin.OUT)
c = Pin(14, Pin.OUT)
d = Pin(13, Pin.OUT)
e = Pin(12, Pin.OUT)
f = Pin(18, Pin.OUT)
g = Pin(19, Pin.OUT)
dp = Pin(15, Pin.OUT)

pins = [Pin(id,Pin.OUT) for id in [17,16,14,13,12,18,19,15]]
leds = [(Pin(id, Pin.OUT)) for id in [0,1,2,3,4,5,6,7,8]]

def show(code):
    for i in range(0, 8):
        pins[i].value(~code & 1)
        code = code >> 1
def setValue(f, to, fun):
    for i in range(f, to):
        leds[i].value(fun)
    
while True:
    lectura = pot.read_u16()
    print(lectura)
    time.sleep_ms(100)
    if lectura < 1000: 
        show(0xc0)
        setValue(0,9,0)
    elif lectura >= 1000 and lectura < 8100:
        show(0xf9)
        setValue(0,1,1)
        setValue(1,9, 0)
    elif lectura >= 8100 and lectura < 15200:
        show(0xa4)
        setValue(0,2,1)
        setValue(2,9,0)
    elif lectura >= 15200 and lectura < 22300:
        show(0xb0)
        setValue(0,3,1)
        setValue(3,9,0)
    elif lectura >= 22300 and lectura < 29400:
        show(0x99)
        setValue(0,4,1)
        setValue(4,9,0)
    elif lectura >= 29400 and lectura < 36500:
        show(0x92)
        setValue(0,5,1)
        setValue(5,9,0)
    elif lectura >= 36500 and lectura < 43600:
        show(0x82)
        setValue(0,6,1)
        setValue(6,9,0)
    elif lectura >= 43600 and lectura < 50700:
        show(0xf8)
        setValue(0,7,1)
        setValue(7,9,0)
    elif lectura >= 50700 and lectura < 57800:
        show(0x80)
        setValue(0,8,1)
        setValue(8,9,0)
    elif lectura >= 57800 and lectura < 65900:
        show(0x90)
        setValue(0,9,1)
        setValue(9,9,0)
