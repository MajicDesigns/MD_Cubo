#ifndef MD_CUBO_NxN_DIRECTIO_H
#define MD_CUBO_NxN_DIRECTIO_H

/**
 * \file
 * \brief Header file for the MD_Cubo Direct I/O object
 */

#define	DEBUG_DIRECTIO 0   ///< Enable or disable (default) debugging output from the example

#if DEBUG_DIRECTIO
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
\page pageDIRECTIO Direct I/O Implementation
Direct I/O nxnxn Cube
---------------------
This cube only requires a handful of discrete components a little bit of 
soldering to make the cube which fits on the breadboard.

See description here: https://github.com/MajicDesigns/MD_Cubo/issues/11

I/O mapping (Arduino Nano and 3x3x3 cube example)
-------------------------------------------------

3 transistors (A949 PNP BJT's) are used to open each layer circuit to ground.
- 5V from power rail to pin 1 on the three BJT's direct connection
- 1K ohm resistor on BJT pin 2 to cube layer 0 common cathode
- 1K ohm resistor on BJT pin 2 to cube layer 1 common cathode
- 1K ohm resistor on BJT pin 2 to cube layer 2 common cathode

- D13 on Arduino to a 100 ohm resistor to pin 3 on the layer 0 BJT
- D12 on Arduino to a 100 ohm resistor to pin 3 on the layer 1 BJT
- D11 on Arduino to a 100 ohm resistor to pin 3 on the layer 2 BJT

LEDs in the layer are numbered by column:

    2  5  8
    1  4  7
    0  3  6

LED in each layer have a direct connection to Arduino digital I/O as follows:

    LED  0   1   2   3   4   5   6   7   8
    Pin  D4  D7  D10 D3  D6  D9  D2  D5  D8

The hardware architecture implemented shown in the schematic below.

![Direct I/O nxn schematic] (DirectIO_Schematic.jpg "Direct I/O nxn Schematic")

###Implementation Overview###
Cube data is buffered in memory organized as vertical layers of 9 bits (1 word). At each refresh,
the I/O for each LED in a layer (_LEDLayer) is set and the Arduino pin corresponding to that layer 
(_enableLayer) is set to ground (LOW) to enable the circuit for that layer.

The animate() call is used to scan the refresh of the cube. No interrupt timers are used in this implementation.

No intensity changes are implemented in this architecture.
*/

const uint8_t LAYER_TIME = 5;   ///< Refresh time for each layer, in milliseconds
const uint8_t CUBE_SIZE = 3;    ///< Cube size in the X, Y and Z axis

class MD_Cubo_DirectIO : public MD_Cubo
{
public:
  MD_Cubo_DirectIO() : MD_Cubo(CUBE_SIZE), _curLayer(0) { return; };
  ~MD_Cubo_DirectIO() { return; };

  void begin();
  void update();
  void animate(uint32_t wait = 0);

  void setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z);
  uint32_t getVoxel(uint8_t x, uint8_t y, uint8_t z);

  void clear(uint32_t p = VOX_OFF)
  {
    memset(_scratch.count, (p == VOX_OFF ? 0 : CUBE_SIZE * CUBE_SIZE), CUBE_SIZE);
    memset(_scratch.data, (p == VOX_OFF ? 0 : 0xff), CUBE_SIZE);
  };

  uint8_t size(axis_t axis) { return CUBE_SIZE; };
    
  private:
  typedef struct 
  {
    uint8_t  count[CUBE_SIZE];// the count of points ON per layer
    uint16_t data[CUBE_SIZE]; // the bit pattern for each layer of the cube
  } dispData_t;
  
  dispData_t  _current;
  dispData_t  _scratch;

  const uint8_t   _enableLayer[CUBE_SIZE] = { 13, 12, 11 };   // pins to enable each layer
  const uint8_t   _LEDLayer[CUBE_SIZE][CUBE_SIZE] = {{4, 7, 10}, {3, 6, 9}, {2, 5, 8} };   // pins for each LED on a layer
  uint8_t   _pwmOffset;
    
  uint8_t   _curLayer;  // currently displayed layer
  uint32_t  _timeLayer; // layer display start time

  void setIO(uint8_t layer);
};

#endif