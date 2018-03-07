/*
MD_Cubo - Arduino LED Cube Library
  
See header file for comments

This file contains class related methods.
  
Copyright (C) 2015 Marco Colli & Carsten Schmitz. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <MD_Cubo.h>
#include "MD_Cubo_4x4_STC.h"

/**
* \file
* \brief Main file for the MD_Cubo STC class
*/

#define TIMEOUT_TIME  5000  ///< Demo intercept timeout in milliseconds
#define RETRY_TIME    100   ///< Intercept retry time in milliseconds

void MD_Cubo_STC::begin()
{
  byte i;

  _CubeSerial.begin(_bps);
  for (i = 0; i < TIMEOUT_TIME/RETRY_TIME; i++)  // keep trying for 
  {
    _CubeSerial.write(HW_INIT);
    delay(RETRY_TIME);
  }

  clear();
  update();
}

void MD_Cubo_STC::clear(uint32_t p)
{ 
  for (uint8_t i = 0; i < LED_DATA_SIZE; i += 3)
  {
    _ledData[i + 0] = R(p);
    _ledData[i + 1] = G(p);
    _ledData[i + 2] = B(p);
  }
};

void MD_Cubo_STC::update()
// Update the cube LEDs
{
  _CubeSerial.write(HW_START_MSG);
  _CubeSerial.write(_ledData, LED_DATA_SIZE);
  _CubeSerial.write(HW_END_MSG);
}

void MD_Cubo_STC::setVoxel(uint32_t color, uint8_t x, uint8_t y, uint8_t z)
{
  if ((x >= CUBE_SIZE) || (y >= CUBE_SIZE) || (z >= CUBE_SIZE))
    return;

  // the xy zero point needs to be brought to the front of the cube
  y = CUBE_SIZE - 1 - y;

  // Offsets for position in the array
  x *= 12; y *= 48; z *= 3;

  _ledData[x + y + z + 0] = R(color);
  _ledData[x + y + z + 1] = G(color);
  _ledData[x + y + z + 2] = B(color);
}

uint32_t MD_Cubo_STC::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  if ((x >= CUBE_SIZE) || (y >= CUBE_SIZE) || (z >= CUBE_SIZE))
    return(VOX_OFF);

  // the xy zero point needs to be brought to the front of the cube
  y = CUBE_SIZE - 1 - y;

  // Offsets for position in the array
  x *= 12; y *= 48; z *= 3;

  return(RGB(_ledData[x + y + z + 0], _ledData[x + y + z + 1], _ledData[x + y + z + 2]));
}
