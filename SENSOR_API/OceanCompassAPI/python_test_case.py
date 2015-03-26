import ctypes
from time import sleep
print ctypes.CDLL('ocean_device_shared_lib.so').ocean_start("/dev/ttyUSB3")
n=0;
while(n < 100):
	print ctypes.CDLL('ocean_device_shared_lib.so').ocean_getHeading() / 100000.0
	n = n + 1
	sleep(0.1)
