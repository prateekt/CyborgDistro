/**************************************************************************/
/*! 
    @file     CHIMUParserClass.c
    Contains CHIMU parsing code and specialized functions for unit
    @author   M. Ryan (ryanmechatronics.com)
    @date     2 July 2011
    @version  2.0

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, Ryan Mechatronics LLC
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
#include "globals.h"
#include "CHIMUParserClass.h"
#include "string.h"
#include "crc.h"
//#include "CommOutput.h"
#include "endian_functions.h"
#include "util.h"
#include "math.h"
//#include "projectconfig.h"
//#include "spi.h"
#include <iostream>
#include <stdio.h>

using namespace std;

#undef  NULL
#define NULL  (void *)0

int pCHIMU_MagData_Read = FALSE; 
int pCHIMU_MagData_Read_Attempts = 0;


/** @defgroup CHIMU CHIMU AHRS Handling
  * Commands and functions that allow parsing of sensor and attitude data from the CHIMU modules, as well as commands to the CHIMU.
  * @{
  */
  
  
/** \name CHIMU AHRS Parsing and Command Functions */
/* @{ */
  

/**
 * CHIMU_Init
 *
 * Initializes the CHIMU data structure for receiving CHIMU messages
 * 
 * @param  *pstData this is a CHIMU_PARSER_DATA structure
 * @return void
 */
void
CHIMU_Init(CHIMU_PARSER_DATA   *pstData){
  unsigned char i;
        pstData->m_State = CHIMU_STATE_MACHINE_START;
        pstData->m_Checksum = 0x00;
        pstData->m_ReceivedChecksum = 0x00;
        pstData->m_Index = 0;
        pstData->m_PayloadIndex = 0;

        //Sensor data holder
        pstData->m_sensor.cputemp=0.0;
        for (i=0;i<3;i++){
          pstData->m_sensor.acc[i]=0.0;
          pstData->m_sensor.rate[i]=0.0;
          pstData->m_sensor.mag[i]=0.0;
        }
        pstData->m_sensor.spare1=0.0;
        pstData->m_CHIMU_TimeCnts = 0;
        //Attitude data
        pstData->m_attitude.euler.phi = 0.0;
        pstData->m_attitude.euler.theta = 0.0;
        pstData->m_attitude.euler.psi = 0.0;
        pstData->m_attitude.q.s = 1.0;
        pstData->m_attitude.q.v.x = 0.0;
        pstData->m_attitude.q.v.y = 0.0;
        pstData->m_attitude.q.v.z = 0.0;
        //Attitude rate data
        pstData->m_attrates.euler.phi = 0.0;
        pstData->m_attrates.euler.theta = 0.0;
        pstData->m_attrates.euler.psi = 0.0;
        pstData->m_attrates.q.s = 1.0;
        pstData->m_attrates.q.v.x = 0.0;
        pstData->m_attrates.q.v.y = 0.0;
        pstData->m_attrates.q.v.z = 0.0;

 	for (i=0; i<CHIMU_RX_BUFFERSIZE; i++) {
		pstData->m_Payload[i]= 0x00;
                pstData->m_FullMessage[i]= 0x00;
		}
	pstData->m_MsgLen = 0;
	pstData->m_MsgID = 0;
	pstData->m_TempDeviceID =0;
  pstData->m_DeviceID = 0x01; //look at this later

  gCHIMU_MagRefVector.x=0.0; gCHIMU_MagRefVector.y=0.0; gCHIMU_MagRefVector.z=0.0;
  gCHIMU_Declination = 0.0;
  gCHIMU_Inclination = 0.0;
  pCHIMU_MagData_Read = FALSE; //Until we actually do...
  pCHIMU_MagData_Read_Attempts = 0;

  gAttitudeReceived = FALSE;
  gCHIMUMissedMessages=0;
}

/**
 * CHIMU_Parse
 *
 * Parse messages from CHIMU, returns TRUE if new data.
 * 
 * @param  btData input byte from data stream (SPI or UART character for example)
 * @param  bInputType reserved for future use, set to 0 currently
 * @param  *pstData this is the resulting data in the CHIMU_PARSER_DATA structure 
 * @return bUpdate TRUE if successful parse of a CHIMU message
 */
unsigned char
CHIMU_Parse(
    unsigned char btData,           /* input byte stream buffer */
    unsigned char bInputType,       /* for future use if special builds of CHIMU data are performed */
    CHIMU_PARSER_DATA   *pstData) {  /* resulting data           */

    long int       i;
    char           bUpdate = FALSE;

    switch (pstData->m_State) {
            case CHIMU_STATE_MACHINE_START:  // Waiting for start character 0xAE
                      if(btData == 0xAE)
                       {
                          pstData->m_State = CHIMU_STATE_MACHINE_HEADER2;
                          pstData->m_Index = 0;
                          pstData->m_FullMessage[pstData->m_Index++] = btData;
                       } else {
                          ;;
                       }
                      bUpdate = FALSE;
                      break;
            case CHIMU_STATE_MACHINE_HEADER2:  // Waiting for second header character 0xAE
                      if(btData == 0xAE) 
                      { 
                          pstData->m_State = CHIMU_STATE_MACHINE_LEN;
                          pstData->m_FullMessage[pstData->m_Index++]=btData;
                      } else {
                          pstData->m_State = CHIMU_STATE_MACHINE_START;
                      } //Fail to see header.  Restart.
                      break;
            case CHIMU_STATE_MACHINE_LEN:  // Get chars to read
              if( btData <= CHIMU_RX_BUFFERSIZE) 
                      { 
                        pstData->m_MsgLen = btData  ; // It might be invalid, but we do a check on buffer size
                        pstData->m_FullMessage[pstData->m_Index++]=btData;
                        pstData->m_State = CHIMU_STATE_MACHINE_DEVICE;
                      } else {
                        pstData->m_State = CHIMU_STATE_MACHINE_START; //Length byte exceeds buffer.  Signal a fail and restart
                        //BuiltInTest(BIT_COM_UART_RECEIPTFAIL, BIT_FAIL);
                      }
              break;
            case CHIMU_STATE_MACHINE_DEVICE:  // Get device.  If not us, ignore and move on.  Allows common com with Monkey / Chipmunk
              if( (btData == pstData->m_DeviceID) || (btData == 0xAA)) 
                      { //0xAA is global message
                        pstData->m_TempDeviceID = btData;
                        pstData->m_FullMessage[pstData->m_Index++]=btData;
                        pstData->m_State = CHIMU_STATE_MACHINE_ID;
                      } else {
                        pstData->m_State = CHIMU_STATE_MACHINE_START;
                      } //Fail to see correct device ID.  Restart.
              break;
            case CHIMU_STATE_MACHINE_ID:  // Get ID
                      pstData->m_MsgID = btData; // might be invalid, chgeck it out here:
                      if ( (pstData->m_MsgID<CHIMU_COM_ID_LOW) || (pstData->m_MsgID>CHIMU_COM_ID_HIGH)) 
                      { 
                        pstData->m_State = CHIMU_STATE_MACHINE_START;
                        //BuiltInTest(BIT_COM_UART_RECEIPTFAIL, BIT_FAIL);
                      } else {
                        pstData->m_FullMessage[pstData->m_Index++]=btData;
                        pstData->m_PayloadIndex = 0;
                        pstData->m_State = CHIMU_STATE_MACHINE_PAYLOAD; //Finally....Good to go...
                      }
              break;
            case CHIMU_STATE_MACHINE_PAYLOAD:  // Waiting for number of bytes in payload
                      pstData->m_Payload[pstData->m_PayloadIndex++]= btData;
                      pstData->m_FullMessage[pstData->m_Index++]=btData;
                      if ((pstData->m_Index) >= (pstData->m_MsgLen + 5)) //Now we have the payload.  Verify XSUM and then parse it next
                      {
                        pstData->m_Checksum = (unsigned char) ((UpdateCRC(0xFFFFFFFF , pstData->m_FullMessage , (unsigned long) (pstData->m_MsgLen)+5)) & 0xFF);                                               
                        pstData->m_State = CHIMU_STATE_MACHINE_XSUM;
                      } else {
                        return FALSE;
                      }
                      break;
            case CHIMU_STATE_MACHINE_XSUM:  // Verify
                      pstData->m_ReceivedChecksum = btData;
                      pstData->m_FullMessage[pstData->m_Index++]=btData;
                      if (pstData->m_Checksum!=pstData->m_ReceivedChecksum) 
                      {
                        bUpdate = FALSE;
                        //BuiltInTest(BIT_COM_UART_RECEIPTFAIL, BIT_FAIL);
                      } else {
                        //Xsum passed, go parse it.
                        // We have pstData->m_MsgID to parse off of, pstData->m_pstData->m_Payload as the data.
                        bUpdate = CHIMU_ProcessMessage(&pstData->m_MsgID, pstData->m_Payload, pstData);
                      }
                      pstData->m_State = CHIMU_STATE_MACHINE_START;
                      break;
                default:
                      pstData->m_State = CHIMU_STATE_MACHINE_START;
              } /* End of SWITCH */
  return (bUpdate);
}


/**
 * CHIMU_ProcessMessage
 *
 * Decodes appropriate data from a valid CHIMU message for use in Monkey 
 * 
 * @param  *pMsgID message ID to decode
 * @param  *pPayloadData actual payload of the message (no header, checksum, etc.)
 * @param  *pstData this is the resulting data in the CHIMU_PARSER_DATA structure used by Monkey
 * @return truefalse TRUE if successful parse of a CHIMU message
 */
unsigned char CHIMU_ProcessMessage(unsigned char *pMsgID, char *pPayloadData, CHIMU_PARSER_DATA *pstData)
{
    //Msgs from CHIMU are off limits (i.e.any CHIMU messages sent up the uplink should go to 
    //CHIMU).  

    //Any CHIMU messages coming from the ground should be ignored, as that byte stream goes up to CHIMU
    // by itself.  However, here we should decode CHIMU messages being received and 
    //  a) pass them down to ground
    //  b) grab the data from the CHIMU for our own needs / purposes
    int index =0, i=0;
    short int stemp = 0; 
    float sanity_check=0.0;
    float ftemp = 0.0;
    #ifdef CFG_CHIMU_AUTOSELECT
      if(gCHIMU_Endian_Is_Small == -1)
        {
          unsigned char ping[7] = {0xAE, 0xAE, 0x01, 0xAA, 0x00, 0x00, 0xE6 };
          //We need a ping to tell what CHIMU is there. Form it and send them up the SPI bus if we haven't done so already.
          SSP1_Write(ping, 7); 
          if((pstData->m_MsgID) != CHIMU_Msg_0_Ping) return (FALSE); //Don't populate with bad data if we don't know what kind of CHIMU is there yet!    
        }
    #endif

//	printf("%x\n",pstData->m_MsgID);

	switch (pstData->m_MsgID){
		case CHIMU_Msg_0_Ping:
                  index = 0;
                  gCHIMU_SW_Exclaim = pPayloadData[index]; index++;
                  gCHIMU_SW_Major = pPayloadData[index]; index++;
                  gCHIMU_SW_Minor = pPayloadData[index]; index++;
                  gCHIMU_SW_SerialNumber = (pPayloadData[index]<<8) & (0x0000FF00); index++;
                  gCHIMU_SW_SerialNumber += pPayloadData[index]; index++;
                  #ifdef CFG_CHIMU_AUTOSELECT
                  if(gCHIMU_Endian_Is_Small == -1)
                  {
                    if (gCHIMU_SW_Major<2) //CHIMU J software is 2.0 or greater
                      { 
                        gCHIMU_Endian_Is_Small = FALSE;
                      }else{
                        gCHIMU_Endian_Is_Small = TRUE;          
                      }      
                  }
                  #endif
                  return TRUE;
                  break;
		case CHIMU_Msg_1_IMU_Raw:
                  break;
		case CHIMU_Msg_2_IMU_FP:
                  index = 0;
                  memmove (&pstData->m_sensor.cputemp, &pPayloadData[index], sizeof(pstData->m_sensor.cputemp));index += (sizeof(pstData->m_sensor.cputemp));
                  pstData->m_sensor.cputemp = FloatSwap(pstData->m_sensor.cputemp);
                  memmove (&pstData->m_sensor.acc[0], &pPayloadData[index], sizeof(pstData->m_sensor.acc));index += (sizeof(pstData->m_sensor.acc));
                  pstData->m_sensor.acc[0] = FloatSwap(pstData->m_sensor.acc[0]);
                  pstData->m_sensor.acc[1] = FloatSwap(pstData->m_sensor.acc[1]);
                  pstData->m_sensor.acc[2] = FloatSwap(pstData->m_sensor.acc[2]);
                  memmove (&pstData->m_sensor.rate[0], &pPayloadData[index], sizeof(pstData->m_sensor.rate));index += (sizeof(pstData->m_sensor.rate));
                  pstData->m_sensor.rate[0] = FloatSwap(pstData->m_sensor.rate[0]);
                  pstData->m_sensor.rate[1] = FloatSwap(pstData->m_sensor.rate[1]);
                  pstData->m_sensor.rate[2] = FloatSwap(pstData->m_sensor.rate[2]); 
                  memmove (&pstData->m_sensor.mag[0], &pPayloadData[index], sizeof(pstData->m_sensor.mag));index += (sizeof(pstData->m_sensor.mag));
                  pstData->m_sensor.mag[0] = FloatSwap(pstData->m_sensor.mag[0]);
                  pstData->m_sensor.mag[1] = FloatSwap(pstData->m_sensor.mag[1]);
                  pstData->m_sensor.mag[2] = FloatSwap(pstData->m_sensor.mag[2]);
                  memmove (&pstData->m_sensor.spare1, &pPayloadData[index], sizeof(pstData->m_sensor.spare1));index += (sizeof(pstData->m_sensor.spare1));
                  pstData->m_sensor.spare1 = FloatSwap(pstData->m_sensor.spare1);
                  memmove (&pstData->m_CHIMU_TimeCnts, &pPayloadData[index], sizeof(pstData->m_CHIMU_TimeCnts));index += (sizeof(pstData->m_CHIMU_TimeCnts));    
                  //Before moving data to the global, make sure there were no transmission errors via SPI
                  index = 0;
                  index += ((pstData->m_sensor.cputemp >200) || (pstData->m_sensor.cputemp <-100));
                  //Max acceleration: 10 g's (98 m/s^2)
                  index += ((pstData->m_sensor.acc[0] >98) || (pstData->m_sensor.acc[0] <-98));
                  index += ((pstData->m_sensor.acc[1] >98) || (pstData->m_sensor.acc[1] <-98));
                  index += ((pstData->m_sensor.acc[2] >98) || (pstData->m_sensor.acc[2] <-98));
                  //Max rate:  2500 deg/s (43 rad/s)
                  index += ((pstData->m_sensor.rate[0] >43) || (pstData->m_sensor.rate[0] <-43));
                  index += ((pstData->m_sensor.rate[1] >43) || (pstData->m_sensor.rate[1] <-43));
                  index += ((pstData->m_sensor.rate[2] >43) || (pstData->m_sensor.rate[2] <-43));
                  //Max mag data:  100 gauss 
                  index += ((pstData->m_sensor.mag[0] >100) || (pstData->m_sensor.mag[0] <-100));
                  index += ((pstData->m_sensor.mag[1] >100) || (pstData->m_sensor.mag[1] <-100));
                  index += ((pstData->m_sensor.mag[2] >100) || (pstData->m_sensor.mag[2] <-100));
                  if (index >0)
                    {
                      gCHIMUMissedMessages++;
                      return FALSE;
                    }


                  gSensor = pstData->m_sensor;
                  gCHIMU_TimeCnts = pstData->m_CHIMU_TimeCnts; 
                  return TRUE;
                  break;
		case CHIMU_Msg_3_IMU_Attitude:
                  //Attitude message data from CHIMU
                  // Includes attitude and rates only, along with configuration bits
                  // All you need for control
                  index = 0;
                  memmove (&pstData->m_attitude.euler, &pPayloadData[index], sizeof(pstData->m_attitude.euler));index += sizeof(pstData->m_attitude.euler);
                  pstData->m_attitude.euler.phi = FloatSwap(pstData->m_attitude.euler.phi);
                  pstData->m_attitude.euler.theta = FloatSwap(pstData->m_attitude.euler.theta);
                  pstData->m_attitude.euler.psi = FloatSwap(pstData->m_attitude.euler.psi);
                  memmove (&pstData->m_sensor.rate[0], &pPayloadData[index], sizeof(pstData->m_sensor.rate));index += (sizeof(pstData->m_sensor.rate));
                  pstData->m_sensor.rate[0] = FloatSwap(pstData->m_sensor.rate[0]);
                  pstData->m_sensor.rate[1] = FloatSwap(pstData->m_sensor.rate[1]);
                  pstData->m_sensor.rate[2] = FloatSwap(pstData->m_sensor.rate[2]);

                  memmove (&pstData->m_attitude.q, &pPayloadData[index], sizeof(pstData->m_attitude.q));index += sizeof(quat_s_v);
                  pstData->m_attitude.q.s = FloatSwap(pstData->m_attitude.q.s);
                  pstData->m_attitude.q.v.x = FloatSwap(pstData->m_attitude.q.v.x);
                  pstData->m_attitude.q.v.y = FloatSwap(pstData->m_attitude.q.v.y);
                  pstData->m_attitude.q.v.z = FloatSwap(pstData->m_attitude.q.v.z);

                  memmove (&pstData->m_attrates.q, &pPayloadData[index], sizeof(pstData->m_attrates.q));index += sizeof(quat_s_v);
                  pstData->m_attrates.q.s = FloatSwap(pstData->m_attrates.q.s);
                  pstData->m_attrates.q.v.x = FloatSwap(pstData->m_attrates.q.v.x);
                  pstData->m_attrates.q.v.y = FloatSwap(pstData->m_attrates.q.v.y);
                  pstData->m_attrates.q.v.z = FloatSwap(pstData->m_attrates.q.v.z);
                  //Now put the rates into the Euler section as well.  User can use pstData->m_attitude and pstData->m_attrates structures for control
                  pstData->m_attrates.euler.phi = pstData->m_sensor.rate[0];
                  pstData->m_attrates.euler.theta = pstData->m_sensor.rate[1];
                  pstData->m_attrates.euler.psi = pstData->m_sensor.rate[2];

                  gCalStatus = pPayloadData[index]; index ++;
                  gCHIMU_BIT = pPayloadData[index]; index ++;

                  gConfigInfo = pPayloadData[index]; index ++;
                  bC0_SPI_En = BitTest (gConfigInfo, 0); 
                  bC1_HWCentrip_En = BitTest (gConfigInfo, 1); 
                  bC2_TempCal_En = BitTest (gConfigInfo, 2); 
                  bC3_RateOut_En = BitTest (gConfigInfo, 3); 
                  bC4_TBD = BitTest (gConfigInfo, 4); 
                  bC5_Quat_Est = BitTest (gConfigInfo, 5); 
                  bC6_SWCentrip_En = BitTest (gConfigInfo, 6); 
                  bC7_AllowHW_Override = BitTest (gConfigInfo, 7); 

                  //CHIMU currently (v 1.3) does not compute Eulers if quaternion estimator is selected
                  if(bC5_Quat_Est == TRUE)
                  {
                    pstData->m_attitude = GetEulersFromQuat((pstData->m_attitude));
                  }


                  //NEW:  Checks for bad attitude data (bad SPI maybe?)
                  //      Only allow globals to contain updated data if it makes sense
                  sanity_check = (pstData->m_attitude.q.s * pstData->m_attitude.q.s);
                  sanity_check += (pstData->m_attitude.q.v.x * pstData->m_attitude.q.v.x);
                  sanity_check += (pstData->m_attitude.q.v.y * pstData->m_attitude.q.v.y);
                  sanity_check += (pstData->m_attitude.q.v.z * pstData->m_attitude.q.v.z);

                  //Before moving data to the global, make sure there were no transmission errors via SPI
                  index = 0;
                  index += ((sanity_check <0.8) || (sanity_check >1.2)); //The normalized quaternion should be 1.0
                  //Range checks on attitude (use 365 degrees, nothing should be bigger than 360)
                  index += ((pstData->m_attitude.euler.phi > 6.37 ) || (pstData->m_attitude.euler.phi < -6.37 ));
                  index += ((pstData->m_attitude.euler.theta > 6.37 ) || (pstData->m_attitude.euler.theta < -6.37 ));
                  index += ((pstData->m_attitude.euler.psi > 6.37 ) || (pstData->m_attitude.euler.psi < -6.37 ));
                  //Max rate:  2500 deg/s (43 rad/s)
                  index += ((pstData->m_sensor.rate[0] >43) || (pstData->m_sensor.rate[0] <-43));
                  index += ((pstData->m_sensor.rate[1] >43) || (pstData->m_sensor.rate[1] <-43));
                  index += ((pstData->m_sensor.rate[2] >43) || (pstData->m_sensor.rate[2] <-43));

                  if (index >0)
                    {
                      gCHIMUMissedMessages++;
                      return FALSE;
                    }
                  
                  //Passed the tests, load the data up
                  gAttitude = pstData->m_attitude;
                  gAttRates = pstData->m_attrates;
                  gSensor.rate[0] = pstData->m_sensor.rate[0];
                  gSensor.rate[1] = pstData->m_sensor.rate[1];
                  gSensor.rate[2] = pstData->m_sensor.rate[2];                  
                  gAttitudeReceived = TRUE;

 
                  return TRUE;
                  break;
		case CHIMU_Msg_4_BiasSF:
                  return FALSE;
                  break;
		case CHIMU_Msg_5_BIT:
                  index = 0;
                  gCHIMU_Mode = pPayloadData[index]; index ++;
                  gCHIMU_BIT = pPayloadData[index]; index ++; 
                  return TRUE;   
                  break;
		case CHIMU_Msg_6_MagCal:
                  //Decodes magnetometer calibration data
                  for (index = 0;index<12;index +=2)
                    {
                      memmove (&stemp, &pPayloadData[index], sizeof(short));
                      gCHIMU_Mags[i++] = ShortSwap(stemp);                  
                    }
                  return TRUE;
                  break;
		case CHIMU_Msg_7_GyroBias:
                  index = 0;
                  memmove (&gGyroBias, &pPayloadData[index], sizeof(gGyroBias));index += sizeof(gGyroBias);
                  gGyroBias.phi = FloatSwap(gGyroBias.phi);
                  gGyroBias.theta = FloatSwap(gGyroBias.theta);
                  gGyroBias.psi = FloatSwap(gGyroBias.psi);    
                  return TRUE;    
                  break;
    case CHIMU_Msg_8_TempCal:
		case CHIMU_Msg_9_DAC_Offsets:
		case CHIMU_Msg_10_Res:
		case CHIMU_Msg_11_Res:
		case CHIMU_Msg_12_Res:
		case CHIMU_Msg_13_Res:
      return FALSE;
      break;
		case CHIMU_Msg_14_RefVector:
          //Decodes mag reference vector data
          memmove (&ftemp, &pPayloadData[0],sizeof(float));
          gCHIMU_MagRefVector.x = FloatSwap(ftemp);
          memmove (&ftemp, &pPayloadData[4],sizeof(float));
          gCHIMU_MagRefVector.y = FloatSwap(ftemp);
          memmove (&ftemp, &pPayloadData[8],sizeof(float));
          gCHIMU_MagRefVector.z = FloatSwap(ftemp);
          memmove (&ftemp, &pPayloadData[12],sizeof(float));
          gCHIMU_Declination = FloatSwap(ftemp);
          memmove (&ftemp, &pPayloadData[16],sizeof(float));
          gCHIMU_Inclination = FloatSwap(ftemp);
          pCHIMU_MagData_Read = TRUE;
      return TRUE;
      break;
		case CHIMU_Msg_15_SFCheck:
      return FALSE;
      break;
		default:
                  return FALSE;
                  break;
	}
}

/**
 * GetEulersFromQuat
 *
 * Gets Euler angles from quaternion output (only needed for original CHIMU, CHIMU-J performs on board at 200 Hz)
 * 
 * @param  attitude CHIMU_attitude_data structure
 * @return CHIMU_attitude_data holds attitude data
 */
CHIMU_attitude_data GetEulersFromQuat(CHIMU_attitude_data attitude)
{
  CHIMU_attitude_data ps;
  ps = attitude;
  float x, sqw,sqx,sqy,sqz,norm;
  sqw = ps.q.s * ps.q.s;
  sqx = ps.q.v.x * ps.q.v.x;
  sqy = ps.q.v.y * ps.q.v.y;
  sqz = ps.q.v.z * ps.q.v.z;
  norm = sqrt(sqw + sqx + sqy + sqz);
  //Normalize the quat
  ps.q.s = ps.q.s / norm;
  ps.q.v.x = ps.q.v.x / norm;
  ps.q.v.y = ps.q.v.y / norm;
  ps.q.v.z = ps.q.v.z / norm;
  ps.euler.phi =atan2(2.0 * (ps.q.s * ps.q.v.x + ps.q.v.y * ps.q.v.z), (1 - 2 * (sqx + sqy)));
  if (ps.euler.phi < 0)  ps.euler.phi = ps.euler.phi + 2 *PI;
  x = ((2.0 * (ps.q.s * ps.q.v.y - ps.q.v.z * ps.q.v.x)));
  //Below needed in event normalization not done
  if (x > 1.0) x = 1.0;
  if (x < -1.0) x = -1.0;
  //
  if ((ps.q.v.x * ps.q.v.y + ps.q.v.z * ps.q.s) == 0.5) 
          {
          ps.euler.theta = 2 *atan2(ps.q.v.x, ps.q.s);
          }
          else
          if ((ps.q.v.x * ps.q.v.y + ps.q.v.z * ps.q.s) == -0.5) 
                  {
                  ps.euler.theta = -2 *atan2(ps.q.v.x, ps.q.s);
                  }
          else{
                  ps.euler.theta = asin(x);
                  }
  ps.euler.psi = atan2(2.0 * (ps.q.s * ps.q.v.z + ps.q.v.x * ps.q.v.y), (1 - 2 * (sqy + sqz)));
  if (ps.euler.psi < 0) 
          {
           ps.euler.psi = ps.euler.psi + (2 * PI);
          }

  return(ps);
  
}


//END CHIMU AHRS Parsing and Command Functions
/* @} */

//END Group
/* @} */




