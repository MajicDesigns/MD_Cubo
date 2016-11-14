#include <MD_Cubo.h>
#include "MD_Cubo_8x8_jC.h"

void MD_Cubo_JC::begin()
{
  pinMode(_data, OUTPUT);
  pinMode(_clock,OUTPUT);
  pinMode(_load, OUTPUT);
  digitalWrite(_load, HIGH);

  sendSPI(ON_OFF,      1);  // switch on LED Driver
  sendSPI(DECODE,      0);  // no decoding on any digits
  sendSPI(SCAN_DIGITS, 7);  // scan all digits
  sendSPI(TEST_MODE,   0);  // test mode off
  setIntensity(MAX_INTENSITY);
}

void MD_Cubo_JC::sendSPI(byte reg, uint8_t val)
// Send the same value to all the MAX7219 devices
// Used for control and setup parameters
{
  digitalWrite(_load, LOW); // load data

	for (uint8_t d = 0; d < MAX_DEVICES; d++)
	{
		shiftOut(_data, _clock, MSBFIRST, reg); // send register
		shiftOut(_data, _clock, MSBFIRST, val); // send value
	}

  digitalWrite(_load, HIGH);
}

void MD_Cubo_JC::update()
// Update the cube LEDs with the currently buffered values
// Do the same register address for all the devices in the IC chain.
// Need to pump them in reverse order to make sure that 
// logically the first cube plane is at the front.
{
  for (uint8_t i = 0; i < CUBE_YSIZE; i++)
  {
    digitalWrite(_load, LOW); // load data
    for (int8_t d = MAX_DEVICES - 1; d >= 0; d--)
	  {
		  shiftOut(_data, _clock, MSBFIRST, DIGIT0 + i);    // send register
		  shiftOut(_data, _clock, MSBFIRST, _layer[d][i]);  // send value
    }
    digitalWrite(_load, HIGH);
  }
}

void MD_Cubo_JC::setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z)
{
// The X coordinate is the device address
// The Y coordinate is the segment
// The Z coordinate is the digit
  
  if ((x >= CUBE_XSIZE) || (y >= CUBE_YSIZE) || (z >= CUBE_ZSIZE))
	  return;
    
  bitWrite(_layer[x][(CUBE_ZSIZE-1)-z], y, p);
}

boolean MD_Cubo_JC::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  // The X coordinate is the device address
  // The Y coordinate is the segment
  // The Z coordinate is the digit

  if ((x >= CUBE_XSIZE) || (y >= CUBE_YSIZE) || (z >= CUBE_ZSIZE))
    return(false);

  return(bitRead(_layer[x][(CUBE_ZSIZE - 1) - z], y) != 0);
}

uint8_t MD_Cubo_JC::size(axis_t axis)
{
  switch(axis)
  {
	  case XAXIS: return(CUBE_XSIZE);
	  case YAXIS: return(CUBE_YSIZE);
	  case ZAXIS: return(CUBE_ZSIZE);
  }
}

