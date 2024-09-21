from machine import ADC, Pin
import time


pot = ADC(27)
a = Pin(17,Pin.OUT)
b = Pin(16,Pin.OUT)
c = Pin(14,Pin.OUT)
d = Pin(13,Pin.OUT)
e = Pin(12,Pin.OUT)
f = Pin(18, Pin.OUT)
g = Pin(19, Pin.OUT)
dp = Pin(15,Pin.OUT)

pins = [Pin(id,Pin.OUT) for id in [17,16,14,13,12,18,19,15]]

def show(code):
    for i in range(0, 8):
        pins[i].value(~code & 1)
        code = code >> 1
while True:
    lectura = pot.read_u16()
    print(lectura)
    time.sleep_ms(100)
    if lectura >= 0 and lectura < 35000: show(0xc0)
    if lectura >= 35000 and lectura < 45000: show(0xb0)
    if lectura >= 45000 and lectura < 50000: show(0x82)
    if lectura >= 50000: show(0x90)
