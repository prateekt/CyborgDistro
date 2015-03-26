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
#include "UltrasonicAPI.h"
#include <cstring>
#include <vector>
#include <map>
using namespace std;

//state
map<string,int> commPortsToPortPtrs;
map<int,string> portPtrsToCommPorts;
map<string,bool> running;
map<string,double> currentVals;
bool debugMode;

bool startUltrasonicDevice(char* commPort_cstr) {
	
	//convert to C++ string
	string commPort = string(commPort_cstr);	

	//open comm port
	int portPtr = open(commPort.c_str(), O_RDONLY | O_NOCTTY);
	if(portPtr==-1) {
		if(debugMode) {
			cout<<"Could not open comm port to "<<commPort<<" for Ultrasonic Device."<<endl;
		}
		return false;
	}
	commPortsToPortPtrs[commPort]  = portPtr;
	portPtrsToCommPorts[portPtr] = commPort;
	if(debugMode) {		
		cout<<"Connected to sonar sensor on port: "<<portPtr<<endl;
	}

	/* set the other settings*/
	struct termios settings;
	tcgetattr(portPtr, &settings);
	speed_t baud = B9600; /* baud rate */
	cfsetospeed(&settings, baud); /* baud rate */
	settings.c_cflag |= CREAD;
	settings.c_cflag &= ~PARENB;
	settings.c_cflag &= ~CSTOPB;
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8;
	settings.c_lflag = ~(ICANON | ECHO | ISIG);
	settings.c_oflag &= ~OPOST; /* raw output */
	tcsetattr(portPtr, TCSANOW, &settings); /* apply the settings */
				
	//fire thread that processes compass value
	pthread_t* serverThread = new pthread_t();
	if((pthread_create(serverThread, NULL, &UltrasonicServerThreadFunc, (void*) commPort_cstr))!=0) {
		if(debugMode) {
			cout<<"Failed to start ultrasonic device handling thread on port "<<portPtr<<"."<<endl;
		}
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

void* UltrasonicServerThreadFunc(void* arg) {

	//set device running flag to true
	char* portPtr_cstr = (char*) arg;
	string portPtr = string(portPtr_cstr);
	running[portPtr] = true;

	//loop
	while(running[portPtr]) {

		//read next sentence
		unsigned char buffer[5];
		int n = read(commPortsToPortPtrs[portPtr],buffer,sizeof(buffer));				
		char string1[5];
		sprintf(string1, "%.2X%.2X", buffer[1],buffer[2]);
		currentVals[portPtr] = atof(string1)/10;		
//		cout<<portPtr<<" "<<string1<<" "<<currentVals[portPtr]<<endl;
	}

	//gracefully close comm port
	close(commPortsToPortPtrs[portPtr]);
}

bool stopUltrasonicDevice(char* commPort_cstr) {
	
	//convert to C++ string
	string commPort = string(commPort_cstr);
	
	//see if exists in map	
	if(commPortsToPortPtrs.find(commPort) != commPortsToPortPtrs.end()) {
		//set flag to stop, port will close gracefully on next availability.
		running[commPort]= false;

		//return success
		return true;
	}
	
	//return failure
	return false;
}

double getCurrentValue(char* commPort_cstr) {
	
	//convert to C++ string
	string commPort = string(commPort_cstr);
	
	//return
	if(currentVals.find(commPort) != currentVals.end()) {
		return currentVals[commPort];
	}
	else {
		return -1; //failure
	}

}

int main() {
	startUltrasonicDevice("/dev/ttyUSB0");	
	startUltrasonicDevice("/dev/ttyUSB1");	
	startUltrasonicDevice("/dev/ttyUSB2");	
	startUltrasonicDevice("/dev/ttyUSB3");	
	startUltrasonicDevice("/dev/ttyUSB4");	
	while(true);
}
