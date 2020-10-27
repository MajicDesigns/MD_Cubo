#include <MD_Cubo.h>
#include "MD_Cubo_nxn_DirectIO.h"

/**
 * \file
 * \brief Main file for the MD_Cubo Direct I/O object
 */

#define PIX(x,y) ((x * CUBE_SIZE) + y)    // pixel bit in the data word

void MD_Cubo_DirectIO::begin()
{
  PRINTS("\nDIO: begin");
  for (uint8_t i = 0; i < CUBE_SIZE; i++)
  {
    pinMode(_enableLayer[i], OUTPUT);
    digitalWrite(_enableLayer[i], HIGH);
  }

  for (uint8_t i = 0; i < CUBE_SIZE; i++)
    for (uint8_t j = 0; j < CUBE_SIZE; j++)
    {
      pinMode(_LEDLayer[i][j], OUTPUT);
      digitalWrite(_LEDLayer[i][j], LOW);
    }

  memset(&_scratch, 0, sizeof(dispData_t));
  memset(&_current, 0, sizeof(dispData_t));
  _timeLayer = millis();
}

void MD_Cubo_DirectIO::setIO(uint8_t layer)
// Set the I/O for all the LEDS in a layer
{
  // first set all the layers off ...
  for (uint8_t i = 0; i < CUBE_SIZE; i++)
    digitalWrite(_enableLayer[i], HIGH);

  // ... now set the I/O for the LEDs ...
   for (uint8_t i = 0; i < CUBE_SIZE; i++)
     for (uint8_t j = 0; j < CUBE_SIZE; j++)
       digitalWrite(_LEDLayer[i][j], (bitRead(_current.data[layer], PIX(i,j)) ? HIGH : LOW));

  // ... and then turn on the current layer
  digitalWrite(_enableLayer[layer], LOW);
}

void MD_Cubo_DirectIO::animate(uint32_t wait)
{
  uint32_t  timeStart = millis();
  
  PRINT("\nDIO: animate ", wait);
  do 
  {
    if (millis() - _timeLayer >= LAYER_TIME)
    {
      // select the new layer one
      _curLayer = (_curLayer + 1) % CUBE_SIZE;

      // write the data for this layer
      setIO(_curLayer);

      // set a new time
      _timeLayer = millis();
    }
  } while (millis() - timeStart < wait);
}

void MD_Cubo_DirectIO::update()
//Update the cube LEDs from the scratch to the display
{
  PRINTS("\nDIO: update");
  memcpy(&_current, &_scratch, sizeof(dispData_t));
}

void MD_Cubo_DirectIO::setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z)
{
  PRINTX("\nDIO: setVoxel ", p);
  PRINT(" @ ", x);
  PRINT(",",y);
  PRINT(",",z);

  if ((x >= CUBE_SIZE) || (y >= CUBE_SIZE) || (z >= CUBE_SIZE))
    return;
  
  uint16_t  pix = PIX(x, y);
  boolean b = (p != VOX_OFF);

  if (p)
    bitSet(_scratch.data[z], pix);
  else
    bitClear(_scratch.data[z], pix);
  _scratch.count[z] += (b ? 1 : -1);
  
  PRINTX(" layer data ", _scratch.data[z]);
}

uint32_t MD_Cubo_DirectIO::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  PRINTS("\nDIO: getVoxel");

  if ((x >= CUBE_SIZE) || (y >= CUBE_SIZE) || (z >= CUBE_SIZE))
    return(VOX_OFF);

  uint16_t  pix = PIX(x, y);

  return (bitRead(_scratch.data[z], pix) ? VOX_ON : VOX_OFF);
}
