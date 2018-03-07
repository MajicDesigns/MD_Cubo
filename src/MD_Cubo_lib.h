/*
MD_Cubo - Arduino LED Cube Library
  
See header file for comments
*/

 #pragma once

/**
 * \file
 * \brief Includes library definitions
 */

#define	MD_DEBUG	0		///< Enable or disable (default) debugging output from the MD_LED3 library

#if MD_DEBUG
#define	PRINT(s, v)		{ Serial.print(F(s)); Serial.print(v); }		  ///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)	{ Serial.print(F(s)); Serial.print(v, HEX); }	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)	{ Serial.print(F(s)); Serial.print(v, BIN); }	///< Print a string followed by a value (binary)
#define	PRINTS(s)		  { Serial.print(F(s)); }						///< Print a string
#else
#define	PRINT(s, v)		///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)	///< Print a string followed by a value (binary)
#define	PRINTS(s)		  ///< Print a string
#endif

/**
\page pageAddHardware Adding New Cube Types
Adding New Cube Types
---------------------
The aim of this libray is to move away from the 'bit-twiddling' code that seems to drive many cubes, 
to a device-independent software library that can be programmed in cartesian (XYZ) coordinates. In this 
way the same code could be used to drive many different cubes (types and sizes) with no or minimal 
changes required to user program code. 

This abstraction means that the library must provide the user code with hardware dependent primitives 
that work the same way for all cube types and sizes.

After doing some research of existing cubes it seems there are 2 main types of LED cube architectures:

- A scanning/refresh model, where the LEDs are refreshed by the Arduino code at a rate fast enough to 
activate a persistence of vision (POV) effect in the observer. This puts the onus of multiplexing control 
in the Arduino software. An example of this type hardware is the ICStation Light Cube Kit, based on 74HC595 
shift registers to control LED anodes and 4 I/O ports of Arduino control 4 common cathodes of LED. This type 
of hardware is the most common for LED cubes and relies on the micro controller software opening a circuit 
(a path for current) for each LED in very quick succession.
- A 'set and forget' model, where each LED is set by the Arduino and some other hardware components ensure 
that they remain turned on or off. One example is the PaulRB or JolliCube versions provided with the library.
These cubes uses a MAX7219 LED controller to drive the 64 LEDs in the cube, with multiple ICS required for 
larger cubes. Refreshing the LEDs is therefore handled by the 7219, with the Arduino periodically writing 
new settings through the interface registers.

The software must take into account these differences and allow both to work using the same basic software 
pattern. Some analysis and experimentation resulted in a very small set of primitives that need to be 
implemented to handle the differences. Virtual class methods are implemented in the MD_Cubo object -  
derived classes replace the generic method with code appropriate for the hardware.

The hardware dependent code needs to implement the following primitive functions:

- _Initialise the hardware_. This will clearly be different for the different hardware types. In the library 
this is implemented in the object constructor and begin() method for object.
- _Size of the cube_. To allow user code to be independent of hardware, the size of the cube being controlled
is obtained form the library. Provision is made to allow each of the X, Y and Z axes to be of different size.
- _Turn a LED on/off_. This is THE basic function for the cube and is implemented in the setVoxel() method. 
Every other drawing function - clear(), drawLine(), fillPlane() -  can be written using setVoxel(). The 
device function maps between the cartesian coordinates and the LED cube.
- _Test a LED on/off_. This is implemented in the getVoxel() method and used to test if a pizel is already on. 
Every other drawing function - clear(), drawLine(), fillPlane() -  can be written in using setVoxel(). The 
device function maps between the cartesian coordinates and the LED cube.
- _Update the cube_. Implemented in the update() method, this allows the internal buffers to be copied to the 
cube display, allowing for crisp animations and controlled updates, as all the changes accumulated since the 
previous update are shown at once.
- _Animate the cube_. This is the animate() method called in between updates to allow refreshing in multiplexed 
cubes. For 'set and forget' cubes, this method is ignored.
- _Clear the cube to on/off_ (optional but recommended). The base object provides a setVoxel() 
implementation in a loop, but it is usually more efficient to set directly set display buffers, as in practice 
this function is called frequently during execution.
- _Set the cube brightness_ (optional but recommended). The base object provides a placeholder. If the hardware 
supports a way to vary the brightness of the cube then this should be implemented to provide additional 
functionality.

The cube examples provided can be used as examples of how these functions may be implemented for new architectures.
*/
