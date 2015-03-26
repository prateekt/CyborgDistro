/***********************************************************************
MODULE:     util.h
VERSION:    1.00
CONTAINS:   Utility functions file for Monkey
COPYRIGHT:  Ryan Mechatronics
Date:       November 2009 - Pre-release for official Monkey
***********************************************************************/
#ifndef UTIL_H
#define UTIL_H

void BitSet (unsigned char *input, unsigned char n);

void BitClear (unsigned char *input, unsigned char n);

unsigned char BitTest (unsigned char input, unsigned char n);

unsigned char FormByte (unsigned char n7,unsigned char n6,unsigned char n5,unsigned char n4,unsigned char n3,unsigned char n2,unsigned char n1,unsigned char n0);

void BitSetLong (unsigned long *input, unsigned char n);

void BitClearLong (unsigned long *input, unsigned char n);

unsigned char BitTestLong (unsigned long input, unsigned char n);

void BuiltInTest(unsigned char bitfield, unsigned char set);

float ScaleNum(float value, float oldLLIM, float oldULIM, float newLLIM, float newULIM);

void unwrap_angle_2PI( float *angleinput );

void unwrap_angle_PI( float *angleinput );

float min_angle_difference( float *angle1, float *angle2 );

unsigned char *i_toa( int i );

unsigned char *l_toa( long i );

#endif 

