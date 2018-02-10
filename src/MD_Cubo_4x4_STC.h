#ifndef MD_CUBO_4x4_STC_H
#define MD_CUBO_4x4_STC_H

/**
 * \file
 * \brief Header file for the MD_Cubo STC class
 */

#define	DEBUG_STC	0		///< Enable or disable (default) debugging output from the example

#if DEBUG_STC
#define	PRINT(s, v)		{ Serial.print(F(s)); Serial.print(v); }		  ///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)	{ Serial.print(F(s)); Serial.print(v, HEX); }	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)	{ Serial.print(F(s)); Serial.print(v, BIN); }	///< Print a string followed by a value (binary)
#define	PRINTS(s)		  { Serial.print(F(s)); }							          ///< Print a string
#else
#define	PRINT(s, v)		///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)	///< Print a string followed by a value (binary)
#define	PRINTS(s)		  ///< Print a string
#endif

/**
\page Zirrfa DIY Implementation
Zirrfa 4x4 Cube
---------------
Source: AliExpress "Zirrfa 4x4x4 RGB LED Cube" at https://www.aliexpress.com/item/NEW-3D-4X4X4-RGB-cubeeds-Full-Color-LED-Light-display-Electronic-DIY-Kit-3d4-4-4/32793425203.html

![Zirrfa 4x4 Cube] (Zirrfa_image.jpg "Zirrfa 4x4 Cube")

The Zirrfa cube can be bought online as a DIY soldering set. It has a STC15F2K60S2 IC with a SPI 
interface to the Arduino controller and is therefore a 'set and forget' type 
of device.

Note: To use this implementation you need to install the SofSerial Arduino library. 
Connect the cube RX to pin 11 (TX) of the Arduino as this is currently the default serial interface. (This should work with most Arduinos).


###Implementation Overview###
The software implements an SPI interface through the standard Arduino SPI object.

Cube data is buffered in memory in a serial 192 byte string, which represents the 4x4x4x3 bytes 
(the dimensions of the cube LEDs multiplied with 3 byte values (RGB) to set the color)
The cube data is held in memory buffers until an update() call is made, at which point it is 
entirely written to the cube through the SPI interface.

*/


#define CUBE_SIZE 4		///< Cube size in the X, Y and Z axis
#define MAX_INTENSITY 254 // @todo This will be removed at some point
#define COLUMN_COUNT 192 /// 4x4x4 LEDs x 3 RGB color values


class MD_Cubo_STC: public MD_Cubo
{
  public:
  MD_Cubo_STC(): MD_Cubo(CUBE_SIZE) {return;};
  ~MD_Cubo_STC() {return;};
  void begin();
  void update();
  void setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z);
  boolean getVoxel(uint8_t x, uint8_t y, uint8_t z);
  void clear(boolean p = false) { memset(_columns, (p ? 0xff : 0), COLUMN_COUNT); };
  uint8_t size(axis_t axis) { return CUBE_SIZE; };
  
  private:
  uint8_t _columns[COLUMN_COUNT]; ///< Holds the current byte pattern for all LEDs of the cube
  uint8_t _intensity;  // @todo: Convert to RGB value
  
};

#endif