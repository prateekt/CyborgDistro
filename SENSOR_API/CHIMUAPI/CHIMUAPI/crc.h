/***********************************************************************
MODULE:     crc.h
VERSION:    1.00
CONTAINS:   Utility functions file for Monkey
COPYRIGHT:  Ryan Mechatronics
Date:       November 2009 - Pre-release for official Monkey
***********************************************************************/

#ifndef _crc_H
#define _crc_H

extern const unsigned long CRC_Table32[];

unsigned long UpdateCRC32(unsigned long crc, unsigned char b);

//__ramfunc __arm
unsigned long UpdateCRC32Data(unsigned long crc, void *data, unsigned long data_len);

unsigned long UpdateCRC(unsigned long CRC_acc, void *data, unsigned long data_len);


//void MakeCRC_Table32(void);

#endif
