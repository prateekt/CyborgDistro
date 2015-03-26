#include "UltrasonicAPI/UltrasonicAPI.h"
#include <iostream>
using namespace std;

extern "C" bool Ultrasonic_start(char* PORT)
{
	return startUltrasonicDevice(PORT);
}

extern "C"  int Ultrasonic_getValue(char* PORT) {
	double valToSend = getCurrentValue(PORT);
	int sent=-1;
	if(valToSend!=-1) {
		sent = (int) (valToSend*10); //salvage as much precision as possible
	}
	return sent;	
}

extern "C" bool Ultrasonic_stop(char* PORT) {
	return stopUltrasonicDevice(PORT);
} 
