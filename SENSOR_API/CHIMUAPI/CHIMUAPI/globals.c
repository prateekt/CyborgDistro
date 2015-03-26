#include "globals.h"

//Global variables
//Com Output specific
unsigned char gCHIMU_BIT;/**< CHIMU specific*/


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


//CHIMU
int gCHIMU_Endian_Is_Small; /**< Endian check for CHIMU AHRS (-J is small).*/
int gCHIMU_Auto_GPS_Vel_Enabled; /**< Allows command to AHRS for centripetal updates automatically from GPS.*/
int gCHIMU_Heading_Substitute_Enabled; /**<Allows magnetometer replacment heading to be sent from GPS to AHRS.*/
vec3d gCHIMU_MagRefVector; /**<Mag reference vector that CHIMU has stored. */
float gCHIMU_Declination; /**<Declination value that CHIMU has stored. */
float gCHIMU_Inclination; /**<Inclination value that CHIMU has stored. */

