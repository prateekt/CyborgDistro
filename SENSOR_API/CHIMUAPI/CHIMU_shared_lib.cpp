#include "CHIMUAPI/CHIMUAPI.h"
#include <iostream>
using namespace std;

extern "C" bool CHIMU_start(char* PORT)
{
	return startCHIMUDevice(PORT);
}

extern "C" int CHIMU_getCompass() {
	double valToSend = latestCompassVal;
	int sent = (int) (valToSend*100000); //salvage as much precision as possible
	return sent;
}

extern "C" bool CHIMU_stop() {
	return stopCHIMUDevice();
} 
