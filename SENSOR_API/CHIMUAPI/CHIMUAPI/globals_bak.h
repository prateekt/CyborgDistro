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

#include <CMSIS/LPC17xx.h> 
#include <LPC1768.h>
#include "monkey_types.h"
#include "projectconfig.h"


/** @defgroup GlobalVariables Global Monkey Variables
  * Master set of globals (structures, defines, variables, etc.) common across Monkey project
  * @{
  */

#define FALSE               0               /**< Used as boolean FALSE = 0*/
#define TRUE                !FALSE          /**< Used as boolean TRUE = 1*/
#define BIT(n)              (1L << (n))     /**< Used to bit shift easily*/

#define NUMPORTS 2              /**< Number of serial ports used by user.  Rarely more than 2, as UART2 is for GPS and UART3 is shared by CHIMU*/
#define UART0_USER    0x00      /**< Main Com Port*/
#define UART1_CHIMU   0x01      /**< Spare Com Port*/
//#define UART2_GPS     0x02
//#define UART3_SPARE   0x03
#define UART_BUFFERSIZE 256     /**< Reserved buffer size (bytes)*/

#define LOWERLIM(X,Y) ((X <= Y) ? (Y) : (X)) /**< If X<Y, return Y.  Else return X.  Used to cap at a lower limit. */
#define UPPERLIM(X,Y) ((X >= Y) ? (Y) : (X)) /**< If X>=Y, return Y.  Else return X.  Used to cap at a upper limit.*/

#define USE_PLL/**< Normal operation is PLL engaged, high speed */
#ifdef USE_PLL 
  #ifdef CFG_USE_48_MHZ_CORE
    #define FOSC                (12000000)  /**< Master Oscillator Freq.*/
    #define PLL_MUL             (24)        /**< PLL Multiplier (must change this if you use PLL). */
    #define CCLK                (FOSC * PLL_MUL) /**< CPU Clock Freq. */
        // Peripheral Bus Speed Divider
    #define PBSD                6 
    #define PCLK                (CCLK / PBSD) /**< Peripheral Bus Clock Freq.*/
    #define PCLK_ADC            PCLK / 4 /**< Might change in future?*/
    #define PCLK_UART           PCLK / 4 /**< This might change based on ultimate clocking scenario. */
  #else
    #define FOSC                (12000000)  /**< Master Oscillator Freq.*/
    #define PLL_MUL             (24)        /**< PLL Multiplier (must change this if you use PLL). */
    #define CCLK                (FOSC * PLL_MUL) /**< CPU Clock Freq. */
        // Peripheral Bus Speed Divider
    #define PBSD                24 
    #define PCLK                (CCLK / PBSD) /**< Peripheral Bus Clock Freq.*/
    #define PCLK_ADC            PCLK 
    #define PCLK_UART           PCLK 
  #endif

#else
// No PLL
#define FOSC                (12000000)  /**< Master Oscillator Freq.*/
#define PLL_MUL             (1)         /**< PLL Multiplier (must change this if you use PLL).*/
#define CCLK                (FOSC * PLL_MUL) /**< CPU Clock Freq.*/
// Peripheral Bus Speed Divider
// This is setup using the APBDIV register in seom init code
#define PBSD                1           /**< MUST BE 1, 2, or 4.*/
#define PCLK                (CCLK / PBSD) /**< Peripheral Bus Clock Freq.*/
#define PCLK_ADC            PCLK / 4 /**< Might change in future?*/
#define PCLK_UART           PCLK  /**< This might change based on ultimate clocking scenario.*/
#endif


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
 * @name Defines for Port 0
 */
/*@{*/  
#define CHIMU_DIO1      0x00000010 /**< P0.4 */
#define CHIMU_DIO2      0x00000020 /**< P0.5 */
#define RS485_RX_EN     0x00200000 /**< P0.21 */
#define RS485_TX_EN     0x00400000 /**< P0.22 */
#define ANA0            0x00800000 /**< P0.23 */
#define ANA1            0x01000000 /**< P0.24 */
#define ANA2            0x02000000 /**< P0.25 */
#define DAC1            0x04000000 /**< P0.26 */
/*@}*/
/**
 * @name Defines for Port 1
 */
/*@{*/ 
#define LED_BLUE       0x00000001 /**< P1.0 */
#define LED_RED        0x00000002 /**< P1.1 */
#define LED_GREEN      0x00000010 /**< P1.4 */
#define BARO_EOC       0x00000100 /**< P1.8 */
#define BARO_XCLR      0x00000200 /**< P1.9 */
#define GPS_PWR_SHTDN  0x00000400 /**< P1.10 */
#define GPS_LNA_SHTDN  0x00004000 /**< P1.14 */
#define CAP_10         0x00040000 /**< P1.18 */
#define CAP_11         0x00080000 /**< P1.19 */
#define CAP_00         0x04000000 /**< P1.26 */
#define CAP_01         0x08000000 /**< P1.27 */
#define SD_CLK_CLK     0x00100000 /**< P1.20 */
#define SD_CS0_DAT3    0x00200000 /**< P1.21 */
#define SD_MISO_D0     0x00800000 /**< P1.23 */
#define SD_MOSI_CMD    0x01000000 /**< P1.24 */
#define ANA4           0x40000000 /**< P1.30 */
#define ANA5           0x80000000 /**< P1.31 */
/*@}*/
/**
 * @name Defines for Port 2
 */
/*@{*/ 
#define PWM0            0x00000001 /**< P2.0 */
#define PWM1            0x00000002 /**< P2.1 */
#define PWM2            0x00000004 /**< P2.2 */
#define PWM3            0x00000008 /**< P2.3 */
#define PWM4            0x00000010 /**< P2.4 */
#define PWM5            0x00000020 /**< P2.5 */
#define GPIO0           0x00001000 /**< P2.12 */
#define GPIO1           0x00002000 /**< P2.13 */
/*@}*/
/**
 * @name Defines for Port 3
 */
/*@{*/ 
#define CHIMU_RESET     0x04000000 /**< P3.26 */
/*@}*/


#define RS485_RXENABLE FIO0CLR |= RS485_RX_EN;  FIO0CLR |= RS485_TX_EN;
#define RS485_TXENABLE FIO0SET |= RS485_TX_EN;  FIO0SET |= RS485_RX_EN;
#define RS485_DISABLE  FIO0SET |= RS485_RX_EN;  FIO0CLR |= RS485_TX_EN;



// Timer 0, 1 and 2 values
#ifdef CFG_USE_48_MHZ_CORE
  #define _1SEC     (unsigned long) (PCLK / 4 * 1)  /**< at 48 Mhz, timers are on a CCLK/4 setting */
  #define _100MSEC  (unsigned long) (PCLK / 4 * 0.01) 
  #define _1MSEC    (unsigned long) (PCLK / 4 * 0.001)
#else
  #define _1SEC     (unsigned long) (PCLK  * 1)  /**< at 12 Mhz, timers are on a CCLK setting */
  #define _100MSEC  (unsigned long) (PCLK  * 0.01) 
  #define _1MSEC    (unsigned long) (PCLK  * 0.001)
#endif    
        
/*Timer 0*/
#define TIMER0_1SEC     _1SEC     /**< Timer 0 setup*/
#define TIMER0_100MSEC  _100MSEC  /**< Timer 0 setup*/
#define TIMER0_1MSEC    _1MSEC    /**< Timer 0 setup*/


/* Timer 1 values*/
//Change this if Timer 1 is selected to be other than /4 of PCLK
#define TIMER1_1SEC    _1SEC /**< Timer 1 setup*/
#define TIMER1_10MSEC  _100MSEC /**< Timer 1 setup*/
#define TIMER1_1MSEC   _1MSEC /**< Timer 1 setup*/

/*Timer 2 used as main system timer*/
#define TIMER2_1SEC     _1SEC /**< Timer 2 setup*/
#define TIMER2_100MSEC  _100MSEC /**< Timer 2 setup*/
#define TIMER2_1MSEC    _1MSEC /**< Timer 2 setup*/

//ADC SECTION
//ADC Extension (allows extra two GPIO's to be used as ADC inputs)
#define ADC_EXTEND 1       /**< 1 = use 2 GPIO's as extra ADC's, 0 = deny.*/
#define NUM_ADC_CHANNELS 5 /**< Number of ADC channels used for Monkey*/
#define POWER_NODE_SCALING 0.008776; /**< Scaling updated for Power Node 2.0 = 3.3/4096*1/.0918 */
float gBatteryVolts; /**< Easy access to battery voltage if available.*/

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


/**
* A structure representing core GPS data (47 bytes)
*/
#pragma pack(1) //Force bytewise packing
typedef struct {
        unsigned char       satstracked;      /**< Satellites in solution*/
        unsigned char       quality;    /**< Accuracy*/
        unsigned char       flags;      /**< GPS flags indicating track/fix status*/
        unsigned short int  UTC_year;   /**< UTC year*/
        unsigned char       UTC_month;  /**< UTC month*/
        unsigned char       UTC_day;    /**< UTC day*/
        unsigned char       UTC_hr;     /**< UTC hr*/
        unsigned char       UTC_min;    /**< UTC min*/
        unsigned char       UTC_sec;    /**< UTC sec*/
        unsigned char       time_valid; /**< UTC time valid*/
        unsigned int        TOW;        /**< GPS TOW (msec)*/
        float         latitude;         /**< Latitude (dec degree format)*/
        float         longitude;        /**< Longitude (dec degree format)*/
        float         altitude;         /**< Altitude (meters above MSL)*/
        float         gndspeed;         /**< Ground speed (m/s)*/
        float         heading;          /**< GPS heading (degrees)*/
        float velN;                     /**< North velocity (m/s)*/
        float velE;                     /**< East velocity (m/s)*/
        float velD;                     /**< Down velocity (m/s)*/
} GPS_Data;

#define UBX_MAX_NUM_CHANNELS 16 //Max number of SV entries we'll allow for reporting / use (info only)

/**
* A structure representing satellite vehicle specific GPS data (8 bytes)
*/
typedef struct {
      unsigned char     chn;    /**< Internal channel*/
      unsigned char     svid;   /**< Sat ID */
      unsigned char     flags;  /**< flags */
      unsigned char     quality;/**< Lock quality (3 = track, 6/7 = good lock*/
      unsigned char     cno;    /**< C/No */
      unsigned char     elev;   /**< Elevation */
      unsigned short    azim;   /**< Azimuth */
      //unsigned long prRes;  //Get rid of this for now - until we need pseudorange residuals, it just takes up space in TLM
} GPS_SV_Data; 


/**
* A structure that holds overall satellite specifics (136 bytes)
*/
typedef struct {
      unsigned int iTOW;        /**< ms*/
      unsigned char numCh;      /**< number of channels */
      unsigned char globalFlags; /**< global flags */
      unsigned short int res2;  /**< reserved */
      GPS_SV_Data svinfo[UBX_MAX_NUM_CHANNELS]; /**< SV data */
} GPS_Sat_Info; 


/**
* A structure representing GPS status\n
* Use for determining message status (14 bytes)
*/
typedef struct {
        unsigned char msg_ready_LLH;    /**< LLA message ready*/
        unsigned char msg_ready_ECEF;   /**< ECEF message ready*/
        unsigned char msg_ready_VELNED; /**< VEL_NED message ready*/
        unsigned char msg_ready_SVInfo; /**< SV Info message ready*/
        unsigned char msg_ready_UTC; /**< UTC message is ready*/
        unsigned char lock_OK;          /**< BIT flag indicating we have good lock (not fully tested)*/
        unsigned char comm_OK;          /**< BIT flag indicating we have good comm (not fully tested)*/
        unsigned char init_OK;          /**< BIT flag indicating we have good init of GPS (not fully tested)*/
        /**
        * @name Unsupported
        */
        /*@{*/
        unsigned short noisePerMS; /**< Noise per msec*/
        unsigned short agcCnt;    /**< AGC count*/
        unsigned char antStatus;  /**< antenna status*/
        unsigned char antPower; /**< antenna Power*/
        unsigned char jamInd;   /**< Jamming indicator*/
        /*@}*/

} GPS_Status;

/**
* A structure representing inertial navigation sensor/system (beta!) 
*/
#pragma pack(1) //Force byte-wise packing
typedef struct {
        /**
        * @name Data variables
        */
        /*@{*/   
        vec3d   acc_bias;           /**< bias computed in filter */
        float   rss_gs;             /**< rss g's  */
        float   motion_thresh;      /**< threshold from user (g's) */
        
        vec3d   pos;                /**< x,y,z in NED coordinates, units of meters, of INS position */
        vec3d   vel;                /**< x,y,z vel in NED coordinates, units of m/s, of INS position */

        float   rss_dist;           /**< distance from filter start position */
    
        float   latitude;           /**< equivalent filter latitude */
        float   longitude;          /**< equivalent filter longitude */
        float   altitude;           /**< equivalent filter altitude */

        vec3d   init_lla;           /**< Starting lat/long/alt for filter basis */
        /*@}*/
        /**
        * @name Status variables
        */
        /*@{*/
        unsigned char bForceZeroPosVel;       /**< Force the filter velocity and position updates to be zero (know you are static) */
        unsigned char bFilter_Valid;      /**< Is the filter solution alright to use? */
        unsigned char bFilterGPSDataInit; /**< Has the filter been initialized? */
        unsigned char bZeroVelocityTrue;  /**< Are we in a zero velocity state? */
        /*@}*/
        /**
        * @name Helper variables
        */
        /*@{*/        
        float meters_per_deg_long;        /**< May be settable in the future by user */
        float meters_per_deg_lat;         /**< May be settable in the future by user */
        /*@}*/
} INS_Data; 


/**
* A structure containing additional / augmented inertial navigation sensor/system (beta!) 
*/
#pragma pack(1) //Force byte-wise packing
typedef struct {
        /**
        * @name Data variables
        */
        /*@{*/   
        vec3d   heave;              /**< heave (vertical) accel, velocity, position */
        unsigned int  msgs_sent;           /**< index that increments upon every transmit of this message to assist in locating dropped messages */
        float   spare1;             /**< spare  */
        float   spare2;             /**< spare */
        float   spare3;             /**< spare */
        float   spare4;             /**< spare */
        float   spare5;             /**< spare */
        /*@}*/
} INS_Augment_Data; 

/**
* A structure representing ADC data (15 bytes)
*/
typedef struct 
{
  unsigned short int raw; /**< Raw ADC counts */
  float scaled; /**< Scaled data */
  float gain;  /**< gain for scaling*/
  float offset; /**< offset for scaling */
  unsigned char valid;  /**< ADC read valid */
} ADC_Data; 

/**
* A structure representing pressure sensor\n
* for reporting only
*/
#pragma pack(1)
typedef struct
{
	unsigned char num_samples; /**< Running number of samples successfully received from sensor */
  short temperature;          /**< temperature in 0.1deg C */
  long pressure;              /**< pressure in Pa */
  float altitude;             /**< altitude computed from pressure (meters) above sea level */
} Pressure_Status_Struct; 

/**
* A structure representing Monkey specific status\n
* @warning Currently in work, use at own risk!!!!!
*/
#pragma pack(1)
typedef struct
{
  unsigned char   Mode; /**< Monkey system mode */
  unsigned char   ComMode; /**< Communication mode (running or not) */
  unsigned short  BIT;  /**< Built in test status */
  unsigned char   DeviceID; /**< Device ID */
  unsigned short  DeviceSerialNumber; /**< Serial number (use SD card to overwrite) */
  unsigned short  CPU_daHz; /**< units of decaHertz (= Hz / 10).  To get Hz, multiply this value by 10.*/
  unsigned char   SW_Major; /**< Software major identifier */
  unsigned char   SW_Minor; /**< Software minor identifier */
  unsigned char   NavMode; /**< Monkey Navigation Mode */
  unsigned char   status; /**< Monkey Status flag */
  unsigned short  load; /**< Estimate of main loop load 0% = 0, 100% = 1000*/
  unsigned short  vbat; /**< Battery voltage in millivolts (note that gBatteryVolts is a float and this is derived from that variable) */
  unsigned short  batt_remain; /**< For future use.  Currently based on <6.5V = 0 remain, >9V = 100% left.*/
  unsigned short  packet_drop; /**< Packets dropped or incorrect via the uplink on the main com port*/
  unsigned short  current; /**< Placeholder for current draw.  Units TBD*/
  unsigned char   lost_link; /**< Indicates if ground link has been lost.  i.e. no heartbeat or other messages received */
  unsigned short  time_since_heartbeat; /**< Time (seconds) since a heartbeat or other message was received by the system */

  unsigned char   Wpt_LastID;  /**< beta*/
  unsigned char   Wpt_NextID;   /**< beta*/
  unsigned char   Wpt_ETA;   /**< beta*/
  unsigned char   Wpt_Status;  /**< beta*/
  unsigned char   Wpt_NumWpts;   /**< beta*/

} Monkey_Status_Struct; 


/**
* A structure identical to the RTC time used for data logging\n
* This is updated automatically in systick.c and performs a one time\n
* GPS sync when / if a valid GPS UTC is provided.\n
* 
* This automatic updating can be prevented thru use of the gMonkeyTime_AutoUpdate\n
* flag.  If you prevent updating, this can be used to force a date/time stamp in log files.\n
*
*/
typedef struct {
    unsigned char UTC_sec;     /**<  Second value - [0,59] */
    unsigned char UTC_min;     /**<  Minute value - [0,59] */
    unsigned char UTC_hour;    /**<  Hour value - [0,23] */
    unsigned char UTC_mday;    /**<  Day of the month value - [1,31] */
    unsigned char UTC_wday;    /**<  Day of week value - [0,6] */
    unsigned short UTC_yday;   /**<  Day of year value - [1,365] */
    unsigned char UTC_month;   /**<  Month value - [1,12] */
    unsigned short UTC_year;   /**<  Year value - [0,4095] */  
} MonkeyUTCTime_Struct;  

//Global variables
//Com Output specific
unsigned char gCHIMU_BIT;/**< CHIMU specific*/
unsigned char tempstr[UART_BUFFERSIZE];/**< string buffer for general use.*/

//System
Monkey_Status_Struct gMonkey; /**< Monkey status information.*/
unsigned char gLostLinkTimeout; /**<Set this value to the number of seconds required without uplink com before the system flags a lost link*/

//UART Buffers
volatile unsigned char UART_rx_buffer[NUMPORTS][UART_BUFFERSIZE]; /**< UART buffers.*/
volatile unsigned char UART_rx_extract_idx[NUMPORTS]/**< UART buffers */, UART_rx_insert_idx[NUMPORTS];/**< UART buffers.*/

//GPS
GPS_Data gGPS;              /**< Holds core GPS data (position, velocity, status).*/
GPS_Status gGPS_Status;     /**< Holds GPS status information.*/
GPS_Sat_Info gGPS_SVinfo;   /**< Satellite info (can be disabled).*/
long gUTC_Nanoseconds;       /**< Special UTC field of nanoseconds from U-blox.  Most don't need this, so not part of gGPS */
float gMetersPerDegLong;          /**< Number of meters per degree of longitude, varies with latitude.  Verify GPS lock has been achieved before using. */

//Attitude and CHIMU data
CHIMU_sensor_data gSensor;    /**< AHRS sensor data (accel, rates, mags).*/
CHIMU_attitude_data gAttitude;/**< AHRS attitude data (Eulers, quats, rates).*/
CHIMU_attitude_data gAttRates;/**< AHRS attitude rates only.*/
euler_angles gGyroBias;       /**< AHRS Gyro biases */
unsigned int gCHIMU_TimeCnts; /**< AHRS Time stamp */
unsigned char gAttitudeReceived; /**< AHRS attitude message has been received.  Not a measure of healthy attitude. */
unsigned int gCHIMUMissedMessages; /**< AHRS Missed messages across SPI bus */
short int gCHIMU_Mags[6]; /**< AHRS Magnetometer data (special).*/
unsigned int gOK_TO_SEND; /**< Will be removed later, but currently flags when it is OK to pass CHIMU data via Monkey TLM.*/

//INS
INS_Data gINS; /**< Inertial navigation data if enabled / allowed.*/
vec3d gInertialAccel; /**< Acceleration with gravity removed. */
INS_Augment_Data gINS_Augment; /**<Augmented data for INS message 0x61 */

//CHIMU BITs and such
unsigned char gCHIMU_Mode; /**<CHIMU specific */
unsigned char gCalStatus;/**< CHIMU specific*/
unsigned char gConfigInfo;/**< CHIMU specific*/
unsigned int bC0_SPI_En; /**< CHIMU specific*/
unsigned int bC1_HWCentrip_En;/**< CHIMU specific*/
unsigned int bC2_TempCal_En;/**< CHIMU specific*/
unsigned int bC3_RateOut_En;/**< CHIMU specific*/
unsigned int bC4_TBD;/**< CHIMU specific*/
unsigned int bC5_Quat_Est;/**< CHIMU specific*/
unsigned int bC6_SWCentrip_En;/**< CHIMU specific*/
unsigned int bC7_AllowHW_Override;/**< CHIMU specific*/
//
unsigned int gCHIMU_SW_Exclaim;/**< CHIMU specific*/
unsigned int gCHIMU_SW_Major;/**< CHIMU specific*/
unsigned int gCHIMU_SW_Minor;/**< CHIMU specific*/
unsigned int gCHIMU_SW_SerialNumber;/**< CHIMU specific*/

//ADC global
ADC_Data gADC[5]; /**< ADC array, 0 thru 4.*/

//GPIO-See UserFunctions.h for GPIO library calls

//Pressure sensor
Pressure_Status_Struct gPressure; /**< Pressure sensor data.*/

//CPU Use
unsigned long gThruputCnts; /**< Thruput counts in primary loop.*/
unsigned long gThruputHz;/**< Equivalent Thruput rate (Hz).*/

// logger boolean
unsigned char bLoggerOK;  /**< SD card file system is o.k. and logging is allowed. */
unsigned char bSDCardPresent; /**< SD card detected (only occurs on power up / sd_init()). */
//

//Special
unsigned char gUart3TX_Running; /**< Special.  Ignore for normal use.*/

//Timekeeping
unsigned int gMonkeyTime_runtime; /**<milliseconds since boot.*/
MonkeyUTCTime_Struct gMonkeyTime_UTC; /**<UTC created internally.*/
unsigned char gMonkeyTime_AutoUpdate; /**<Typically set to true.  If false, UTC will not update, but runtime will.*/
unsigned char gGPSTimeSyncTrue; /**<True if we have synced with the time pulse.*/

//Telemetry
unsigned int gTLM_Frame_Time_msec; /**< This is the frame time (msec) that Monkey telemetry will repeat over.  Standard is 10Hz == 100 */
unsigned int gTLM_QuietForRX_Time_msec; /**< This is the time within the frame that should be clear of transmits to allow uplink via RF */
float gTLM_Xmit_Delay_ratio; /**< This is a ratio (0 to 1.0) applied to every output message.  It results in a delay of time equal to the ratio * message transmit time to allow the modem buffer to transmit.  Poor man's handshake to play nice with XBee's. */
unsigned int gTLM_HW_Handshake; /**< Not supported currently */
unsigned char gTLM_UserTextMessage[128]; /**< For user text messages via 0xB1 */
unsigned char gTLM_UserTextMessageLen; /**< For user text messages via 0xB1 */
unsigned char gTLM_TextMessageWarningLevel;/**< For user text messages via 0xB1 */
float gTLM_UserFloat[10]; /**< Array of floats for user to fill for debug talk across modules as needed.  Also used in telemetry output. */
unsigned char gTLM_OutputGPSImmediately; /**< If user sets to TRUE, GPS messages are output immediately upon receipt.  If FALSE, GPS message output is per the defined rate in the telemetry stream */

//LED Heartbeat
int gLED_Heartbeat; /**< Blue LED heartbeat.  Active = 1, inactive = 0.  Used in system.c */

//CHIMU
int gCHIMU_Endian_Is_Small; /**< Endian check for CHIMU AHRS (-J is small).*/
int gCHIMU_Auto_GPS_Vel_Enabled; /**< Allows command to AHRS for centripetal updates automatically from GPS.*/
int gCHIMU_Heading_Substitute_Enabled; /**<Allows magnetometer replacment heading to be sent from GPS to AHRS.*/
vec3d gCHIMU_MagRefVector; /**<Mag reference vector that CHIMU has stored. */
float gCHIMU_Declination; /**<Declination value that CHIMU has stored. */
float gCHIMU_Inclination; /**<Inclination value that CHIMU has stored. */
//END Group
/* @} */



#endif

