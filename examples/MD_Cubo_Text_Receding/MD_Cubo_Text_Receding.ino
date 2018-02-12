// LED Cube receding text demo
// 
// Single letter appears in the YZ plane at X=0 and recedes to the back
// Font file used is the same as for the MD_MAX72xx library at
// https://github.com/MajicDesigns/MD_MAX72xx 
// Font builder utility and documentation on the font format can be found 
// with the MD_MAX72xx library.
//
// Note this demo only works with large cubes (greater than the size of 
// the font width and height). There is no checking for cube sizing in 
// this demo code.

#include <MD_Cubo.h>
#include "MD_Cubo_Font.h"

// Pick the include file for the right cube
#include "MD_Cubo_8x8_jC.h"

// Define the cube object
#ifdef MD_CUBO_8x8_JC_H
MD_Cubo_JC  C;
#endif

#define DEBUG   0   // Enable or disable (default) debugging output from the example

#if DEBUG
#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }    // Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } // Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } // Print a string followed by a value (binary)
#define PRINTS(s)     { Serial.print(F(s)); }           // Print a string
#define PRINTC(s, x, y, z) { PRINTS(s); PRINT("(",x); PRINT(",",y); PRINT(",",z); PRINTS(")"); }  // Print coordinate tuple
#else
#define PRINT(s, v)   ///< Print a string followed by a value (decimal)
#define PRINTX(s, v)  ///< Print a string followed by a value (hex)
#define PRINTB(s, v)  ///< Print a string followed by a value (binary)
#define PRINTS(s)     ///< Print a string
#define PRINTC(s, x, y, z)  ///< Print coordinate tuple
#endif


void displayChar(char c, uint16_t delay)
// Display a character in the cube
{
  uint8_t size;
  uint8_t cBuf[C.size(MD_Cubo::YAXIS)];  // column data for characters

  C.clear();

  // Get the character defined from the font table and place it in the 
  // first YZ plane (x=0)
  size = getFontChar(c, ARRAY_SIZE(cBuf), cBuf);

  // Starting with a clean buffer, only need to set the bits that are on and
  // center the text by adding an offset to the Z coordinate
  for (uint8_t y = 0; y < size; y++) 
    for (uint8_t z = 0; z < 8; z++)    // only 8 bits in the height of the font
      if (cBuf[y] & (1 << z)) 
        C.setVoxel(true, 0, y + (C.size(MD_Cubo::YAXIS) - size) / 2, C.size(MD_Cubo::ZAXIS) - 1 - z);

  // Move the character through the X planes front to back. As an added
  // effect also vary the intensity from max to min.
  for (uint8_t i = 0; i < C.size(MD_Cubo::XAXIS); i++)
  {
    // display the update
    C.update();
    C.setIntensity(MAX_INTENSITY - ((MAX_INTENSITY / C.size(MD_Cubo::XAXIS) * (i + 1))));
    C.animate(delay);

    // copy to next plane and delete original
    C.copyPlane(MD_Cubo::YZPLANE, i, i + 1);
    C.fillPlane(false, MD_Cubo::YZPLANE, i);
  }
}

void setup()
{
#if DEBUG
  Serial.begin(57600);
  PRINTS("\n[MD_Cubo Text Receding Demo]");
#endif
  C.begin();
}

void loop()
{
  const char  message[] = "Hello World!  1234  ";
  static char  *cp = (char *)message;

  if (*cp == '\0') cp = (char *)message;
  displayChar(*cp++, 100);
}
