#ifndef MD_CUBO_4x4_ICS595_H
#define MD_CUBO_4x4_ICS595_H

/**
 * \file
 * \brief Main header file for the MD_Cubo ICStation 595 object
 */

#define	DEBUG_595	0		///< Enable or disable (default) debugging output from the example

#if DEBUG_595
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
\page pageICSTATION4x4 ICStation '595 Implementation
ICStation 595 4x4 Cube
----------------------
Source: ICStation "ICStation 4X4X4 Light Cube Kit For Arduino UNO" at http://www.icstation.com/icstation-4x4x4-light-cube-arduino-p-5312.html

![ICStation 4x4 Cube] (ICStation_image.jpg "ICStation 4x4 Cube")

The ICStation 595 cube shield is implemented using 2 74595 ICs with an SPI interface to the Arduino 
controller, and is therefore a 'scan and refresh' type of device. The 595 IC outputs are connected 
across the LEDs and each layer of the cube is enabled for a short period (LAYER_TIME), switching in 
rapid succession to create a Persistence of Vision in the user's eyes.

The hardware architecture implemented shown in the schematic below.

![ICStation 4x4 schematic] (ICStation_Schematic.png "ICStation 4x4 Schematic")

The shield uses pins 0 through 3 for the SPI interface and 4 through 7 to activate each layer (called 
D16..D19 in the schematic. Because of the way the LEDs are wired through only one resistor per layer, 
the result is less than impressive from a brightness perspetive, but it works.

###Implementation Overview###
The software implements an SPI interface through the standard Arduino SPI object.

Cube data is buffered in memory organised as vertical layers of 16 bits (1 word). At each refresh, the 595
is loaded with the data for a specific layer and the Arduino corresponding to that layer is set to groung (LOW)
to enable the circuit for that layer.

The cube data is held in memory buffers until an update() call is made, at which point it is entirely 
written to the cube through the SPI interface. The animate() call is used to scan the refresh of the 
cube. No interrupt timers are used in this implementation.

No intensity changes are implemented in this architecture.
*/

// Pins for SPI comm with the 595 IC. These are hard coded in the library.
#define CLK     0		///< SPI Clock pin number
#define LOAD    1		///< SPI Load pin number
#define DATA    3		///< SPI Data pin number
#define OUT_ENA 2		///< 595 Output Enable pin number

#define LAYER_TIME  5  	///< Reflech time for each layer, in milliseconds

#define CUBE_SIZE 4		///< Cube size in the X, Y and Z axis

class MD_Cubo_ICS595: public MD_Cubo
{
  public:
  MD_Cubo_ICS595(): MD_Cubo(CUBE_SIZE), _data(DATA), _clock(CLK), _load(LOAD), _enable(OUT_ENA), _curLayer(0), _pwmOffset(7) {return;};
  ~MD_Cubo_ICS595() {return;};
  void begin();
  void update();
  void animate(uint32_t wait = 0);
  void setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z);
  boolean getVoxel(uint8_t x, uint8_t y, uint8_t z);
  void clear(boolean p = false) { memset(&_scratch, (p ? 0xff : 0), sizeof(dispData_t)); };
  uint8_t size(axis_t axis) { return CUBE_SIZE; };
    
  private:
  typedef struct 
  {
    uint8_t  count[CUBE_SIZE];// the count of point on per layer  of current 
    uint16_t data[CUBE_SIZE]; // the bit pattern for each layer of the cube
  } dispData_t;
  
  dispData_t  _current;
  dispData_t  _scratch;

  const uint8_t   _enableLayer[CUBE_SIZE] = { 4, 5, 6, 7 };   // pins to enable each layer
  uint8_t   _pwmOffset;
    
  uint8_t   _curLayer;  // currently displayed layer
  uint32_t  _timeLayer; // layer display start time
  uint8_t   _data;    // SPI Data pin for transmission
  uint8_t   _clock;   // SPI Clock pin for transmisison
  uint8_t   _load;    // SPI Load pin for transmisison
  uint8_t   _enable;  // 595 enable active low
 
  void send595(uint16_t val);
};

#endif