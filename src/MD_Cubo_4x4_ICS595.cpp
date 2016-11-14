#include <MD_Cubo.h>
#include "MD_CUBO_4x4_ICS595.h"

void MD_Cubo_ICS595::begin()
{
  pinMode(_data, OUTPUT);
  pinMode(_clock,OUTPUT);
  pinMode(_load, OUTPUT);
  pinMode(_enable, OUTPUT);

  for (uint8_t i=0; i<CUBE_SIZE; i++)
  {
    pinMode(_enableLayer[i], OUTPUT);
    digitalWrite(_enableLayer[i], HIGH);
  }
  
  memset(&_scratch, 0, sizeof(dispData_t));
  memset(&_current, 0, sizeof(dispData_t));
  _timeLayer = millis();
  digitalWrite(_enable, LOW); // enable outputs on 595
}

void MD_Cubo_ICS595::send595(uint16_t val)
// send 2 bytes to the HC595 Driver ICs
{
  digitalWrite(_load, LOW);

  shiftOut(_data, _clock, LSBFIRST, val); 
  shiftOut(_data, _clock, LSBFIRST, (val >> 8)); 

  digitalWrite(_load, HIGH);
}

void MD_Cubo_ICS595::animate(uint32_t wait)
{
  uint32_t  timeStart = millis();
  
  PRINT("\nAnimate ", wait);
  do 
  {
    if (millis() - _timeLayer >= LAYER_TIME)
    {
      // stop output to prevent ghosting
      digitalWrite(_enable, HIGH);
      
      // reset this layer and select the new one
      digitalWrite(_enableLayer[_curLayer], HIGH);
      _curLayer = (_curLayer + 1) % CUBE_SIZE;
      digitalWrite(_enableLayer[_curLayer], LOW);
      
      // write the data for this layer
      send595(_current.data[_curLayer]);
      // re-enable output
      digitalWrite(_enable, LOW);
      
      // set a new time
      _timeLayer = millis();
    }      
  } while (millis() - timeStart < wait);
}

void MD_Cubo_ICS595::update()
//Update the cube LEDs from the scratch to the display
{
  memcpy(&_current, &_scratch, sizeof(dispData_t));
}

void MD_Cubo_ICS595::setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z)
{
  PRINT("\nPixel ", p);
  PRINT(" @ ", x);
  PRINT(",",y);
  PRINT(",",z);
  if ((x>CUBE_SIZE) || (y>CUBE_SIZE) || (z>CUBE_SIZE))
    return;
  
  uint16_t  pix = (x + (y << 2));  // x + (y * 4)
  

  bitWrite(_scratch.data[z], pix, p);
  _scratch.count[z] += (p ? 1 : -1);
  
  PRINTX(" layer data ", _scratch.data[z]);
}

boolean MD_Cubo_ICS595::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  if ((x>CUBE_SIZE) || (y>CUBE_SIZE) || (z>CUBE_SIZE))
    return(false);

  uint16_t  pix = x + (y << 2);  // x + (y * 4)

  return (bitRead(_scratch.data[z], pix) != 0);
}
