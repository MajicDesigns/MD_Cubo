#ifndef MD_CUBO_8x8_JC_H
#define MD_CUBO_8x8_JC_H

/**
 * \file
 * \brief header file for the MD_Cubo jolliCube object
 */

#define	DEBUG_JC  0   ///< Enable or disable (default) debugging output from the example

#if DEBUG_JC
#define	PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }		  ///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); }	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); }	///< Print a string followed by a value (binary)
#define	PRINTS(s)     { Serial.print(F(s)); }							          ///< Print a string
#else
#define	PRINT(s, v)   ///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)  ///< Print a string followed by a value (hex)
#define	PRINTB(s, v)  ///< Print a string followed by a value (binary)
#define	PRINTS(s)     ///< Print a string
#endif

/**
\page pageJOLLICUBE8x8x8 jolliCube Implementation
JolliCube 8x8x8 Cube
--------------------
Reference: jolliCube by jollifactory "JolliCube - an 8x8x8 LED Cube (SPI)" at http://www.instructables.com/id/JolliCube-an-8x8x8-LED-Cube-SPI/

![jolliCube 8x8 Cube] (JolliCube_image.jpg "jolliCube 8x8 Cube")

The monochrome jolliCube is implemented using 8 MD_MAX7219 ICs with an SPI interface to the Arduino 
controller, and is therefore a 'set and forget' type of device. The MAX7219 ICs are connected in 
series with the serial output of one device being the input to the next. 

The hardware architecture implemented is shown in the figure below.

![jolliCube 7219 hardware mapping] (jolliCube_hardware_Map.jpg "jolliCube hardware mapping")

- Each 7219 IC controls one vertical plane of the cube (64 LEDs). The software maps these so that the
first device is at the front of the display and the X axis increases in the direction of the devices 
(ie, the X coordinate is the device number)
- Each 7219 digit is implemented as a row on a plane (Z axis) incrementing down the cube. The software 
remaps these to be increasing up from the base (ie, the lowest row near the PCb is Z = 0).
- Each segment is implemented with bits DP, A, ... F from the right face to the left. The software maps
these with Y coordinate 0 on the left.

###Implementation Overview###
The software implements an SPI interface through the standard Arduino SPI object.

Cube data is buffered in memory organized as vertical layers of 64 bits (8 bytes) corresponding to the data
for each 7219 IC, referenced to provide an origin point on the lower left corner of the cube. The cube data 
is held in memory buffers until an update() call is made, at which point it is entirely written to the cube 
through the SPI interface.

The intensity values 0..255 are remapped to 0..15 levels available in the MAX7219 IC (divide by 16).  
*/

// Pins for SPI comm with the MAX7219 IC
const uint8_t DATA = 11;  ///< SPI Data pin number
const uint8_t CLK  = 13;  ///< SPI Clock pin number
const uint8_t LOAD = 10;  ///< SPI Load pin number

const uint8_t CUBE_XSIZE  = 8;  ///< Cube size in the X axis
const uint8_t CUBE_YSIZE  = 8;  ///< Cube size in the Y axis
const uint8_t CUBE_ZSIZE  = 8;  ///< Cube size in the Z axis
const uint8_t MAX_DEVICES = CUBE_XSIZE;	///< Number of 7219 devices is the same as X axis size

// Relevant MAX7219 Control Registers
const uint8_t INTENSITY   = 0x0A; ///< MAX7219 intensity control register address
const uint8_t ON_OFF      = 0x0C; ///< MAX7219 on/off control register address
const uint8_t DECODE      = 0x09; ///< MAX7219 BCD decode control register address
const uint8_t SCAN_DIGITS = 0x0B; ///< MAX7219 scanned digits control register address
const uint8_t TEST_MODE   = 0x0F; ///< MAX7219 test mode control register address
const uint8_t DIGIT0      = 0x01; ///< MAX7219 DIGIT 0 register address. Digits 1..9 consecutive after this.

class MD_Cubo_JC: public MD_Cubo
{
  public:
  MD_Cubo_JC(): MD_Cubo(CUBE_XSIZE), _data(DATA), _clock(CLK), _load(LOAD) {return;};
  ~MD_Cubo_JC() {return;};

  void begin();
  void update();

  void setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z);
  uint32_t getVoxel(uint8_t x, uint8_t y, uint8_t z);

  void setIntensity(uint8_t intensity) { sendSPI(INTENSITY, intensity >> 4); MD_Cubo::setIntensity(intensity); };
  void clear(uint32_t p = VOX_OFF) { memset(_layer, (p == VOX_OFF ? 0 : 0xff), sizeof(_layer[0][0]) * CUBE_ZSIZE * MAX_DEVICES); };
  uint8_t size(axis_t axis);
  
  private:
  uint8_t   _data;    ///< SPI Data pin for transmission
  uint8_t   _clock;   ///< SPI Clock pin for transmisison
  uint8_t   _load;    ///< SPI Load pin for transmisison
  uint8_t 	_layer[MAX_DEVICES][CUBE_ZSIZE]; ///< Holds the current bit pattern for each layer of the cube.
  
  void sendSPI(uint8_t reg, uint8_t val);
};

#endif