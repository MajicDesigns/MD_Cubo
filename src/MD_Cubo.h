/**
\mainpage Arduino LED Cube Library
LED Cubes
---------

The MD_Cubo library implements methods and a framework that enable the software elements of 
any single color LED cube to be easily implemented. This allows the programmer to use the 
LED matrix as a pixel device addressable through XYZ cartesian coordinates, displaying 
graphics elements much like any other pixel addressable display.

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
- A 'set and forget' model. In this model the pixel is set by the Arduino and some other hardware 
ensures that the appropriate LEDs are turned on.

The software must take into account these differences and allow both to work using the same basic 
software pattern. At a fundamental level, the hardware dependent code required needs to do the following:
- Initialise the hardware
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
- \subpage pageICSTATION4x4
- \subpage pagePAULRB4x4
- \subpage pageJOLLICUBE8x8

Revision History 
----------------

Feb 2016 - version 1.1
- Added jolliCube - first 8x8x8 cube
- Reorganised library
- Reorganised and expanded examples
- Added text based examples, font defintion file and related utility functions

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

/**
 * \file
 * \brief Main header file for the MD_Cubo library
 */

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))		///< Generic macro to return number of array elemenmts
#define MAX_INTENSITY 0xff		///< Maximum intensity for a cube; setting is 0..MAX_INTENSITY

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
		XAXIS,	///< X axis
		YAXIS,	///< Y axis
		ZAXIS	///< Z axis (always vertical, starting at the bottom) 
	};

	/**
	 * Plane name enumerated type.
	 *
	 * This enumerated type is used to define a specific cartesian plane
	 */
	enum plane_t
	{
		XYPLANE,	///< Cartesian plane bounded by XY axes
		XZPLANE,	///< Cartesian plane bounded by XZ axes
		YZPLANE	  ///< Cartesian plane bounded by YZ axes
	};

 /** 
   * Class Constructor.
   *
   * Instantiate a new instance of the class. The parameters passed are used to 
   * initialise the object. Multiple instances may co-exist  provided the virtual
   * cube functions can do so also.
   * 
   * \param sizeCube	the number of LEDs on each side of the cube.
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
   * Initialise the object data. This needs to be called during setup() to initialise new 
   * data for the class that cannot be done during the object creation.
   *
   * Hardware interfaces implemented in virtual functions should be initialized here.
   */
  virtual void begin(void) {};

 /** 
   * Set the specified point in the cube.
   * 
   * This is the most basic graphic function and is necessarily only implementable in 
   * the user derived object, as it is heavily related to the hardware configuration.
   * The (x,y,z) coordinate for the pixel needs to be mapped to a device action to turn the 
   * pixel on or off.
   *
   * \param p     pixel value - if false, set the pixel off. If true, set the pixel on.
   * \param x     x coordinate for the pixel.
   * \param y     y coordinate for the pixel.
   * \param z     z coordinate for the pixel.
   * \return No return value.
   */
  virtual void setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z) {};

 /** 
   * Get the status of specified pixel in the cube.
   * 
   * This is the most basic graphic function and is necessarily only implementable in 
   * the user derived object, as it is heavily related to the hardware configuration.
   * The (x,y,z) coordinate for the pixel needs to be mapped to a device address and 
   * on/off value of the pixel returned to the calling program..
   *
   * \param x     x coordinate for the pixel.
   * \param y     y coordinate for the pixel.
   * \param z     z coordinate for the pixel.
   * \return pixel value - if false if the pixel is off. true if the pixel on.
   */
  virtual boolean getVoxel(uint8_t x, uint8_t y, uint8_t z) {};

 /** 
   * Set the intensity/brightness of the cube.
   * 
   * Set the overall brightness of the cube. This is only implementable in the user 
   * derived object, as it is heavily reliant on the hardware configuration.
   *
   * Brightness is supplied as a number 0 to MAX_INTENSITY (0 is darkest). If the
   * hardware has a different range this will need to be remapped in the user derived 
   * object.
   *
   * \param intensity the intensity for the cube (0 .. MAX_INTENSITY).
   * \return No return value.
   */
  virtual void setIntensity(uint8_t intensity) {};

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
   * \param	axis	specifies the axis required, one of the axis_t enumerations
   * \return size of the cube..
   */
  virtual uint8_t size(axis_t axis) { return 0; };

  /** @} */
  //--------------------------------------------------------------
  /** \name Methods for device-independent graphics.
   * @{
   */
  /** 
   * Clear the cube.
   * 
   * Set every pixel in the cube to the specificed value (default off). The generic method 
   * iterates through all the pixels. A more efficient implementation may be possible by 
   * exploiting access 
   * to hardware.
   *
   * \param p set ON if true, off if false (default).
   * \return No return value.
   */
  virtual void clear(boolean p = false);
 
  /** 
   * Fill the specified plane.
   * 
   * One slice of LEDs (a square) is turned on or off at the specified coordinate 
   * value. The coordinate relates to the intersection point between the plane 
   * and the axis specified. For example, if plane is XYPLANE, coord is the z 
   * value through which the plane passes.
   *
   * \param p         pixel value - if false, set the pixel off. If true, set the pixel on.
   * \param plane the plane this applies to. One of the plane_t value.
   * \param coord axis intersection coordinate value.
   * \return No return value.
   */
  virtual void fillPlane(boolean p, plane_t plane, uint8_t coord);

  /**
  * Copy the specified plane.
  *
  * One slice of LEDs (a square) is copied from the plane coordinate cordFrom 
  * to the plane coordinate cordTo. The coordinate relates to the intersection 
  * point between the plane and the axis specified. For example, if plane is 
  * XYPLANE, coord is the z value through which the plane passes.
  *
  * \param plane the plane this applies to. One of the plane_t value.
  * \param cordFrom the source coordinate value.
  * \param cordTo the destination coordinate value.
  * \return No return value.
  */
  virtual void copyPlane(plane_t plane, uint8_t cordFrom, uint8_t cordTo);

  /**
   * Draw an arbitrary line in 3D space.
   * 
   * The line is drawn from the start to the end coordinates using Bresenham's 
   * line drawing algorithm in 3D. The line can be drawn 'on' or 'off' by specifying 
   * the pixel value, p.
   *
   * \param p       pixel value - if false, set the pixel off. If true, set the pixel on.
   * \param x1      x coordinate for the start point.
   * \param y1      y coordinate for the start point.
   * \param z1      z coordinate for the start point.
   * \param x2      x coordinate for the end point.
   * \param y2      y coordinate for the end point.
   * \param z2      z coordinate for the end point.
   * \return No return value.
   */
  virtual void drawLine(boolean p, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2);

  /**
  * Draw an arbitrary rectangular prism in 3D space.
  *
  * The rectangular prism is drawn from the start coordinates with to the diagonally 
  * opposite corner in 3D space. The line can be drawn 'on' or 'off' by specifying
  * the pixel value, p.
  * A cube is a rectangular prism with the same dx, dy, dz
  * A rectangle (2D) is a rectangular prism with one of dx, dy or dz set to 0
  *
  * \param p       pixel value - if false, set the pixel off. If true, set the pixel on.
  * \param x       x coordinate for one corner.
  * \param y       y coordinate for one corner.
  * \param z       z coordinate for one corner.
  * \param dx      x delta for diagonally opposite point.
  * \param dy      y delta for diagonally opposite point.
  * \param dz      z delta for diagonally opposite point.
  * \return No return value.
  */
  virtual void drawRPrism(boolean p, uint8_t x, uint8_t y, uint8_t z, int8_t dx, uint8_t dy, uint8_t dz);

  /**
  * Draw the outline of a cube in 3D space.
  *
  * The cube is a rectangular prism with same dx, dy, dz, so this is a wrapper 
  * for the more generic case.
  *
  * \param p       pixel value - if false, set the pixel off. If true, set the pixel on.
  * \param x       x coordinate for one corner.
  * \param y       y coordinate for one corner.
  * \param z       z coordinate for one corner.
  * \param size    The size of one side of the cube in pixels.
  * \return No return value.
  */
  inline void drawCube(boolean p, uint8_t x, uint8_t y, uint8_t z, int8_t size) { drawRPrism(p, x, y, z, size-1, size-1, size-1); };

  /** @} */

protected:
  uint8_t _sizeXaxis;  ///< the number of LEDs on x axis of the cube. Total number of pixels is x*y*z.
  uint8_t _sizeYaxis;  ///< the number of LEDs on y axis of the cube. Total number of pixels is x*y*z.
  uint8_t _sizeZaxis;  ///< the number of LEDs on z axis of the cube. Total number of pixels is x*y*z.
};

#endif