/**************************************************************************/
/*! 
    @file     CHIMUParserClass.h
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

//---[Defines]------------------------------------------------------
#ifndef CHIMU_DEFINED_H
#define CHIMU_DEFINED_H

#include "globals.h"

#define CHIMU_STATE_MACHINE_START   	0
#define CHIMU_STATE_MACHINE_HEADER2   	1
#define CHIMU_STATE_MACHINE_LEN		2
#define CHIMU_STATE_MACHINE_DEVICE	3
#define CHIMU_STATE_MACHINE_ID		4
#define CHIMU_STATE_MACHINE_PAYLOAD	5
#define CHIMU_STATE_MACHINE_XSUM	6


//---[DEFINES for Message List]---------------------------------------
//Message ID's that go TO the CHIMU
#define MSG00_PING		0x00
#define MSG01_BIAS		0x01
#define MSG02_DACMODE		0x02
#define MSG03_CALACC		0x03
#define MSG04_CALMAG		0x04
#define MSG05_CALRATE		0x05
#define MSG06_CONFIGCLR		0x06
#define MSG07_CONFIGSET		0x07
#define MSG08_SAVEGYROBIAS	0x08
#define MSG09_ESTIMATOR		0x09
#define MSG0A_SFCHECK		0x0A
#define MSG0B_CENTRIP		0x0B
#define MSG0C_INITGYROS		0x0C
#define MSG0D_DEVICEID		0x0D
#define MSG0E_REFVECTOR		0x0E
#define MSG0F_RESET		0x0F
#define MSG10_UARTSETTINGS	0x10
#define MSG11_SERIALNUMBER	0x11

//Output message identifiers from the CHIMU unit
//---[Defines]------------------------------------------------------
#define CHIMU_Msg_0_Ping			0
#define CHIMU_Msg_1_IMU_Raw                     1
#define CHIMU_Msg_2_IMU_FP			2
#define CHIMU_Msg_3_IMU_Attitude                3
#define CHIMU_Msg_4_BiasSF			4
#define CHIMU_Msg_5_BIT                         5
#define CHIMU_Msg_6_MagCal			6
#define CHIMU_Msg_7_GyroBias                    7
#define CHIMU_Msg_8_TempCal                     8
#define CHIMU_Msg_9_DAC_Offsets                 9
#define CHIMU_Msg_10_Res			10
#define CHIMU_Msg_11_Res			11
#define CHIMU_Msg_12_Res			12
#define CHIMU_Msg_13_Res			13
#define CHIMU_Msg_14_RefVector                  14
#define CHIMU_Msg_15_SFCheck                    15


//---[COM] defines
#define CHIMU_COM_ID_LOW	0x00
#define CHIMU_COM_ID_HIGH	0x1F  //Must set this to the max ID expected above

#define NP_MAX_CMD_LEN			8		// maximum command length (CHIMU address)
#define NP_MAX_DATA_LEN			256		// maximum data length
#define NP_MAX_CHAN			36		// maximum number of channels
#define NP_WAYPOINT_ID_LEN		32		// waypoint max string len
#define NP_XSUM_LEN                     3               // chars in checksum string

#define CHIMU_STANDARD   0x00
#define CHIMU_RX_BUFFERSIZE 128

typedef struct {
    unsigned char	m_State;			// Current state protocol parser is in
    unsigned char 	m_Checksum;			// Calculated CHIMU sentence checksum
    unsigned char 	m_ReceivedChecksum;		// Received CHIMU sentence checksum (if exists)
    unsigned char   m_Index;			// Index used for command and data
    unsigned char   m_PayloadIndex;
    unsigned char   m_MsgID;
    unsigned char   m_MsgLen;
    unsigned char   m_TempDeviceID;
    unsigned char   m_DeviceID;
    unsigned char   m_Payload[CHIMU_RX_BUFFERSIZE];        // CHIMU data
    unsigned char   m_FullMessage[CHIMU_RX_BUFFERSIZE];	// CHIMU data
    CHIMU_attitude_data m_attitude;
    CHIMU_attitude_data m_attrates;
    CHIMU_sensor_data   m_sensor;
    unsigned int  m_CHIMU_TimeCnts;

} CHIMU_PARSER_DATA;


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
void CHIMU_Init(CHIMU_PARSER_DATA   *pstData);

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
unsigned char CHIMU_Parse(unsigned char btData, unsigned char bInputType, CHIMU_PARSER_DATA *pstData);

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
unsigned char CHIMU_ProcessMessage(unsigned char *pMsgID, char *pPayloadData, CHIMU_PARSER_DATA  *pstData);


/**
 * GetEulersFromQuat
 *
 * Gets Euler angles from quaternion output (only needed for original CHIMU, CHIMU-J performs on board at 200 Hz)
 * 
 * @param  attitude CHIMU_attitude_data structure
 * @return CHIMU_attitude_data holds attitude data
 */
CHIMU_attitude_data GetEulersFromQuat(CHIMU_attitude_data attitude);



//END CHIMU AHRS Parsing and Command Functions
/* @} */



#endif // CHIMU_DEFINED

