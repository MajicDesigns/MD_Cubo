#include <MD_Cubo.h>
#include "MD_Cubo_4x4_72xx.h"

void MD_Cubo_72xx::begin()
{
  pinMode(_data, OUTPUT);
  pinMode(_clock,OUTPUT);
  pinMode(_load, OUTPUT);

  sendSPI(ON_OFF,      1);  // switch on LED Driver
  sendSPI(DECODE,      0);  // no decoding on any digits
  sendSPI(SCAN_DIGITS, 7);  // scan all digits
  sendSPI(TEST_MODE,   0);  // test mode off
  setIntensity(MAX_INTENSITY);
}

void MD_Cubo_72xx::sendSPI(byte reg, uint8_t val)
// send 2 bytes to the MAX7219 Driver IC
{
  digitalWrite(_load, LOW); // load data

  shiftOut(_data, _clock, MSBFIRST, reg); // send register
  shiftOut(_data, _clock, MSBFIRST, val); // send value

  digitalWrite(_load, HIGH);
}

void MD_Cubo_72xx::update()
//Update the cube LEDs
{
  sendSPI(2, highByte(_layer[0]));
  sendSPI(6, lowByte (_layer[0]));
  sendSPI(8, highByte(_layer[1]));
  sendSPI(4, lowByte (_layer[1]));
  sendSPI(3, highByte(_layer[2]));
  sendSPI(7, lowByte (_layer[2]));
  sendSPI(5, highByte(_layer[3]));
  sendSPI(1, lowByte (_layer[3]));
}

static uint8_t mapBit[CUBE_SIZE][CUBE_SIZE] =
{
  { 11, 12, 13, 14 },  // x=0 plane
  { 15, 10, 8, 9 },
  { 3, 4, 5, 6 },
  { 7, 2, 0, 1 },  // x=3 plane
};

void MD_Cubo_72xx::setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z)
{
  
  if ((x>CUBE_SIZE) || (y>CUBE_SIZE) || (z>CUBE_SIZE))
    return;
  
  bitWrite(_layer[z], mapBit[x][y], p);
}

boolean MD_Cubo_72xx::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{

  if ((x>CUBE_SIZE) || (y>CUBE_SIZE) || (z>CUBE_SIZE))
    return(false);

  return(bitRead(_layer[z], mapBit[x][y]) != 0);
}
