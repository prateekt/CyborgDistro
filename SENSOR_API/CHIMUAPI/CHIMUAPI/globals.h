/**************************************************************************/
/*! 
    @file     globals.h
    Contains header file for global variables, macros, etc.\n
    Use in conjunction with projectconfig.h\n

    @author   M. Ryan (ryanmechatronics.com)
    @date     18 July 2011
    @version  2.0
    @section HISTORY
              - 3 Jan 2008

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2011, Ryan Mechatronics LLC
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef GLOBALS_H
#define GLOBALS_H


/** @defgroup GlobalVariables Global Monkey Variables
  * Master set of globals (structures, defines, variables, etc.) common across Monkey project
  * @{
  */

#define FALSE               0               /**< Used as boolean FALSE = 0*/
#define TRUE                !FALSE          /**< Used as boolean TRUE = 1*/
#define BIT(n)              (1L << (n))     /**< Used to bit shift easily*/

#define LOWERLIM(X,Y) ((X <= Y) ? (Y) : (X)) /**< If X<Y, return Y.  Else return X.  Used to cap at a lower limit. */
#define UPPERLIM(X,Y) ((X >= Y) ? (Y) : (X)) /**< If X>=Y, return Y.  Else return X.  Used to cap at a upper limit.*/


//Conversion defines
#define PI 3.1415926535897932384626433832795f /**< PI*/
#define NEGPI -PI /**< -PI*/
#define TWOPI 6.283185307179586476925286766558f /**< 2*PI*/
#define PI_OVER_2 1.5707963267948966192313216916395f /**< PI/2*/
#define PI_OVER_4 0.78539816339744830961566084581975f/**< PI/4*/

#define DEGREES_TO_RAD  0.017453292519943295769236907684886 /**< = radians per degree (PI/180).*/
#define RAD_TO_DEG      57.295779513082320876798154814105 /**< = degrees per radian (180/PI).*/
#define KNOTS_TO_METERS_PER_SECOND 0.5144444 /**< m/s per knot*/
#define EARTH_RADIUS  6378137.0 /**< Earth Radius (m)*/
//METERS_PER_DEG_LAT = (2.0 * pi * EARTH_RADIUS) / 360.0;
//METERS_PER_DEG_LONG = (2.0 * pi * EARTH_RADIUS * cos(gps.pos.latitude(min(find(status.gps.flags==4))) * pi/180)  ) / 360.0;
#define METERS_PER_DEG_LAT 111319.4907932736 /**< meters per degree of latitude.*/
//#define METERS_PER_DEG_LONG  92650.58575393548 (not a constant, changes with cos(latitude))
#define GRAVITY 9.81 /**< m/s^2*/

//Overall system time
#define TIMER_DT  0.001 /**< Recommend not changing unless you understand the timer / timing structures. */


/**
* A structure to represent 3d vectors v=(x,y,z)
*/
typedef struct {
	float x; /**< x*/
	float y; /**< y*/
	float z; /**< z*/
} vec3d; 

/**
* A structure to represent [3x3] matrix from vectors M = [vx vy vz]
*/
typedef struct {
	vec3d x; /**< vx*/ 
	vec3d y; /**< vy*/
	vec3d z; /**< vz*/
} mat3d;


/**
* A structure to represent quaternion in s,v format
*/
typedef struct {
        float s; /**< scalar s*/
        vec3d v; /**< vector v*/
} quat_s_v; 


/**
* A structure to represent euler angles roll, pitch, yaw
*/
typedef struct {
	float phi;      /**< roll*/
	float theta;    /**< pitch*/
	float psi;      /**< yaw*/
} euler_angles; 


/**
* A structure representing CHIMU sensor data
*/
typedef struct {
	float cputemp;      /**< temperature*/
	float acc[3];      /**< acceleration (3d)*/
	float rate[3];      /**< angular rate (3d)*/
	float mag[3];      /**< magnetic field (3d)*/
	float spare1;      /**< spare*/
	}CHIMU_sensor_data; 


/**
* A structure representing CHIMU attitude data
*/
typedef struct {
	euler_angles euler;     /**< Euler angles*/
	quat_s_v q;             /**< Quaternion*/
	}CHIMU_attitude_data; 



//Global variables
//Com Output specific
extern unsigned char gCHIMU_BIT;/**< CHIMU specific*/


//Attitude and CHIMU data
extern CHIMU_sensor_data gSensor;    /**< AHRS sensor data (accel, rates, mags).*/
extern CHIMU_attitude_data gAttitude;/**< AHRS attitude data (Eulers, quats, rates).*/
extern CHIMU_attitude_data gAttRates;/**< AHRS attitude rates only.*/
extern euler_angles gGyroBias;       /**< AHRS Gyro biases */
extern unsigned int gCHIMU_TimeCnts; /**< AHRS Time stamp */
extern unsigned char gAttitudeReceived; /**< AHRS attitude message has been received.  Not a measure of healthy attitude. */
extern unsigned int gCHIMUMissedMessages; /**< AHRS Missed messages across SPI bus */
extern short int gCHIMU_Mags[6]; /**< AHRS Magnetometer data (special).*/
extern unsigned int gOK_TO_SEND; /**< Will be removed later, but currently flags when it is OK to pass CHIMU data via Monkey TLM.*/

//CHIMU BITs and such
extern unsigned char gCHIMU_Mode; /**<CHIMU specific */
extern unsigned char gCalStatus;/**< CHIMU specific*/
extern unsigned char gConfigInfo;/**< CHIMU specific*/
extern unsigned int bC0_SPI_En; /**< CHIMU specific*/
extern unsigned int bC1_HWCentrip_En;/**< CHIMU specific*/
extern unsigned int bC2_TempCal_En;/**< CHIMU specific*/
extern unsigned int bC3_RateOut_En;/**< CHIMU specific*/
extern unsigned int bC4_TBD;/**< CHIMU specific*/
extern unsigned int bC5_Quat_Est;/**< CHIMU specific*/
extern unsigned int bC6_SWCentrip_En;/**< CHIMU specific*/
extern unsigned int bC7_AllowHW_Override;/**< CHIMU specific*/
//
extern unsigned int gCHIMU_SW_Exclaim;/**< CHIMU specific*/
extern unsigned int gCHIMU_SW_Major;/**< CHIMU specific*/
extern unsigned int gCHIMU_SW_Minor;/**< CHIMU specific*/
extern unsigned int gCHIMU_SW_SerialNumber;/**< CHIMU specific*/


//CHIMU
extern int gCHIMU_Endian_Is_Small; /**< Endian check for CHIMU AHRS (-J is small).*/
extern int gCHIMU_Auto_GPS_Vel_Enabled; /**< Allows command to AHRS for centripetal updates automatically from GPS.*/
extern int gCHIMU_Heading_Substitute_Enabled; /**<Allows magnetometer replacment heading to be sent from GPS to AHRS.*/
extern vec3d gCHIMU_MagRefVector; /**<Mag reference vector that CHIMU has stored. */
extern float gCHIMU_Declination; /**<Declination value that CHIMU has stored. */
extern float gCHIMU_Inclination; /**<Inclination value that CHIMU has stored. */
//END Group
/* @} */



#endif

