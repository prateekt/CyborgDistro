#ifndef CHIMU_API_H
#define CHIMU_API_H

//imports
#include <string>  /* String function definitions */
#include <pthread.h>
#include "globals.h"
#include "CHIMUParserClass.h"
using namespace std;

//API vars for developers
extern double latestCompassVal;
extern pthread_mutex_t compassValLock;
extern CHIMU_PARSER_DATA pstData;

//API for compass developers
bool startCHIMUDevice(string commPort); //returns true if compass could be started on comm port
bool stopCHIMUDevice(); //returns true if compass could be stopped on comm port

//used internally
void* CHIMUServerThreadFunc(void* arg);

#endif


