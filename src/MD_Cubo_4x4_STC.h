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
\page pagePAULRB4x4 PaulRB DIY Implementation
PaulRB 4x4 Cube
---------------
Source: Arduino Forum "4x4x4 LED Cube with ATtiny85 and MAX7219" at http://forum.arduino.cc/index.php?topic=226262.0

![PaulRB 4x4 Cube] (PaulRB_image.jpg "PaulRB 4x4 Cube")

The PaulRB cube is implemented as a single sided PCB for DIY projects. It has a single MD_MAX7219 IC with an SPI 
interface to the Arduino controller or an ATTiny85 on the board itself, and is therefore a 'set and forget' type 
of device.

The hardware architecture implemented is shown in the figure below. Essentially, the 7219 IC's 64 bits 
(8 Digits x 8 Segments) are remapped into the 64 LEDs of the cube in a non-linear manner.

![PaulRB Cube Schematic] (PaulRB_Schematic.png "PaulRB Cube Schematic")

###Implementation Overview###
The software implements an SPI interface through the standard Arduino SPI object.

Cube data is buffered in memory organised as the digits of the 7219 IC (8 bytes). The setVoxel() function uses a 
lookup table to find the bits representing the XYZ position for each LED in the cube. The cube data is held in 
memory buffers until an update() call is made, at which point it is entirely written to the cube through the SPI 
interface.

The intensity values 0..255 are remapped to 0..15 levels available in the MAX7219 IC (divide by 16).  
*/


#define CUBE_SIZE 4		///< Cube size in the X, Y and Z axis
#define MAX_INTENSITY 254
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
  uint8_t _columns[COLUMN_COUNT]; ///< Holds the current bit pattern for each layer of the cube
  uint8_t _intensity;  // Can hold an RGB value
  
};

#endif