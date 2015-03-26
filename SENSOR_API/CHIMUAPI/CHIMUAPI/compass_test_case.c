#include <iostream>
#include "CHIMUParserClass.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <math.h>
#include <stdlib.h>
#include "globals.h"
#include "CHIMUAPI.h"
#include <sys/time.h>
using namespace std;

#define CHIMU_PORT "/dev/ttyUSB3"
#define POLL_WAIT 500

//logger program
int main() {

        struct timeval starttime,endtime,timediff;
	gettimeofday(&starttime,0x0);
	bool started = startCHIMUDevice(CHIMU_PORT);
	if(started) {
		while(true) {
			long seconds  = endtime.tv_sec  - starttime.tv_sec;
			long useconds = endtime.tv_usec - starttime.tv_usec;
			double ts = (seconds + useconds/1000000.0) ;
			
			cout<<ts<<","<<pstData.m_sensor.acc[0]<<","<<pstData.m_sensor.acc[1]<<","<<pstData.m_sensor.acc[2]<<","<<pstData.m_sensor.rate[0]<<","<<pstData.m_sensor.rate[1]<<","<<pstData.m_sensor.rate[2]<<","<<pstData.m_attitude.euler.phi<<","<<pstData.m_attitude.euler.theta<<","<<pstData.m_attitude.euler.psi<<","<<pstData.m_attrates.euler.phi<<","<<pstData.m_attrates.euler.theta<<","<<pstData.m_attrates.euler.psi<<endl;
			pthread_mutex_unlock(&compassValLock);
			gettimeofday(&endtime,0x0);
			usleep(POLL_WAIT);
		}
	}
}

//example user program for compass

/*
int main() {
	bool started = startCompass("/dev/ttyUSB0");
	if(started) {	
		for(int x=0; x < 100000; x++) {
			pthread_mutex_lock(&compassValLock);
			cout<<latestCompassVal<<endl;
			pthread_mutex_unlock(&compassValLock);
			usleep(10000);
		}
		stopCompass();
	}
}
*/

/*
int
read_port(void)
{
	CHIMU_PARSER_DATA pstData;
	CHIMU_Init(&pstData);
	gCHIMU_Endian_Is_Small = TRUE;
    int fd = open("/dev/ttyUSB3", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyS0 - ");
    }
    for(int x=0; x< 1000000; x++) {
	    unsigned char buffer[32];
	    int n = read(fd, buffer, sizeof(buffer));
	    if (n < 0)
		fputs("read failed!\n", stderr);
	    for(int i=0; i < 32; i++) {
//		printf("%x ",buffer[i]);
		CHIMU_Parse(buffer[i],0,&pstData);
	    }
		printf("%f\n",pstData.m_sensor.mag[0]);
//		cout<<gCHIMU_Endian_Is_Small<<endl;
    }
    return (fd);
}

int main() {
read_port();
}*/
