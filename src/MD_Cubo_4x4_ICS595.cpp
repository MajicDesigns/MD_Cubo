#include <MD_Cubo.h>
#include "MD_CUBO_4x4_ICS595.h"

/**
 * \file
 * \brief Main file for the MD_Cubo ICStation 595 object
 */

void MD_Cubo_ICS595::begin()
{
  PRINTS("\n595: begin");
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
  
  PRINT("\n595: Animate ", wait);
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
  PRINTS("\n595: Update");
  memcpy(&_current, &_scratch, sizeof(dispData_t));
}

void MD_Cubo_ICS595::setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z)
{
  PRINTX("\n595: setVoxel ", p);
  PRINT(" @ ", x);
  PRINT(",",y);
  PRINT(",",z);

  if ((x >= CUBE_SIZE) || (y >= CUBE_SIZE) || (z >= CUBE_SIZE))
    return;
  
  uint16_t  pix = (x + (y << 2));  // same as x + (y * 4)
  boolean b = (p != VOX_OFF);

  if (p)
    bitSet(_scratch.data[z], pix);
  else
    bitClear(_scratch.data[z], pix);
  _scratch.count[z] += (b ? 1 : -1);
  
  PRINTX(" layer data ", _scratch.data[z]);
}

uint32_t MD_Cubo_ICS595::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  PRINTS("\n595: getVoxel");

  if ((x >= CUBE_SIZE) || (y >= CUBE_SIZE) || (z >= CUBE_SIZE))
    return(VOX_OFF);

  uint16_t  pix = x + (y << 2);  // same as x + (y * 4)

  return (bitRead(_scratch.data[z], pix) ? VOX_ON : VOX_OFF);
}
