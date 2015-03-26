import ctypes
from time import sleep
print ctypes.CDLL('Ultrasonic_device_shared_lib.so').Ultrasonic_start("/dev/ttyUSB0")
n=0;
while(n < 100):
	print ctypes.CDLL('Ultrasonic_device_shared_lib.so').Ultrasonic_getValue("/dev/ttyUSB0")
	n = n + 1
	sleep(0.1)
