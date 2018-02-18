/**
  Arduino RGB Color Shifter Class 

  Revision History
  ----------------
  Feb 2018 - version 1.0.0
  - First release

  Copyright
  ---------
  Copyright (C) 2017 Carsten Schmitz. All rights reserved.

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

#include <Arduino.h>

/**
   \file
   \brief Main file for the ColorShifter class
*/
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))    ///< Generic macro to return number of array elements

#define RGB(r,g,b) ((((uint32_t)r<<16)+((uint32_t)g<<8)+(uint32_t)b) & 0xffffff) ///< create RGB integer from components

#define R(c) ((uint8_t)((c >>16) & 0xff))  ///< extract R component of RGB
#define G(c) ((uint8_t)((c >> 8) & 0xff))  ///< extract G component of RGB
#define B(c) ((uint8_t)(c & 0xff))         ///< extract B component of RGB

/**
 Core object for the ColorShifter class
*/
class ColorShifter
{
  private:
  float _stepWidth;   ///< 
  float _currentR;    ///<
  float _currentG;    ///<
  float _currentB;    ///<
  float _nextR;       ///<
  float _nextG;       ///<
  float _nextB;       ///<
  uint32_t* _shiftColors;
  boolean _backwards;
  boolean _ascending;
  uint8_t _finishMode;  // 0 = once , 1 = Wrap around , 2= reverse
  uint8_t _nextColorIndex;
  uint8_t _colorCount;
  boolean _isDone;

  static float shiftSingleColor(float current, float next, float width);

  public:
  uint32_t getCurrentColor(void) {};

  /**
   Class Constructor.

   Instantiate a new instance of the class. The parameters passed are used to
   initialize the object.

   \param shiftColors  An array of colors the actual RGB value is shifted towards to. Once the next color is reached, the next color in the array is set as destination
   \param shiftColorCount  The number of colors in the array
   \param stepWidth        The step width which all 3 colors are moved along
   \param finishMode       This determines what happens if the last color in the array is reached: 
      Set to 0 to end the shifting - Property _isDone will be set to true after that.
      Set to 1 to move backward in the array (bounce)
      Set to 2 to set the first color as next color (loop)
   \return the new RGB color
  */
  ColorShifter(uint32_t* shiftColors, uint8_t shiftColorCount, float stepWidth, uint8_t finishMode);
  
  /**
     Class Destructor.

     Released any allocated memory and does the necessary to clean up once the object is
     no longer required.
  */
  ~ColorShifter() {};

  /**
   Shift the color by step width.

   \return the new RGB color resulting from the shift.
  */
  uint32_t shift(void);

  /**
   Raises or lowers brightness of a RGB color

   \param Current color
   \param The step width
   \return the new RGB color.
   */
  static uint32_t dim(uint32_t color, float stepsize);
};
