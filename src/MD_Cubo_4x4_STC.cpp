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

void MD_Cubo_STC::begin()
{
  byte i;

  _CubeSerial.begin(_bps);
  for (i = 0; i < 50; i++)
  {
    _CubeSerial.write(HW_INIT);
    delay(100);
  }
  this->clear();
  this->update();
}

void MD_Cubo_STC::update()
// Update the cube LEDs
// ### MC20180211 WHAT HAPPENS IF THE RGB VALUES INCLUDES A BYTE WITH SAME
// VALUE AS HW_END_MSG??
// ### CS20180211 It is probably ignored because it needs the full 192 bytes before it will acknowledge the end message
{
  _CubeSerial.write(HW_START_MSG);
  _CubeSerial.write(_columns, COLUMN_COUNT);
  _CubeSerial.write(HW_END_MSG);
}

void MD_Cubo_STC::setVoxel(uint32_t color, uint8_t x, uint8_t y, uint8_t z)
{
  if ((x > CUBE_SIZE-1) || (y > CUBE_SIZE-1) || (z > CUBE_SIZE-1))
    return;

  _columns[x*12 + y*48 + z*3] = R(color);
  _columns[x*12 + y*48 + z*3 + 1] = G(color);
  _columns[x*12 + y*48 + z*3 + 2] = B(color);
}

uint32_t MD_Cubo_STC::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  if ((x > CUBE_SIZE) || (y > CUBE_SIZE) || (z > CUBE_SIZE))
    return(VOX_OFF);

  return(RGB(_columns[x*12+y*48+z*3], _columns[x*12+y*48+z*3+1], _columns[x*12+y*48+z*3+2]));
}
