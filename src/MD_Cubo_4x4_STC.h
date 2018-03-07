#ifndef MD_CUBO_4x4_STC_H
#define MD_CUBO_4x4_STC_H

#include <SoftwareSerial.h>
/**
 * \file
 * \brief Header file for the MD_Cubo STC class
 */

#define	DEBUG_STC 0   ///< Enable or disable (default) debugging output from the example

#if DEBUG_STC
    #define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }      ///< Print a string followed by a value (decimal)
    #define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } ///< Print a string followed by a value (hex)
    #define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } ///< Print a string followed by a value (binary)
    #define PRINTS(s)     { Serial.print(F(s)); }                       ///< Print a string
#else
    #define PRINT(s, v)   ///< Print a string followed by a value (decimal)
    #define PRINTX(s, v)  ///< Print a string followed by a value (hex)
    #define PRINTB(s, v)  ///< Print a string followed by a value (binary)
    #define PRINTS(s)     ///< Print a string
#endif

/**
\page pageZIFFRADIY4x4x4 Zirrfa DIY Implementation
Zirrfa 4x4 Cube
---------------
Source: AliExpress "Zirrfa 4x4x4 RGB LED Cube" at https://www.aliexpress.com/item/NEW-3D-4X4X4-RGB-cubeeds-Full-Color-LED-Light-display-Electronic-DIY-Kit-3d4-4-4/32793425203.html

![Zirrfa 4x4x4 Cube] (Zirrfa_image.jpg "Zirrfa 4x4x4 Cube")

The Zirrfa RGB color cube can be bought online as a DIY soldering set. It has a STC15F2K60S2 IC with a SPI 
interface to the Arduino controller and is therefore a 'set and forget' type of device.

Note: To use this implementation you need to install the SoftwareSerial Arduino library available at https://www.arduino.cc/en/Reference/SoftwareSerial 
The RX and TX pins selected for communications should be passed to the class constructor.

###Implementation Overview###
The software implements an SPI interface through the standard Arduino SPI object.

Cube data is buffered in memory in a serial 192 byte string, which represents the 4x4x4x3 bytes 
(the dimensions of the cube LEDs multiplied with 3 byte values (RGB) to set the color)
The cube data is held in memory buffers until an update() call is made, at which point it is 
entirely written to the cube through the SPI interface.

###Serial Communications Protocol###
Serial port runs at 57600 bps.
When the cube is powered up there is a 5 second delay during which commands can be received 
from the serial port. If the cube is not initialized within the delay period it goes into demo mode.
To initialize the cube send 0xAD.
To update the cube send 0xEA + 192 bytes of LED/color data + 0xEB to finish.
192 bytes = 4 x 4 x 4 x 3 RGB values.
*/

const uint8_t CUBE_SIZE = 4;        ///< Cube size in the X, Y and Z axis;
const uint8_t LED_DATA_SIZE = (CUBE_SIZE * CUBE_SIZE * CUBE_SIZE * 3); ///< LEDs x 3 RGB color values

const uint8_t HW_INIT = 0xad;       ///< Hardware initialization into serial mode. Send within 5 seconds of HW reset.
const uint8_t HW_START_MSG = 0xea;  ///< Hardware start serial message packet.
const uint8_t HW_END_MSG = 0xeb;    ///< Hardware end serial message packet. Repeat once at end of packet.

class MD_Cubo_STC: public MD_Cubo
{
  public:
  MD_Cubo_STC(uint8_t tx, uint8_t rx, uint32_t bps): _tx(tx), _rx(rx), _bps(bps), MD_Cubo(CUBE_SIZE), _CubeSerial(_tx, _rx) { };
  ~MD_Cubo_STC() { };

  void begin();
  void update();

  void setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z);
  uint32_t getVoxel(uint8_t x, uint8_t y, uint8_t z);
  void setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z) { setVoxel(p ? VOX_ON : VOX_OFF, x, y, z); };

  void clear(uint32_t p = VOX_OFF);
  uint8_t size(axis_t axis) { return CUBE_SIZE; };
  boolean isColorCube () { return true; }

  private:
  uint8_t _tx, _rx;   ///< SoftwareSerial parameters - not sure if we need them!
  uint32_t _bps;      ///< Software Serial comms speed.
  SoftwareSerial _CubeSerial;  ///< Serial comms through this object
  uint8_t _ledData[LED_DATA_SIZE]; ///< Holds the current color pattern for all LEDs of the cube
};

#endif