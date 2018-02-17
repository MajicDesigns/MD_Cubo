#ifndef MD_CUBO_4x4_72XX_H
#define MD_CUBO_4x4_72XX_H

/**
 * \file
 * \brief Header file for the MD_Cubo 72xx class
 */

#define DEBUG_72XX  0   ///< Enable or disable (default) debugging output from the example

#if DEBUG_72XX
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
\page pagePAULRB4x4x4 PaulRB DIY Implementation
PaulRB 4x4x4 Cube
-----------------
Source: Arduino Forum "4x4x4 LED Cube with ATtiny85 and MAX7219" at http://forum.arduino.cc/index.php?topic=226262.0

![PaulRB 4x4 Cube] (PaulRB_image.jpg "PaulRB 4x4 Cube")

The PaulRB monochrome cube is implemented as a single sided PCB for DIY projects. It has a single MD_MAX7219 IC with an SPI 
interface to the Arduino controller or an ATTiny85 on the board itself, and is therefore a 'set and forget' type 
of device.

The hardware architecture implemented is shown in the figure below. Essentially, the 7219 IC's 64 bits 
(8 Digits x 8 Segments) are remapped into the 64 LEDs of the cube in a non-linear manner.

![PaulRB Cube Schematic] (PaulRB_Schematic.png "PaulRB Cube Schematic")

###Implementation Overview###
The software implements an SPI interface through the standard Arduino SPI object.

Cube data is buffered in memory organized as the digits of the 7219 IC (8 bytes). The setVoxel() function uses a 
lookup table to find the bits representing the XYZ position for each LED in the cube. The cube data is held in 
memory buffers until an update() call is made, at which point it is entirely written to the cube through the SPI 
interface.

The intensity values 0..255 are remapped to 0..15 levels available in the MAX7219 IC (divide by 16).  
*/

// Pins for SPI comm with the MAX7219 IC
const uint8_t DATA = 11;  ///< SPI Data pin number
const uint8_t CLK  = 13;  ///< SPI Clock pin number
const uint8_t LOAD = 10;  ///< SPI Load pin number

const uint8_t CUBE_SIZE = 4;    ///< Cube size in the X, Y and Z axis

// Relevant MAX7219 Control Registers
const uint8_t INTENSITY   = 0x0A; ///< MAX7219 intensity control register address
const uint8_t ON_OFF      = 0x0C; ///< MAX7219 on/off control register address
const uint8_t DECODE      = 0x09; ///< MAX7219 BCD decode control register address
const uint8_t SCAN_DIGITS = 0x0B; ///< MAX7219 scanned digits control register address
const uint8_t TEST_MODE   = 0x0F; ///< MAX7219 test mode control register address
const uint8_t DIGIT0      = 0x01; ///< MAX7219 DIGIT 0 register address. Digits 1..9 consecutive after this.

class MD_Cubo_72xx: public MD_Cubo
{
  public:
  MD_Cubo_72xx(): MD_Cubo(CUBE_SIZE), _data(DATA), _clock(CLK), _load(LOAD) {return;};
  ~MD_Cubo_72xx() {return;};

  void begin();
  void update();

  void setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z);
  uint32_t getVoxel(uint8_t x, uint8_t y, uint8_t z);

  void setIntensity(uint8_t intensity) { sendSPI(INTENSITY, intensity >> 4); MD_Cubo::setIntensity(intensity); };

  void clear(uint32_t p = VOX_OFF) { memset(_layer, (p == VOX_OFF ? 0 : 0xff), sizeof(_layer[0])*CUBE_SIZE); };
  uint8_t size(axis_t axis) { return CUBE_SIZE; };
  
  private:
  uint8_t   _data;    ///< SPI Data pin for transmission
  uint8_t   _clock;   ///< SPI Clock pin for transmission
  uint8_t   _load;    ///< SPI Load pin for transmission
  uint16_t _layer[CUBE_SIZE]; ///< Holds the current bit pattern for each layer of the cube
  
  void sendSPI(uint8_t reg, uint8_t val);
};

#endif