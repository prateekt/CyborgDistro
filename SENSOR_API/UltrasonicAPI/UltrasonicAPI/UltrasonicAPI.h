#ifndef ULTRASONIC_API_H
#define ULTRASONIC_API_H

//imports
#include <string>  /* String function definitions */
#include <pthread.h>
using namespace std;

//API functions for developers
bool startUltrasonicDevice(char* commPort); //returns true if compass could be started on comm port
bool stopUltrasonicDevice(char* commPort); //returns true if compass could be stopped on comm port
double getCurrentValue(char* commPort);

//used internally
void* UltrasonicServerThreadFunc(void* arg);
char *substring(char *string, int position, int end);

#endif
