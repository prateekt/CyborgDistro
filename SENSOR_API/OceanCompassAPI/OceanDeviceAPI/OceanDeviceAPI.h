#ifndef OCEAN_DEVICE_API_H
#define OCEAN_DEVICE_API_H

//imports
#include <string>  /* String function definitions */
#include <pthread.h>
using namespace std;

//API vars for developers
extern double heading;
extern double pitch;
extern double roll;
extern double temperature;
extern pthread_mutex_t valLock;

//API functions for developers
bool startOceanDevice(string commPort); //returns true if compass could be started on comm port
bool stopOceanDevice(); //returns true if compass could be stopped on comm port

//used internally
void* OceanServerThreadFunc(void* arg);
char *substring(char *string, int position, int end);

#endif
