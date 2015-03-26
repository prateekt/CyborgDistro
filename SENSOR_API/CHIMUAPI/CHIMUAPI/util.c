/***********************************************************************
MODULE:     util.c
VERSION:    1.00
CONTAINS:   Utility functions file for Monkey
COPYRIGHT:  Ryan Mechatronics
Date:       November 2009 - Pre-release for official Monkey
***********************************************************************/

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include "util.h"
#include "math.h"
#include "globals.h"


//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
// Found in util.h

//All functions set for only an unsigned character input
void BitSet (unsigned char *input, unsigned char n)
{
//Set a bit (where n is the bit number, and 0 is the least significant bit):
	*input |= (1 << n);
}

void BitClear (unsigned char *input, unsigned char n)
{
//Clear a bit (where n is the bit number, and 0 is the least significant bit):
	*input &= ~(1 << n);
}
unsigned char BitTest (unsigned char input, unsigned char n)
{
//Test a bit in n and return TRUE or FALSE
	if ( input & (1 << n)) return TRUE; else return FALSE;
}


unsigned char FormByte (unsigned char n7,unsigned char n6,unsigned char n5,unsigned char n4,unsigned char n3,unsigned char n2,unsigned char n1,unsigned char n0)
{
//Form a byte from representative bits, bits input as unsigned char though...
//
//Very slow, don't use frequently
	return ((n7 * 128) + (n6 * 64) + (n5 * 32) + (n4 * 16) + (n3 * 8) + (n2 * 4) + (n1 * 2) + (n0));
}


//All functions set for only an unsigned character input
void BitSetLong (unsigned long *input, unsigned char n)
{
//Set a bit (where n is the bit number, and 0 is the least significant bit):
	*input |= (1 << n);
}

void BitClearLong (unsigned long *input, unsigned char n)
{
//Clear a bit (where n is the bit number, and 0 is the least significant bit):
	*input &= ~(1 << n);
}
unsigned char BitTestLong (unsigned long input, unsigned char n)
{
//Test a bit in n and return TRUE or FALSE
	if ( input & (1 << n)) return TRUE; else return FALSE;
}

/***********************************************
* Built In Test Functions
***********************************************/

void BuiltInTest(unsigned char bitfield, unsigned char set)
{
  unsigned char val = 0;
	if (set == TRUE){
		BitSet (&val, bitfield);
	}
	if (set == FALSE){
		BitClear (&val, bitfield); 
	}
   //gMonkey.BIT = val;
}

/***********************************************
* Scaling Functions
***********************************************/
float ScaleNum(float value, float oldLLIM, float oldULIM, float newLLIM, float newULIM)
{
    float d = 0;
    
    // determine ratio of value in old range
    d = (value - oldLLIM) / (oldULIM - oldLLIM);
    
    // map ratio to new range            
    value = newLLIM + (d * (newULIM - newLLIM));
    return value;
}




/***********************************************
* Angle Functions
***********************************************/
	
void unwrap_angle_2PI( float *angleinput )
// This is just a simplified mod function that unwraps and returns a value with the 0 to 2PI range
{
	if ((*angleinput<0) || (*angleinput> TWOPI)){
		while (*angleinput > TWOPI) *angleinput -= TWOPI;
		while (*angleinput < 0.0) *angleinput += TWOPI;
	}
}


void unwrap_angle_PI( float *angleinput )
// This is just a simplified mod function -  0 to PI range however
{
	if ( *angleinput > PI) {
		*angleinput = TWOPI - *angleinput;
	}
	if (*angleinput < NEGPI) {
		*angleinput += TWOPI;
	}
}


float min_angle_difference( float *angle1, float *angle2 )
// Minimum between the two (angle1-angle2) - Useful for determining correct angle about true north for example...
{
	float e, abse1, abse2, abse3;
	if (*angle1 == *angle2) return 0.0;
	e =*angle1 - *angle2;
	abse1 = fabs(e);
	abse2 = fabs(e - TWOPI);
	abse3 = fabs(e + TWOPI);
	if((abse1<=abse2) && (abse1<=abse3)) return (e);
	if((abse2<=abse1) && (abse2<=abse3)) return (e-TWOPI);
	return (e+TWOPI);
}

// String functions

unsigned char *i_toa( int i )
{
  static unsigned char buff[16];
  int div = 10000,res;
  int lz = 1;
  int ch = 0;

  // Handle zero
  if( i == 0){
    buff[ch++] = '0';
    buff[ch] = 0;
    return buff;
  }

  // Handle negative
  if( i < 0){
    buff[ch++] = '-';
    i = -i;
  }

  // Loop through all digits
  while(div > 0) {
    res = i / div;
    i -= res * div;
    if( res || !lz) {
      lz = 0;
      buff[ch++] = '0' + res;
    }
    div /= 10;
  }

  buff[ch] = 0;

  return buff;
}


unsigned char *l_toa( long i )
{
  static unsigned char buff[16];
  long div = 1000000000,res;
  long lz = 1;
  int ch = 0;

  // Handle zero
  if( i == 0){
    buff[ch++] = '0';
    buff[ch] = 0;
    return buff;
  }

  // Handle negative
  if( i < 0){
    buff[ch++] = '-';
    i = -i;
  }

  // Loop through all digits
  while(div > 0) {
    res = i / div;
    i -= res * div;
    if( res || !lz) {
      lz = 0;
      buff[ch++] = '0' + res;
    }
    div /= 10;
  }

  buff[ch] = 0;

  return buff;
}




