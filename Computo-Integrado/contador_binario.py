from gpiozero import LEDBoard
from time import sleep

led_pins = [26,16,13,17,27,22,23,24]
leds = LEDBoard(*led_pins)

def decToBin(dec):
	return [int(bit) for bit in bin(dec)[2:].zfill(8)]

try:
	for num in range(256):
		print(f"Decimal: {num}")
		bin_list = decToBin(num)
		leds.value = tuple(bin_list)
		sleep(1)

finally:
	leds.off()
