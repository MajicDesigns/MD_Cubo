#ifndef MD_CUBO_8x8_ICS574_H
#define MD_CUBO_8x8_ICS574_H

/**
 * \file
 * \brief Header file for the MD_Cubo ICS574 class
 */

/**
\page pageICS5748x8x8 ICS574 Implementation
ICS574 8x8x8 Cube
-----------------
The monochrome cube is implemented using 8x 74HCT574, 74HCT138, 2x ULN2803A on an
Arduino NANO following the instructions given at https://www.electronicshub.org/8x8x8-led-cube.

![ICS574 8x8x8 Cube] (ICS574_Image.jpg "ICS574 8x8x8 Cube")

The cube is a modification of the original design with the UNL2803 transistor arrays
replacing the discrete 2n222 transistors. The circuit schematic for this modified design
is shown below.

![ICS574 Circuit Schematic] (8x8x8-ICS574-LED-Cube_RM_v4.png "ICS574 Circuit Schematic")

###Implementation Overview###
The cube is refreshed using a timer interrupt routine which makes this in the 'set
and forget' type of device. Note that the timer interrupt routine is implemented in
AVR assembler and is not portable across architectures.

Data is buffered in an internal array and displayed by the time routine. The data
buffer decouples the front end and back end of the application.
*/

const uint8_t CUBE_XSIZE  = 8;  ///< Cube size in the X axis
const uint8_t CUBE_YSIZE  = 8;  ///< Cube size in the Y axis
const uint8_t CUBE_ZSIZE  = 8;  ///< Cube size in the Z axis

class MD_Cubo_ICS574: public MD_Cubo
{
public:
  MD_Cubo_ICS574(): MD_Cubo(CUBE_XSIZE){};
  ~MD_Cubo_ICS574() {};

  void begin();
  void animate(uint32_t wait);
  void setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z);
  uint32_t getVoxel(uint8_t x, uint8_t y, uint8_t z);
  uint8_t size(axis_t axis);
};

#endif
