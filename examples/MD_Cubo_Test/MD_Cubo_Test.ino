// LED Cube device independent hardware test code
//
// Runs through a general test of the hardware
// - Identify origin and x, y, Z axes
// - Show the led planes (in RGB order if color)
// - Intensity changes if implemented
// - All pixels on one at a time
// - All pixels progressively on and then progressively off
// - Some line drawing around cube edges and diagonals
//
#include <MD_Cubo.h>

// Pick the include file for the right cube type
//#include "MD_Cubo_4x4_72xx.h" 
//#include "MD_Cubo_4x4_ICS595.h"
#include "MD_Cubo_4x4_STC.h"
//#include "MD_Cubo_8x8_jC.h"

// Define the cube object
#ifdef MD_CUBO_4x4_72XX_H
MD_Cubo_72xx  C;
#endif
#ifdef MD_CUBO_4x4_ICS595_H
MD_Cubo_ICS595  C;
#endif
#ifdef MD_CUBO_8x8_JC_H
MD_Cubo_JC  C;
#endif
#ifdef MD_CUBO_4x4_STC_H
const uint8_t RX_PIN = 10;
const uint8_t TX_PIN = 11;
MD_Cubo_STC  C(RX_PIN, TX_PIN, 57600);
#endif

#define DEBUG 1   ///< Enable or disable (default) debugging output from the example

#if DEBUG
#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }      ///< Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } ///< Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } ///< Print a string followed by a value (binary)
#define PRINTS(s)     { Serial.print(F(s)); }           ///< Print a string
#define PRINTC(s, x, y, z, c) { PRINTS(s); PRINT("(",x); PRINT(",",y); PRINT(",",z); PRINTS(")");}  ///< Print coordinate tuple
#else
#define PRINT(s, v)   ///< Print a string followed by a value (decimal)
#define PRINTX(s, v)  ///< Print a string followed by a value (hex)
#define PRINTB(s, v)  ///< Print a string followed by a value (binary)
#define PRINTS(s)     ///< Print a string
#define PRINTC(s, x, y, z, c)  ///< Print coordinate tuple
#endif

void ledTest()
{
  const uint16_t  delay = 1000;

  C.clear();
  PRINTS("\nLED Test");
  PRINTS("\nXY Red Plane Z = ");
  for (uint8_t i = 0; i < C.size(MD_Cubo::ZAXIS); i++)
  {
    PRINT(" ", i);
    C.fillPlane(VOX_RED, MD_Cubo::XYPLANE, i);
    C.update();
    C.animate(delay);
    C.fillPlane(VOX_OFF, MD_Cubo::XYPLANE, i);
  }
  C.animate();

  PRINTS("\nXZ Green Y = ");
  for (uint8_t i = 0; i < C.size(MD_Cubo::YAXIS); i++)
  {
    PRINT(" ", i);
    C.fillPlane(VOX_GREEN, MD_Cubo::XZPLANE, i);
    C.update();
    C.animate(delay);
    C.fillPlane(VOX_OFF, MD_Cubo::XZPLANE, i);
  }
  C.animate();

  PRINTS("\nYZ Blue Plane X = ");
  for (uint8_t i = 0; i < C.size(MD_Cubo::XAXIS); i++)
  {
    PRINT(" ", i);
    C.fillPlane(VOX_BLUE, MD_Cubo::YZPLANE, i);
    C.update();
    C.animate(delay);
    C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, i);
  }
  C.animate();

  PRINTS("\nIntensity");
  for (uint8_t i = 0; i < C.size(MD_Cubo::XAXIS); i++)
    C.fillPlane(VOX_ON, MD_Cubo::YZPLANE, i);
  C.update();
  C.animate(delay);
  for (int16_t i = MAX_INTENSITY; i >= 0; --i)
  {
    PRINT(" ", i);
    C.setIntensity(i);
    C.animate(20);
  }

  C.clear();
  C.update();
  C.setIntensity(MAX_INTENSITY/2); 
  C.animate();
}

void lines()
{
  uint8_t lines[] = 
  {
    // diagonals
    0, 0, 0, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1,
    C.size(MD_Cubo::XAXIS) - 1, 0, 0, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1,
    C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, 0, 0, 0, C.size(MD_Cubo::ZAXIS) - 1,
    0, C.size(MD_Cubo::YAXIS) - 1, 0, C.size(MD_Cubo::XAXIS) - 1, 0, C.size(MD_Cubo::ZAXIS) - 1,
    // verticals                                                                                
    0, 0, 0, 0, 0, C.size(MD_Cubo::ZAXIS) - 1,
    C.size(MD_Cubo::XAXIS) - 1, 0, 0, C.size(MD_Cubo::XAXIS) - 1, 0, C.size(MD_Cubo::ZAXIS) - 1,
    C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, 0, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1,
    0, C.size(MD_Cubo::YAXIS) - 1, 0, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1,
    // base square                                    
    0, 0, 0, C.size(MD_Cubo::XAXIS) - 1, 0, 0,
    C.size(MD_Cubo::XAXIS) - 1, 0, 0, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, 0,
    C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, 0, 0, C.size(MD_Cubo::ZAXIS) - 1, 0,
    0, C.size(MD_Cubo::YAXIS) - 1, 0, 0, 0, 0,
    // top square
    0, 0, C.size(MD_Cubo::ZAXIS) - 1, C.size(MD_Cubo::XAXIS) - 1, 0, C.size(MD_Cubo::ZAXIS) - 1,
    C.size(MD_Cubo::XAXIS) - 1, 0, C.size(MD_Cubo::ZAXIS) - 1, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1,
    C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1,
    0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1, 0, 0, C.size(MD_Cubo::ZAXIS) - 1,
  };

  PRINTS("\nLines");
  C.clear();
  
  for (uint8_t i=0; i<ARRAY_SIZE(lines); i+=6)
  {
    PRINTC("\n", lines[i], lines[i + 1], lines[i + 2], VOX_ON);
    PRINTC("-", lines[i + 3], lines[i + 4], lines[i + 5], VOX_ON);
    C.drawLine(VOX_ON, lines[i], lines[i + 1], lines[i + 2], lines[i + 3], lines[i + 4], lines[i + 5]);
    C.update();
    C.animate(500);
  }    
}
  
void singles()
{
  // total time is set seconds, so scale delay accordingly
  uint16_t delay = 30000 / (C.size(MD_Cubo::XAXIS) * C.size(MD_Cubo::YAXIS) * C.size(MD_Cubo::ZAXIS));

  PRINTS("\nSingles: LED (X,Y,Z)")
  C.clear();
  for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
  {
    for (uint8_t y = 0; y < C.size(MD_Cubo::YAXIS); y++)
    {
      PRINTS("\n");
      for (uint8_t x = 0; x < C.size(MD_Cubo::XAXIS); x++)
      {
        PRINTC("", x, y, z, VOX_ON);
        C.setVoxel(VOX_ON, x, y, z);
        C.update();
        C.animate(delay);
        PRINTC("", x, y, z, VOX_OFF);
        C.setVoxel(VOX_OFF, x, y, z);
      }
    }
  }
}

void progressive()
{
  // total time is set seconds, so scale delay accordingly
  uint16_t delay = 30000 / (C.size(MD_Cubo::XAXIS) * C.size(MD_Cubo::YAXIS) * C.size(MD_Cubo::ZAXIS));

  PRINTS("\nProgressive (X,Y,Z)");
  C.clear();

  for (uint8_t pass = 0; pass < 2; pass++)
  {
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      for (uint8_t y = 0; y < C.size(MD_Cubo::YAXIS); y++)
      {
        PRINTS("\n");
        for (uint8_t x = 0; x < C.size(MD_Cubo::XAXIS); x++)
        {
          PRINTC("", x, y, z, pass == 0);
          C.setVoxel(pass == 0, x, y, z);
          C.update();
          C.animate(delay);
        }
      }
    }
  }
}

void showZero()
{
  const bool labelAxis = true;
  const uint16_t delay = 250;

  C.clear();
  PRINTS("\nShowZero\nX");
  // print out the X
  if (labelAxis)
  {
    if (C.size(MD_Cubo::XAXIS) >= 6)
    {
      C.drawLine(VOX_ON, 0, 1, 1, 0, 5, 5);
      C.drawLine(VOX_ON, 0, 1, 5, 0, 5, 1);
    }
    else
    {
      C.drawLine(VOX_ON, 0, 1, 1, 0, 3, 3);
      C.drawLine(VOX_ON, 0, 1, 3, 0, 3, 1);
    }
  }

  for (uint8_t i = 0; i < C.size(MD_Cubo::XAXIS); i++)
  {
    PRINT("", i);
    C.setVoxel(VOX_ON, i, 0, 0);
    C.update();
    C.animate(delay);
  }

  if (labelAxis)
  {
    // clear the X
    if (C.size(MD_Cubo::XAXIS) >= 6)
    {
      C.drawLine(VOX_OFF, 0, 1, 1, 0, 5, 5);
      C.drawLine(VOX_OFF, 0, 1, 5, 0, 5, 1);
    }
    else
    {
      C.drawLine(VOX_OFF, 0, 1, 1, 0, 3, 3);
      C.drawLine(VOX_OFF, 0, 1, 3, 0, 3, 1);
    }

    PRINTS("\nY");
    // draw the Y
    if (C.size(MD_Cubo::XAXIS) >= 6)
    {
      C.drawLine(VOX_ON, 0, 3, 3, 0, 5, 5);
      C.drawLine(VOX_ON, 0, 3, 3, 0, 1, 5);
      C.drawLine(VOX_ON, 0, 3, 3, 0, 3, 1);
    }
    else
    {
      C.drawLine(VOX_ON, 0, 2, 2, 0, 3, 3);
      C.drawLine(VOX_ON, 0, 2, 2, 0, 1, 3);
      C.drawLine(VOX_ON, 0, 2, 2, 0, 2, 1);
    }
  }

  for (uint8_t i = 0; i < C.size(MD_Cubo::YAXIS); i++)
  {
    PRINT("", i);
    C.setVoxel(VOX_ON, 0, i, 0);
    C.update();
    C.animate(delay);
  }

  if (labelAxis)
  {
    // clear the Y
    if (C.size(MD_Cubo::XAXIS) >= 6)
    {
      C.drawLine(VOX_OFF, 0, 3, 3, 0, 5, 5);
      C.drawLine(VOX_OFF, 0, 3, 3, 0, 1, 5);
      C.drawLine(VOX_OFF, 0, 3, 3, 0, 3, 1);
    }
    else
    {
      C.drawLine(VOX_OFF, 0, 2, 2, 0, 3, 3);
      C.drawLine(VOX_OFF, 0, 2, 2, 0, 1, 3);
      C.drawLine(VOX_OFF, 0, 2, 2, 0, 2, 1);
    }

    PRINTS("\nZ");
    // draw the Z
    if (C.size(MD_Cubo::XAXIS) >= 6)
    {
      C.drawLine(VOX_ON, 0, 1, 5, 0, 5, 5);
      C.drawLine(VOX_ON, 0, 1, 1, 0, 5, 5);
      C.drawLine(VOX_ON, 0, 1, 1, 0, 5, 1);
    }
    else
    {
      C.drawLine(VOX_ON, 0, 1, 3, 0, 3, 3);
      C.drawLine(VOX_ON, 0, 1, 1, 0, 3, 3);
      C.drawLine(VOX_ON, 0, 1, 1, 0, 3, 1);
    }
  }

  for (uint8_t i = 0; i < C.size(MD_Cubo::ZAXIS); i++)
  {
    PRINT("", i);
    C.setVoxel(VOX_ON, 0, 0, i);
    C.update();
    C.animate(delay);
  }
}

void scaleCube()
{
  const uint16_t delay = 500;

  PRINTS("\nScale Cube\nX");
  C.clear();
  for (uint8_t i = 0; i < C.size(MD_Cubo::ZAXIS)/2; i++)
  {
    C.drawCube(VOX_ON, i, i, i, C.size(MD_Cubo::ZAXIS) - (2*i));
    C.update();
    C.animate(delay);
    C.drawCube(VOX_OFF, i, i, i, C.size(MD_Cubo::ZAXIS) - (2 * i));
  }

  for (uint8_t i = 1; i <= C.size(MD_Cubo::ZAXIS) / 2; i++)
  {
    C.drawCube(VOX_ON, (C.size(MD_Cubo::ZAXIS) / 2) - i, (C.size(MD_Cubo::ZAXIS) / 2) - i, (C.size(MD_Cubo::ZAXIS) / 2) - i, 2 * i);
    C.update();
    C.animate(delay);
    C.drawCube(VOX_OFF, (C.size(MD_Cubo::ZAXIS) / 2) - i, (C.size(MD_Cubo::ZAXIS) / 2) - i, (C.size(MD_Cubo::ZAXIS) / 2) - i, 2 * i);
  }

  C.animate(delay);
}

void setup()
{
#if DEBUG
  Serial.begin(57600);
  PRINTS("\n[MD_Cubo Tester]");
#endif
  C.begin();
  C.setIntensity(MAX_INTENSITY / 2);
}

void loop()
{
  showZero();
  ledTest();
  scaleCube();
  singles();
  progressive();
  lines();
}

