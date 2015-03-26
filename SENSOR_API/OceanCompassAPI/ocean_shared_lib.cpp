#include "OceanDeviceAPI/OceanDeviceAPI.h"
#include <iostream>
using namespace std;

extern "C" bool ocean_start(char* PORT)
{
	return startOceanDevice(PORT);
}

extern "C" int ocean_getHeading() {
	double valToSend = heading;
	int sent = (int) (valToSend*100000); //salvage as much precision as possible
	return sent;
}

extern "C" int ocean_getPitch() {
	double valToSend = pitch;
	int sent = (int) (valToSend*100000); //salvage as much precision as possible
	return sent;
}

extern "C" int ocean_getRoll() {
	double valToSend = roll;
	int sent = (int) (valToSend*100000); //salvage as much precision as possible
	return sent;
}

extern "C" int ocean_getTemperature() {
	double valToSend = temperature;
	int sent = (int) (valToSend*100000); //salvage as much precision as possible
	return sent;
}

extern "C" bool ocean_stop() {
	return stopOceanDevice();
} 
