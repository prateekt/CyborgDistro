/**************************************************************************/
/*! 
    @file     crc.c
    @author   M. Ryan (ryanmechatronics.com)
    @date     18 July 2011
    @version  2.0

    @section DESCRIPTION

    CRC checksum calculations for Monkey


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

#include "crc.h"

#define Poly32 0x04C11DB7

/** @addtogroup COM_Functions
  * @{
  */

/** @defgroup CRC CRC Checksum Functions
  * Functions for CRC checksum calculations 
  * @{
  */

/**
 * Computes CRC for an input buffer
 * 
 * @param CRC_acc CRC initial value
 * @param *data buffer of bytes to compute checksum
 * @param data_len number of bytes to run thru CRC
 * @return CRC 
 */
unsigned long UpdateCRC(unsigned long CRC_acc, void *data, unsigned long data_len)
{
  unsigned long i; // loop counter
#define POLY 0xEDB88320 // bit-reversed version of the poly 0x04C11DB7
  // Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
  // with no carries)

  unsigned char *CRC_input = (unsigned char*)data;
  for (unsigned long j = data_len; j; --j)
  {
  
    CRC_acc = CRC_acc ^ *CRC_input++;
    // "Divide" the poly into the dividend using CRC XOR subtraction
    // CRC_acc holds the "remainder" of each divide
    //
    // Only complete this division for 8 bits since input is 1 byte
    for (i = 0; i < 8; i++)
    {
    // Check if the MSB is set (if MSB is 1, then the POLY can "divide"
    // into the "dividend")
    if ((CRC_acc & 0x00000001) == 0x00000001)
    {
    // if so, shift the CRC value, and XOR "subtract" the poly
    CRC_acc = CRC_acc >> 1;
    CRC_acc ^= POLY;
    }
    else
    {
    // if not, just shift the CRC value
    CRC_acc = CRC_acc >> 1;
    }
    }
  }
  // Return the final remainder (CRC value)
  return CRC_acc;
}

/*


const unsigned long CRC_Table32[]	=
{
	0x00000000, 0x06233697, 0x05C45641, 0x03E760D6, 0x020A97ED, 0x0429A17A, 0x07CEC1AC, 0x01EDF73B,
	0x04152FDA, 0x0236194D, 0x01D1799B, 0x07F24F0C, 0x061FB837, 0x003C8EA0, 0x03DBEE76, 0x05F8D8E1,
	0x01A864DB, 0x078B524C, 0x046C329A, 0x024F040D, 0x03A2F336, 0x0581C5A1, 0x0666A577, 0x004593E0,
	0x05BD4B01, 0x039E7D96, 0x00791D40, 0x065A2BD7, 0x07B7DCEC, 0x0194EA7B, 0x02738AAD, 0x0450BC3A,
	0x0350C9B6, 0x0573FF21, 0x06949FF7, 0x00B7A960, 0x015A5E5B, 0x077968CC, 0x049E081A, 0x02BD3E8D,
	0x0745E66C, 0x0166D0FB, 0x0281B02D, 0x04A286BA, 0x054F7181, 0x036C4716, 0x008B27C0, 0x06A81157,
	0x02F8AD6D, 0x04DB9BFA, 0x073CFB2C, 0x011FCDBB, 0x00F23A80, 0x06D10C17, 0x05366CC1, 0x03155A56,
	0x06ED82B7, 0x00CEB420, 0x0329D4F6, 0x050AE261, 0x04E7155A, 0x02C423CD, 0x0123431B, 0x0700758C,
	0x06A1936C, 0x0082A5FB, 0x0365C52D, 0x0546F3BA, 0x04AB0481, 0x02883216, 0x016F52C0, 0x074C6457,
	0x02B4BCB6, 0x04978A21, 0x0770EAF7, 0x0153DC60, 0x00BE2B5B, 0x069D1DCC, 0x057A7D1A, 0x03594B8D,
	0x0709F7B7, 0x012AC120, 0x02CDA1F6, 0x04EE9761, 0x0503605A, 0x032056CD, 0x00C7361B, 0x06E4008C,
	0x031CD86D, 0x053FEEFA, 0x06D88E2C, 0x00FBB8BB, 0x01164F80, 0x07357917, 0x04D219C1, 0x02F12F56,
	0x05F15ADA, 0x03D26C4D, 0x00350C9B, 0x06163A0C, 0x07FBCD37, 0x01D8FBA0, 0x023F9B76, 0x041CADE1,
	0x01E47500, 0x07C74397, 0x04202341, 0x020315D6, 0x03EEE2ED, 0x05CDD47A, 0x062AB4AC, 0x0009823B,
	0x04593E01, 0x027A0896, 0x019D6840, 0x07BE5ED7, 0x0653A9EC, 0x00709F7B, 0x0397FFAD, 0x05B4C93A,
	0x004C11DB, 0x066F274C, 0x0588479A, 0x03AB710D, 0x02468636, 0x0465B0A1, 0x0782D077, 0x01A1E6E0,
	0x04C11DB7, 0x02E22B20, 0x01054BF6, 0x07267D61, 0x06CB8A5A, 0x00E8BCCD, 0x030FDC1B, 0x052CEA8C,
	0x00D4326D, 0x06F704FA, 0x0510642C, 0x033352BB, 0x02DEA580, 0x04FD9317, 0x071AF3C1, 0x0139C556,
	0x0569796C, 0x034A4FFB, 0x00AD2F2D, 0x068E19BA, 0x0763EE81, 0x0140D816, 0x02A7B8C0, 0x04848E57,
	0x017C56B6, 0x075F6021, 0x04B800F7, 0x029B3660, 0x0376C15B, 0x0555F7CC, 0x06B2971A, 0x0091A18D,
	0x0791D401, 0x01B2E296, 0x02558240, 0x0476B4D7, 0x059B43EC, 0x03B8757B, 0x005F15AD, 0x067C233A,
	0x0384FBDB, 0x05A7CD4C, 0x0640AD9A, 0x00639B0D, 0x018E6C36, 0x07AD5AA1, 0x044A3A77, 0x02690CE0,
	0x0639B0DA, 0x001A864D, 0x03FDE69B, 0x05DED00C, 0x04332737, 0x021011A0, 0x01F77176, 0x07D447E1,
	0x022C9F00, 0x040FA997, 0x07E8C941, 0x01CBFFD6, 0x002608ED, 0x06053E7A, 0x05E25EAC, 0x03C1683B,
	0x02608EDB, 0x0443B84C, 0x07A4D89A, 0x0187EE0D, 0x006A1936, 0x06492FA1, 0x05AE4F77, 0x038D79E0,
	0x0675A101, 0x00569796, 0x03B1F740, 0x0592C1D7, 0x047F36EC, 0x025C007B, 0x01BB60AD, 0x0798563A,
	0x03C8EA00, 0x05EBDC97, 0x060CBC41, 0x002F8AD6, 0x01C27DED, 0x07E14B7A, 0x04062BAC, 0x02251D3B,
	0x07DDC5DA, 0x01FEF34D, 0x0219939B, 0x043AA50C, 0x05D75237, 0x03F464A0, 0x00130476, 0x063032E1,
	0x0130476D, 0x071371FA, 0x04F4112C, 0x02D727BB, 0x033AD080, 0x0519E617, 0x06FE86C1, 0x00DDB056,
	0x052568B7, 0x03065E20, 0x00E13EF6, 0x06C20861, 0x072FFF5A, 0x010CC9CD, 0x02EBA91B, 0x04C89F8C,
	0x009823B6, 0x06BB1521, 0x055C75F7, 0x037F4360, 0x0292B45B, 0x04B182CC, 0x0756E21A, 0x0175D48D,
	0x048D0C6C, 0x02AE3AFB, 0x01495A2D, 0x076A6CBA, 0x06879B81, 0x00A4AD16, 0x0343CDC0, 0x0560FB57
};

unsigned long UpdateCRC32(unsigned long crc, unsigned char b)
{
	return ((crc >> 8) ^ CRC_Table32[(crc ^ b) & 0xff]);
}


//__ramfunc __arm	// place in sram for maximum speed .. a ram based loot-up table would also speed this up (if you can spare the sram)
unsigned long UpdateCRC32Data(unsigned long crc, void *data, unsigned long data_len)
{
  unsigned char *p = (unsigned char*)data;
  unsigned long _crc = crc;
  for (unsigned long i = data_len; i; --i)
  {
    _crc = (_crc >> 8) ^ CRC_Table32[(_crc ^ *p++) & 0xff];
  }
//			_crc = UpdateCRC32(_crc, *p++);
  return _crc;
}

void MakeCRC_Table32(void)
{	// Generate the CRC table in ram
	for (u32 i = 0; i < 256; i++)
	{
		u32 crc = i;
		for (u32 j = 8; j; --j)
			crc = (crc & 1) ? (crc >> 1) ^ Poly32 : crc >> 1;
		CRC_Table32[i] = crc;
	}
}
*/

 
//END Group
/* @} */

//END Group
/* @} */

