#include "CHIMUAPI.h"
#include "CHIMUParserClass.h"
#include "globals.h"
#include <iostream>
#include <stdio.h>   /* Standard input/output definitions */
#include <string>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

//globals for API
double latestCompassVal = 0;
CHIMU_PARSER_DATA pstData;
pthread_mutex_t compassValLock = PTHREAD_MUTEX_INITIALIZER;

//private globals
bool running = FALSE;
int portPtr;

bool startCHIMUDevice(string commPort) {
	
	//if already running, return true.
	if(running) {
		return true;
	}

	//if not already running, proceed with initializing structures
	CHIMU_Init(&pstData);
	gCHIMU_Endian_Is_Small = TRUE; //important - tell parser that CHIMU J is SMALL-ENDIAN!!!
	
	//open comm port
	portPtr = open(commPort.c_str(), O_RDONLY | O_NOCTTY);
	if(portPtr==-1) {
		cout<<"Could not open comm port to "<<commPort<<" for CHIMU device."<<endl;
		return FALSE; //fail
	}

	/* set the other settings*/
	struct termios settings;
	tcgetattr(portPtr, &settings);
	speed_t baud = B115200; /* baud rate */
	cfsetospeed(&settings, baud); /* baud rate */
	settings.c_cflag &= ~PARENB; /* no parity */
	settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
	settings.c_lflag = ICANON; /* canonical mode */
	settings.c_oflag &= ~OPOST; /* raw output */
	tcsetattr(portPtr, TCSANOW, &settings); /* apply the settings */
	tcflush(portPtr, TCOFLUSH);

	//fire thread that processes compass value
	pthread_t* CHIMUServerThread = new pthread_t();
	if((pthread_create(CHIMUServerThread, NULL, &CHIMUServerThreadFunc, (void*) 1))!=0) {
		cout<<"Failed to start server thread for CHIMU device."<<endl;
		return  FALSE;
	}

	//success we're done!
	return true;
}

void* CHIMUServerThreadFunc(void* arg) {

	//set device running flag to true
	running = true;

	//loop
	while(running) {

		//read next set of bytes
		unsigned char buffer[32];
		int n = read(portPtr,buffer,sizeof(buffer));

		if(n < 0) {
			cout<<"CHIMU Read failed!\n"<<endl;
			continue; //skip
		}
		else {
			//update latest compass value buffer
			pthread_mutex_lock(&compassValLock);

			//feed bytes to decoder
			for(int i=0; i < 32; i++) {
				CHIMU_Parse(buffer[i],0,&pstData);
			}

			//convert 3D compass value to bearing
			double x = pstData.m_sensor.mag[0];
			double y = pstData.m_sensor.mag[1];
			double temp = -1;
			if(y > 0) {
				temp = 90.0 - atan(x/y)*180.0/PI;
			}
			if(y < 0) {
				temp = 270.0 - atan(x/y)*180.0/PI;
			}
			if(y==0 && x < 0) {
				temp = 180.0;
			}
			if(y==0 && x > 0) {
				temp = 0.0;
			}			

			//crude glitch detection and prevention
			if(abs(temp) > 1 && abs(temp) <= 360) {
				latestCompassVal = temp;
			} 
			
			//unlock
			pthread_mutex_unlock(&compassValLock);

		}
	}

	//gracefully close comm port
	close(portPtr);
}

bool stopCHIMUDevice() {

	//set flag to stop, port will close gracefully on next availability.
	running = false;
	
	//return true
	return true;
}

