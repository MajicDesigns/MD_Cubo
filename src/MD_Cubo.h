/**
\mainpage Arduino LED Cube Library
LED Cubes
---------

The MD_Cubo library implements methods and a framework that enable the software elements of 
any monochrome or color LED cube to be easily implemented. This allows the programmer to 
use the LED matrix as a voxel device addressable through XYZ cartesian coordinates, displaying 
graphics elements much like any other voxel addressable display.

The Library
-----------
The library implements functions that allow LED cubes to be abstracted in software so that 
underlying hardware changes do not affect the definition and structure of the controlling code.

In this scenario, it is convenient to abstract out the concept of the hardware device and 
create a uniform and consistent 3D cartesian (XYZ) voxel address space, with the libraries 
determining device-element address for each LED. Similarly, control of the devices should 
be uniform and abstracted to a system level.

There appears to be 2 major architectures for implementing cubes:

-# A 'scanning refresh' model. In this model the LEDs are refreshed by the Arduino at a rate fast 
enough to activate a persistence of vision (POV) effect.
-# A 'set and forget' model. In this model the voxel is set by the Arduino and some other hardware 
ensures that the appropriate LEDs are turned on.

The software must take into account these differences and allow both to work using the same basic 
software pattern. At a fundamental level, the hardware dependent code required needs to do the following:
- Initialize the hardware
- Turn on specific LED (voxel) on or off in the specified color
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

Cube Hardware Supported
-----------------------
- \subpage pageICSTATION4x4x4
- \subpage pagePAULRB4x4x4
- \subpage pageJOLLICUBE8x8x8
- \subpage pageZIFFRADIY4x4x4

Revision History 
----------------
Jul 2018 - version 2.0.1
- Fixed setVoxel issue with 595 cube

Mar 2018 - version 2.0.0
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
#ifndef MD_CUBO_H
#define MD_CUBO_H

#include <Arduino.h>
#include "ColorDefs.h"

/**
 * \file
 * \brief Main header file for the MD_Cubo library
 */

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))    ///< Generic macro to return number of array elemenmts
#define MAX_INTENSITY 0xff    ///< Maximum intensity for a cube; setting is 0..MAX_INTENSITY

/**
 * Core object for the MD_Cubo library
 */
class MD_Cubo
{
public:
  /**
  * Axis name enumerated type.
  *
  * This enumerated type is used to specify a cartesian axis
  */
  enum axis_t
  {
    XAXIS,  ///< X axis
    YAXIS,  ///< Y axis
    ZAXIS   ///< Z axis (always vertical, starting at the bottom) 
  };

  /**
  * Plane name enumerated type.
  *
  * This enumerated type is used to define a specific cartesian plane
  */
  enum plane_t
  {
    XYPLANE,  ///< Cartesian plane bounded by XY axes
    XZPLANE,  ///< Cartesian plane bounded by XZ axes
    YZPLANE   ///< Cartesian plane bounded by YZ axes
  };

 /** 
   * Class Constructor.
   *
   * Instantiate a new instance of the class. The parameters passed are used to 
   * initialize the object. Multiple instances may co-exist provided the virtual
   * cube functions can do so also.
   * 
   * \param sizeCube the number of LEDs on each side of the cube.
   */
  MD_Cubo(uint8_t sizeCube): _sizeXaxis(sizeCube), _sizeYaxis(sizeCube), _sizeZaxis(sizeCube) {};

  /** 
   * Class Destructor.
   *
   * Released any allocated memory and does the necessary to clean up once the object is
   * no longer required.
   */
  ~MD_Cubo() {};

  //--------------------------------------------------------------
  /** \name Methods for hardware control.
   * @{
   */
 /** 
   * Initialize the object.
   *
   * Initialize the object data. This needs to be called during setup() to initialize new 
   * data for the class that cannot be done during the object creation.
   *
   * Hardware interfaces implemented in virtual functions should be initialized here.
   */
  virtual void begin(void) {};

 /** 
   * Set the specified point in the cube (monochrome version).
   * 
   * This method should only be implemented for monochrome cubes and is for backward
   * compatibility only. The preferred implementation is to use the color version for
   * all cubes.
   * 
   * This is the most basic graphic function and is necessarily only implementable in 
   * the user derived object, as it is heavily related to the hardware configuration.
   * The (x,y,z) coordinate for the voxel needs to be mapped to a device action to turn the 
   * voxel on or off.
   *
   * \param p  voxel value - if false, set the voxel off. Otherwise, set the voxel on.
   * \param x  x coordinate for the voxel.
   * \param y  y coordinate for the voxel.
   * \param z  z coordinate for the voxel.
   * \return No return value.
   */
  virtual void setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z) { setVoxel(p ? VOX_ON : VOX_OFF, x, y, z);  };

  /**
  * Set the specified point in the cube.
  *
  * This is the most basic graphic function and is necessarily only implementable in
  * the user derived object, as it is heavily related to the hardware configuration.
  * The (x,y,z) coordinate for the voxel needs to be mapped to a device action to turn the
  * voxel to the specified color in the specified color and intensity.
  *
  * By default all cubes are treated as color cubes. User code should recognize if it is 
  * operating with a color or monochrome cube by testing isColorCube(). For clarity, 
  * monochrome cubes should use the default VOX_ON and VOX_OFF colors for clarity, 
  * color cubes are free to choose any RGB color.
  *
  * Default for this virtual method is to invoke the monochrome method if color is
  * not implemented in user code.
  *
  * \see isColorCube()
  *
  * \param p  voxel RGB value - if VOX_OFF, set the voxel off. Otherwise, set the voxel to the color.
  * \param x  x coordinate for the voxel.
  * \param y  y coordinate for the voxel.
  * \param z  z coordinate for the voxel.
  * \return No return value.
  */
  virtual void setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z) { };
  
  /**
   * Get the status of specified point in the cube.
   * 
   * This is the most basic graphic function and is necessarily only implementable in 
   * the user derived object, as it is heavily related to the hardware configuration.
   * The (x,y,z) coordinate for the voxel needs to be mapped to a device address and 
   * color value of the voxel returned to the calling program.
   *
   * \param x  x coordinate for the voxel.
   * \param y  y coordinate for the voxel.
   * \param z  z coordinate for the voxel.
   * \return voxel RGB value - VOX_OFF if the voxel is off. Otherwise the voxel color value.
   */
  virtual uint32_t getVoxel(uint8_t x, uint8_t y, uint8_t z) {};

 /** 
   * Set the default intensity/brightness of the cube.
   * 
   * Set the default brightness of the cube. This is only implementable in the user 
   * derived object, as it is heavily reliant on the hardware configuration.
   *
   * Brightness is supplied as a number 0 to MAX_INTENSITY (0 is darkest). If the
   * hardware has a different range this will need to be remapped in the user derived 
   * object.
   *
   * \param intensity  the intensity for the cube (0 .. MAX_INTENSITY).
   * \return No return value.
   */
  virtual void setIntensity(uint8_t intensity) { _intensity = intensity; };

  /**
  * Get the default intensity/brightness of the cube.
  *
  * Get the default brightness of the cube. This is only implementable in the user
  * derived object, as it is heavily reliant on the hardware configuration.
  *
  * \return The intensity for the cube (0 .. MAX_INTENSITY).
  */
  uint8_t getIntensity(void) { return(_intensity); };
  
  /**
   * Update the cube display.
   * 
   * All cube changes are buffered internally until this method is called, at which time 
   * the cube is updated. This is only implementable in the user derived object, as it is 
   * heavily reliant on the hardware configuration.
   *
   * \return No return value.
   */
  virtual void update() {};

  /** 
   * Clear the cube.
   * 
   * Set every voxel in the cube to the specified value (default off). The generic method 
   * iterates through all the voxels. A more efficient implementation may be possible by 
   * exploiting access to hardware.
   *
   * \param p  set the the RGB value required, RGB(0,0,0) for all black (default).
   * \return No return value.
   */
  virtual void clear(uint32_t p = VOX_OFF);
 
 /** 
   * Animate the cube display.
   * 
   * Cubes that need to be refreshed/multiplexed in order to show the display should 
   * have the multiplexing code inserted here. The code should be written as a non-blocking
   * state machine that should be executing for _wait_ milliseconds. If _wait_ is zero then 
   * one step in the refresh should be executed and the method needs to be invoke often 
   * enough to ensure animations are seen to be smooth. This is only implementable in 
   * the user derived object, as it is heavily reliant on the hardware configuration.
   *
   * \param wait  optional period for this animation to execute before returning.
   * \return No return value.
   */
  virtual void animate(uint32_t wait = 0) { if (wait != 0) delay(wait); };
    
 /** 
   * Return the size of the cube.
   * 
   * Return the number of LED on one side of the cube. Allows high level code to be 
   * independent of any size definitions, which are known in in the user derived object, 
   * as it is reliant on the hardware configuration.
   *
   * \param axis  specifies the axis required, one of the axis_t enumerations.
   * \return Size of the cube.
   */
  uint8_t size(axis_t axis) { return 0; };

  /**
  * Return whether the cube supports RGB color.
  *
  * Return true if the cube supports RGB colors. This will depend on the hardware
  * implemented, so can be overridden by the user functions. 
  *
  * \return True if RGB color supported.
  */
  virtual bool isColorCube(void) { return(false); };

  /* @} */
  //--------------------------------------------------------------
  /** \name Methods for device-independent graphics.
   * @{
   */
  /** 
   * Fill the specified plane.
   * 
   * One slice of LEDs (a square) is turned on or off at the specified coordinate 
   * value. The coordinate relates to the intersection point between the plane 
   * and the axis specified. For example, if plane is XYPLANE, coord is the z 
   * value through which the plane passes.
   *
   * \param p      voxel RGB value - if VOX_OFF set the voxel off. Otherwise set the voxel to the color.
   * \param plane  the plane this applies to. One of the plane_t value.
   * \param coord  axis intersection coordinate value.
   * \return No return value.
   */
  void fillPlane(uint32_t p, plane_t plane, uint8_t coord);

  /**
  * Copy the specified plane.
  *
  * One slice of LEDs (a square) is copied from the plane coordinate cordFrom 
  * to the plane coordinate cordTo. The coordinate relates to the intersection 
  * point between the plane and the axis specified. For example, if plane is 
  * XYPLANE, coord is the z value through which the plane passes.
  *
  * \param plane     the plane this applies to. One of the plane_t value.
  * \param cordFrom  the source coordinate value.
  * \param cordTo    the destination coordinate value.
  * \return No return value.
  */
  void copyPlane(plane_t plane, uint8_t cordFrom, uint8_t cordTo);

  /**
   * Draw an arbitrary line in 3D space.
   * 
   * The line is drawn from the start to the end coordinates using Bresenham's 
   * line drawing algorithm in 3D. The line can be drawn 'on' or 'off' by specifying 
   * the voxel value, p.
   *
   * \param p   voxel RGB value - if VOX_OFF, set the voxel off. Otherwise, set the voxel to the color.
   * \param x1  x coordinate for the start point.
   * \param y1  y coordinate for the start point.
   * \param z1  z coordinate for the start point.
   * \param x2  x coordinate for the end point.
   * \param y2  y coordinate for the end point.
   * \param z2  z coordinate for the end point.
   * \return No return value.
   */
  void drawLine(uint32_t p, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2);

  /**
  * Draw an arbitrary rectangular prism in 3D space.
  *
  * The rectangular prism is drawn from the start coordinates to the diagonally 
  * opposite corner in 3D space. The line can be drawn in any color by specifying
  * the voxel value, p.
  * A cube is a rectangular prism with the same dx, dy, dz
  * A rectangle (2D) is a rectangular prism with one of dx, dy or dz set to 0
  *
  * \param p   voxel RGB value - if VOX_OFF set the voxel off. Otherwise set the voxel to the color.
  * \param x   x coordinate for one corner.
  * \param y   y coordinate for one corner.
  * \param z   z coordinate for one corner.
  * \param dx  x delta for diagonally opposite point.
  * \param dy  y delta for diagonally opposite point.
  * \param dz  z delta for diagonally opposite point.
  * \return No return value.
  */
  void drawRPrism(uint32_t p, uint8_t x, uint8_t y, uint8_t z, int8_t dx, int8_t dy, int8_t dz);

  /**
  * Draw the outline of a cube in 3D space.
  *
  * The cube is a rectangular prism with same dx, dy, dz, so this is a wrapper 
  * for the more generic case.
  *
  * \param p     voxel RGB value - if VOX_OFF, set the voxel off. Otherwise set the voxel to the color.
  * \param x     x coordinate for one corner.
  * \param y     y coordinate for one corner.
  * \param z     z coordinate for one corner.
  * \param size  the size of one side of the cube in voxels.
  * \return No return value.
  */
  void drawCube(uint32_t p, uint8_t x, uint8_t y, uint8_t z, int8_t size) { drawRPrism(p, x, y, z, size-1, size-1, size-1); };

  /* @} */

protected:
  uint8_t _sizeXaxis;  ///< the number of LEDs on x axis of the cube. Total number of voxels is x*y*z.
  uint8_t _sizeYaxis;  ///< the number of LEDs on y axis of the cube. Total number of voxels is x*y*z.
  uint8_t _sizeZaxis;  ///< the number of LEDs on z axis of the cube. Total number of voxels is x*y*z.
  uint8_t _intensity;  ///< the default intensity for the cube.
};

#endif