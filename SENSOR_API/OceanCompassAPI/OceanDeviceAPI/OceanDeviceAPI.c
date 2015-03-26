#include <iostream>
#include <stdio.h>   /* Standard input/output definitions */
#include <string>    /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "OceanDeviceAPI.h"
#include <cstring>
using namespace std;

//global returns
double heading = 0.0;
double pitch = 0.0;
double roll = 0.0;
double temperature = 0.0;
pthread_mutex_t valLock = PTHREAD_MUTEX_INITIALIZER;

//state
bool running = false;
int portPtr = -1;

bool startOceanDevice(string commPort) {

	//easy case
	if(running) {
		return true;
	}

	//open comm port
	portPtr = open(commPort.c_str(), O_RDONLY | O_NOCTTY);
	if(portPtr==-1) {
		cout<<"Could not open comm port to "<<commPort<<" for Ocean Device."<<endl;
		return false;
	}

	/* set the other settings*/
	struct termios settings;
	tcgetattr(portPtr, &settings);
	speed_t baud = B19200; /* baud rate */
	cfsetospeed(&settings, baud); /* baud rate */
	settings.c_cflag &= ~PARENB; /* no parity */
	settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
	settings.c_lflag = ICANON; /* canonical mode */
	settings.c_oflag &= ~OPOST; /* raw output */
	tcsetattr(portPtr, TCSANOW, &settings); /* apply the settings */
	
	//fire thread that processes compass value
	pthread_t* compassServerThread = new pthread_t();
	if((pthread_create(compassServerThread, NULL, &OceanServerThreadFunc, (void*) 1))!=0) {
		cout<<"Failed to start ocean device handling thread."<<endl;
		return  false;
	}
	
	//return success
	return true;

}

char *substring(char *string, int position, int end) 
{
	int length = (end - position)+1;
	char *rtn = new char[length];
	for(int x=position; x < end; x++) {
		rtn[x-position] = string[x];
	}
	rtn[end] = 0;
 
   return rtn;
}

void* OceanServerThreadFunc(void* arg) {

	//set device running flag to true
	running = true;

	//loop
	while(running) {

		//read next sentence
		unsigned char buffer[32];
		int n = read(portPtr,buffer,sizeof(buffer));
		
		//read
		if(n < 0) {
			cout<<"Compass Read failed!\n"<<endl;
			continue; //skip
		}
		else {			
						
			//parser
			int locC=-1;
			int locP=-1;
			int locR=-1;
			int locT=-1;
			int locStar=-1;
			for(int x=0; x < 32; x++) {
				if(locC==-1 && buffer[x]=='C') {
					locC = x;
				}
				if(locP==-1 && buffer[x]=='P') {
					locP = x;
				}
				if(locR==-1 && buffer[x]=='R') {
					locR = x;
				}
				if(locT==-1 && buffer[x]=='T') {
					locT = x;
				}
				if(locStar==-1 && buffer[x]=='*') {
					locStar=x;
				}
			}	
			
			//message validity check
			if(locC==-1 || locP==-1 || locR==-1 || locT==-1 || locStar==-1) {
				cout<<"Ocean compass returned unparsable message."<<endl;
			}	
			else {

				//parse from string
				char* tempHeadingStr = substring(buffer,locC+1,locP);
				char* tempPitchStr = substring(buffer,locP+1,locR);
				char* tempRollStr = substring(buffer,locR+1,locT);
				char* tempTempStr = substring(buffer,locT+1,locStar);
				double tempHeading = atof(tempHeadingStr);
				double tempPitch = atof(tempPitchStr);
				double tempRoll = atof(tempRollStr);
				double tempTemp = atof(tempTempStr);

				//update latest values
				pthread_mutex_lock(&valLock);
				heading = tempHeading;
				pitch = tempPitch;
				roll = tempRoll;	
				temperature = tempTemp;
				pthread_mutex_unlock(&valLock);
			}
		}
	}

	//gracefully close comm port
	close(portPtr);
}

bool stopOceanDevice() {

	//set flag to stop, port will close gracefully on next availability.
	running = false;
	
	//return true
	return true;
}
