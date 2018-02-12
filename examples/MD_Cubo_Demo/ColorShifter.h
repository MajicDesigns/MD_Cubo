/**
\mainpage Arduino LED Cube Library
LED Cubes
---------

The MD_Cubo library implements methods and a framework that enable the software elements of 
any single color LED cube to be easily implemented. This allows the programmer to use the 
LED matrix as a voxel device addressable through XYZ cartesian coordinates, displaying 
graphics elements much like any other voxel addressable display.

The Library
-----------
The library implements functions that allow LED cubes to be abstracted in software so that 
underlying hardware changes do not affect the definition and structure of the controlling code.

In this scenario, it is convenient to abstract out the concept of the hardware device and 
create a uniform and consistent cartesian (XYZ) voxel address space, with the libraries 
determining device-element address for each LED. Similarly, control of the devices should 
be uniform and abstracted to a system level.

There appears to be 2 major architectures for implementing cubes:

- A scanning refresh model. In this model the LEDs are refreshed by the Arduino at a rate fast 
enough to activate a persistence of vision (POV) effect.
- A 'set and forget' model. In this model the voxel is set by the Arduino and some other hardware 
ensures that the appropriate LEDs are turned on.

The software must take into account these differences and allow both to work using the same basic 
software pattern. At a fundamental level, the hardware dependent code required needs to do the following:
- Initialize the hardware
- Turn on specific LED (voxel) on or off
- Update the display (internal buffers to actual device)
- Animate the display (ignored for 'set and forget', refresh for scanning)

The standard library supports several different types of cubes, listed below. To 
add a new type of hardware architecture, refer to \subpage pageAddHardware

A word about Axes
-----------------
To enable user code to work properly a convention related to the orientation of the XYZ axes needs 
to be implemented in the hardware dependent code. 

Decide on a 'bottom' corner to be the origin (0,0,0). This will usually be one of the corners just 
above the supporting PCB. Orient the cube so one face of the cube is directly towards you and the 
origin is located in the lower left hand corner.

![MD_Cubo Axes Convention] (MD_Cubo_Axes.jpg "MD_Cubo Axes Convention")

From this reference point
- the Z axis is the vertical axis up the cube.
- the Y axis is the left to right axis you can see.
- the X axis is the axis in the 'depth' direction of the cube.

Hardware Supported
------------------
- \subpage pageICSTATION4x4x4
- \subpage pagePAULRB4x4x4
- \subpage pageJOLLICUBE8x8x8
- \subpage pageZIFFRADIY4x4x4

Revision History 
----------------
Feb 2018 - version 2.0.0
- Added color cube support
- Add Ziffra 4x4x4 color cube based on STC15F2K60S2 with serial interface

Feb 2016 - version 1.1
- Added jolliCube - first 8x8x8 cube
- Reorganized library
- Reorganized and expanded examples
- Added text based examples, font definition file and related utility functions

Aug 2015 - version 1.0
- First release

Copyright
---------
Copyright (C) 2015 Marco Colli. All rights reserved.

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
 * \file
 * \brief Main header file for the MD_Cubo library
 */

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))    ///< Generic macro to return number of array elemenmts

#define RGB(r,g,b) ((((uint32_t)r<<16)+((uint32_t)g<<8)+b) & 0xffffff) ///< create RGB integer from components
#define R(c)       ((uint8_t)((c >>16) & 0xff))  ///< extract R component of RGB
#define G(c)       ((uint8_t)((c >> 8) & 0xff))  ///< extract G component of RGB
#define B(c)       ((uint8_t)(c & 0xff))         ///< extract B component of RGB


/**
 * Core object for the ColorShifter library
 */
class ColorShifter
{
    float _stepWidth;
    float _currentR;
    float _currentG;
    float _currentB;
    float _nextR;
    float _nextG;
    float _nextB;
    uint32_t[] _shiftColors;
    boolean _backwards;
    boolean _ascending;
    uint8_t _finishMode;  // 0 = once , 1 = Wrap around , 2= reverse
    uint8_t _nextColorIndex;
    
public:
    uint32_t getCurrentColor(void) {};

 /** 
   * Class Constructor.
   *
   * Instantiate a new instance of the class. The parameters passed are used to 
   * initialise the object. 
   * 
   * \param sizeCube    the number of LEDs on each side of the cube.
   */
  ColorShifter(uint32_t[] shiftColors, float[] stepWidths, uint8_t finishMode) {};

  boolean isDone;
  /** 
   * Class Destructor.
   *
   * Released any allocated memory and does the necessary to clean up once the object is
   * no longer required.
   */
  ~ColorShifter() {};

  //--------------------------------------------------------------
  /** \name Methods for hardware control.
   * @{
   */
 /** 
   * Shift the colors and return the result.
   *
   */
   uint32_t shift(void) {};

}