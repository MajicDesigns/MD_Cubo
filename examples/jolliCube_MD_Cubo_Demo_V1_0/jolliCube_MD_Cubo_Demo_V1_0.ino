// This sketch is largely adapted from the MD_Cubo library example and 
// some codes written for other LED cubes to work on the 8x8x8 jolliCube from jolliFactory
//
// Check out http://www.instructables.com/id/JolliCube-an-8x8x8-LED-Cube-SPI/ for detail 
// on building jolliCube
// Check out http://www.instructables.com/id/JolliCube-8x8x8-LED-Cube-With-MDCubo-Library/ 
// to see jolliCube in action using the MD_Cubo library.

// LED Cube demo patterns
// 
// Some original and some shamelessly copied from YouTube video of other cubes
//

#include <MD_Cubo.h>
#include "MD_Cubo_Font.h"

// Pick the include file for the right cube
//#include "MD_Cubo_4x4_72xx.h" 
//#include "MD_Cubo_4x4_ICS595.h"
#include "MD_Cubo_8x8_jC.h"

// Define the cube object
#ifdef MD_CUBO_4x4_72XX_H
MD_Cubo_72xx  C;
#endif
#ifdef MD_CUBO_4x4_ICS595_H
MD_Cubo_ICS595  C;
#endif
#ifdef MD_CUBO_8x8_JC_H
MD_Cubo_JC	C;
#endif

#define RANDOM_CYCLE  0 // 1 for random selection, 0 for sequential cycle

#define DEBUG   0   // Enable or disable (default) debugging output from the example

#if DEBUG
#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }      // Print a string followed by a value (decimal)
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

// Most demos run for a DEMO_RUNTIME ms before ending.
// De4fine constants and time tracking variable
#define DEMO_RUNTIME  10000L  //20000L
uint32_t timeStart;

void firefly()
// LEDs on and off like fireflies in the night 
{
  uint8_t x, y, z;
  
  PRINTS("\nFirefly");
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    C.clear();
    for (uint8_t x = 0; x<C.size(MD_Cubo::XAXIS); x++)
      for (uint8_t y = 0; y<C.size(MD_Cubo::YAXIS); y++)
        for (uint8_t z = 0; z<C.size(MD_Cubo::ZAXIS); z++)
          C.setVoxel((random(100)>50) ? VOX_ON : VOX_OFF, x, y, z);
    C.update();
    C.animate(50);
  }  
}

void brownian()
// 2x2 cube doing brownian motion
{
  uint8_t x = 0, y = 0, z = 0; 
  int8_t dx, dy, dz;
  
  PRINTS("\nBrownian");
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    C.clear();
    // PRINTC("\n", x, y, z);
    C.drawCube(VOX_ON, x, y, z, 2);
    C.update();
    C.animate(75);

    dx = random(3) - 1;
    dy = random(3) - 1;
    dz = random(3) - 1;
    if ((x + dx >= 0) && (x + dx + 1 < C.size(MD_Cubo::XAXIS))) x += dx;
    if ((y + dy >= 0) && (y + dy + 1 < C.size(MD_Cubo::YAXIS))) y += dy;
    if ((z + dz >= 0) && (z + dz + 1 < C.size(MD_Cubo::ZAXIS))) z += dz;
  }    
}

void slideFaces()
// Slide the faces around the horizontal center of the cube 
{
  const uint16_t  delay = 100;

  PRINTS("\nSlide Faces");  

  C.clear();
  C.fillPlane(VOX_ON, MD_Cubo::XYPLANE, 0);
  C.update();
  C.animate(delay);
  
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    for (uint8_t j = 0; j<C.size(MD_Cubo::YAXIS) - 1; j++)
    {
      C.drawLine(VOX_OFF, 0, j, 0, C.size(MD_Cubo::XAXIS) - 1, j, 0);
      C.drawLine(VOX_ON, 0, C.size(MD_Cubo::YAXIS) - 1, j + 1, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, j + 1);
      C.update();
      C.animate(delay);
    }
    for (uint8_t j = 0; j<C.size(MD_Cubo::ZAXIS) - 1; j++)
    {
      C.drawLine(VOX_OFF, 0, C.size(MD_Cubo::YAXIS) - 1, j, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, j);
      C.drawLine(VOX_ON, 0, C.size(MD_Cubo::YAXIS) - 2 - j, C.size(MD_Cubo::ZAXIS) - 1, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 2 - j, C.size(MD_Cubo::ZAXIS) - 1);
      C.update();
      C.animate(delay);
    }
    for (uint8_t j = 0; j<C.size(MD_Cubo::YAXIS) - 1; j++)
    {
      C.drawLine(VOX_OFF, 0, C.size(MD_Cubo::YAXIS) - 1 - j, C.size(MD_Cubo::ZAXIS) - 1, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1 - j, C.size(MD_Cubo::ZAXIS) - 1);
      C.drawLine(VOX_ON, 0, 0, C.size(MD_Cubo::ZAXIS) - 2 - j, C.size(MD_Cubo::XAXIS) - 1, 0, C.size(MD_Cubo::ZAXIS) - 2 - j);
      C.update();
      C.animate(delay);
    }
    for (uint8_t j = 0; j<C.size(MD_Cubo::ZAXIS) - 1; j++)
    {
      C.drawLine(VOX_OFF, 0, 0, C.size(MD_Cubo::ZAXIS) - 1 - j, C.size(MD_Cubo::XAXIS) - 1, 0, C.size(MD_Cubo::ZAXIS) - 1 - j);
      C.drawLine(VOX_ON, 0, j + 1, 0, C.size(MD_Cubo::XAXIS) - 1, j + 1, 0);
      C.update();
      C.animate(delay);
    }
  }  
}

void wrapFaces()
// Wrap the around all the 6 faces of the cube in a sliding pattern
{
  const uint16_t  delay = 100;

  PRINTS("\nWrap Faces");   

  C.clear();
  C.fillPlane(VOX_ON, MD_Cubo::XYPLANE, 0);
  C.update();
  C.animate(delay);
  
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    for (uint8_t j=0; j<C.size(MD_Cubo::YAXIS)-1; j++)
    { // move across the XY plane @ Z 0 into XZ plane @ Y max
      C.drawLine(VOX_OFF, 0, j, 0, C.size(MD_Cubo::ZAXIS)-1, j, 0);
      C.drawLine(VOX_ON, 0, C.size(MD_Cubo::YAXIS)-1, j+1, C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1, j+1);
      C.update();
      C.animate(delay);
    }
    for (uint8_t j=0; j<C.size(MD_Cubo::ZAXIS)-1; j++)
    { // move across the XZ plane @ Y max into XY plane @ Z max
      C.drawLine(VOX_OFF, 0, C.size(MD_Cubo::YAXIS)-1, j, C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1, j);
      C.drawLine(VOX_ON, 0, C.size(MD_Cubo::YAXIS)-2-j, C.size(MD_Cubo::ZAXIS)-1, C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-2-j, C.size(MD_Cubo::ZAXIS)-1);
      C.update();
      C.animate(delay);
    }
    for (uint8_t j=0; j<C.size(MD_Cubo::XAXIS)-1; j++)
    { // move across the XY plane @ Z max into YZ plane @ X max  
      C.drawLine(VOX_OFF, j, 0, C.size(MD_Cubo::ZAXIS)-1, j, C.size(MD_Cubo::YAXIS)-1, C.size(MD_Cubo::ZAXIS)-1);
      C.drawLine(VOX_ON, C.size(MD_Cubo::XAXIS)-1, 0, C.size(MD_Cubo::ZAXIS)-2-j, C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1, C.size(MD_Cubo::ZAXIS)-2-j);
      C.update();
      C.animate(delay);
    } 
    for (uint8_t j=0; j<C.size(MD_Cubo::YAXIS)-1; j++)
    { // move across the YZ plane @ X max into XZ plane @ Y 0
      C.drawLine(VOX_OFF, C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1-j, 0, C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1-j, C.size(MD_Cubo::ZAXIS)-1);
      C.drawLine(VOX_ON, C.size(MD_Cubo::XAXIS)-2-j, 0, 0, C.size(MD_Cubo::XAXIS)-2-j, 0, C.size(MD_Cubo::ZAXIS)-1);
      C.update();
      C.animate(delay);
    }
    for (uint8_t j=0; j<C.size(MD_Cubo::XAXIS)-1; j++)
    { // move across XZ Plane @ Y 0 into YZ plane @ X 0
      C.drawLine(VOX_OFF, C.size(MD_Cubo::XAXIS)-1-j, 0, 0, C.size(MD_Cubo::XAXIS)-1-j, 0, C.size(MD_Cubo::ZAXIS)-1);
      C.drawLine(VOX_ON, 0, j+1, 0, 0, j+1, C.size(MD_Cubo::ZAXIS)-1);
      C.update();
      C.animate(delay);
    }  
    for (uint8_t j=0; j<C.size(MD_Cubo::ZAXIS)-1; j++)
    { // move across YZ plane @ X 0 into XY plane @ Z 0 - back to starting point
      C.drawLine(VOX_OFF, 0, 0, C.size(MD_Cubo::ZAXIS)-1-j, 0, C.size(MD_Cubo::YAXIS)-1, C.size(MD_Cubo::ZAXIS)-1-j);
      C.drawLine(VOX_ON, j+1, 0, 0, j+1, C.size(MD_Cubo::YAXIS)-1, 0);
      C.update();
      C.animate(delay);
    }
  }
}

void intersectPlanes()
// Vertical and horizontal intersecting planes moving end-to-end in unison
{
  const uint16_t  delay = 200;

  PRINTS("\nIntersect Planes");

  C.clear();
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    for (uint8_t p = 0; p<C.size(MD_Cubo::YAXIS); p++)
    { // move plane one way ...
      C.fillPlane(VOX_ON, MD_Cubo::XYPLANE, p);
      C.fillPlane(VOX_ON, MD_Cubo::YZPLANE, p);
      C.update();
      C.animate(delay);
      C.fillPlane(VOX_OFF, MD_Cubo::XYPLANE, p);
      C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, p);
    }
    for (uint8_t p = C.size(MD_Cubo::YAXIS) - 2; p>0; --p)
    { // ... reverse back
      C.fillPlane(VOX_ON, MD_Cubo::XYPLANE, p);
      C.fillPlane(VOX_ON, MD_Cubo::YZPLANE, p);
      C.update();
      C.animate(delay);
      C.fillPlane(VOX_OFF, MD_Cubo::XYPLANE, p);
      C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, p);
    }
  }          
}

void droplets()
// Droplets moving up and down between the top and bottom
{
  const uint16_t delay = 50;
  const uint16_t MAX_DROP = (C.size(MD_Cubo::XAXIS)*C.size(MD_Cubo::YAXIS)) / 2;
  uint8_t *dropx = (uint8_t *)malloc(MAX_DROP);  
  uint8_t *dropy = (uint8_t *)malloc(MAX_DROP);
  uint8_t get = 0, put = 1;
  
  PRINTS("\nDroplets");
  memset(dropx, 0, MAX_DROP*sizeof(dropx[0]));
  memset(dropy, 0, MAX_DROP*sizeof(dropy[0]));
  C.clear();
  
  C.fillPlane(VOX_ON, MD_Cubo::XYPLANE, C.size(MD_Cubo::ZAXIS) - 1);
  
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    boolean found;
    uint8_t x, y;
    
    // find a coordinate to push down
    do 
    {
      x = random(C.size(MD_Cubo::XAXIS));
      y = random(C.size(MD_Cubo::YAXIS));

      //PRINT("\nNew drop ", x);
      //PRINT(",", y);
      found = false;
      for (uint16_t i=0; i<MAX_DROP && !found; i++)
        found |= (x == dropx[i]) && (y == dropy[i]);
      //PRINT("- search ", found);
    } while (found);
  
    if (put == get)   // raise the oldest one
    {
      //PRINTS(" - overwrite");
      for (uint8_t z = 0; z<C.size(MD_Cubo::ZAXIS) - 1; z++)
      {
        C.setVoxel(VOX_OFF, dropx[get], dropy[get], z);
        C.setVoxel(VOX_ON, dropx[get], dropy[get], z+1);
        C.update();
        C.animate(delay);
      }
      get = (get+1) % MAX_DROP;
    }
    
    dropx[put] = x;
    dropy[put] = y;
    put = (put+1) % MAX_DROP;
    
    // drop the new one
    for (uint8_t z = C.size(MD_Cubo::ZAXIS) - 1; z>0; --z)
    {
      C.setVoxel(VOX_OFF, x, y, z);
      C.setVoxel(VOX_ON, x, y, z-1);
      C.update();
      C.animate(delay);
    }
  }
  
  // now finish up by returning all the drops to the top
  //PRINTS("\nFinishing up");
  do
  {
    for (uint8_t z = 0; z<C.size(MD_Cubo::ZAXIS) - 1; z++)
    {
      C.setVoxel(VOX_OFF, dropx[get], dropy[get], z);
      C.setVoxel(VOX_ON, dropx[get], dropy[get], z+1);
      C.update();
      C.animate(delay);
    }
    get = (get+1) % MAX_DROP;
  } while (get != put);
  
  free(dropx);
  free(dropy);
}

void scaleCube()
// Scale a cube from the largest size to the smallest in the center
{
  const uint16_t delay = 125;

  C.clear();
  for (uint8_t i = 0; i < C.size(MD_Cubo::ZAXIS) / 2; i++)
  {
    C.drawCube(VOX_ON, i, i, i, C.size(MD_Cubo::ZAXIS) - (2 * i));
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
}

void shrinkCube()
// Shrink a cube from largest to smallest in the corner. 
// Random corners picked for each turn
{
  const uint16_t delay = 75;
  uint8_t cur = 0;
  uint8_t x, y, z, dx, dy, dz;
  int8_t corners[8][6] = // corner coordinates and delta multipliers for size 
  {
    { 0, 0, 0 , 1, 1, 1 },
    { C.size(MD_Cubo::XAXIS)-1, 0, 0 , -1, 1, 1 },
    { 0, C.size(MD_Cubo::YAXIS)-1, 0, 1, -1, 1 },
    { 0, 0, C.size(MD_Cubo::ZAXIS)-1, 1, 1, -1 },
    { C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1, 0, -1, -1, 1 },
    { C.size(MD_Cubo::XAXIS)-1, 0, C.size(MD_Cubo::ZAXIS)-1, -1, 1, -1 },
    { 0, C.size(MD_Cubo::YAXIS)-1, C.size(MD_Cubo::ZAXIS)-1, 1, -1, -1 },
    { C.size(MD_Cubo::XAXIS)-1, C.size(MD_Cubo::YAXIS)-1, C.size(MD_Cubo::ZAXIS)-1, -1, -1, -1 }
  };

  PRINTS("\nShrink Cube");

  C.clear();

  // Grown the cube from max size to min size then back to max,
  // selected another corner and repeat
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    x = corners[cur][0];
    y = corners[cur][1];
    z = corners[cur][2];

    // growth phase
    for (uint8_t i = 1; i < C.size(MD_Cubo::ZAXIS); i++)
    {
      dx = i * corners[cur][3];
      dy = i * corners[cur][4];
      dz = i * corners[cur][5];

      //PRINTC("\nG Cube ", x, y, z);
      //PRINT(" size ", i+1);

      C.drawRPrism(VOX_ON, x, y, z, dx, dy, dz);
      C.update();
      C.animate(delay);
      C.drawRPrism(VOX_OFF, x, y, z, dx, dy, dz);
    }

    // see if we should throw in a WOW!
    if (random(50) >= 25) scaleCube();

    // pick another corner
    cur = random(ARRAY_SIZE(corners));
    x = corners[cur][0];
    y = corners[cur][1];
    z = corners[cur][2];

    // shrink down to this new corner
    for (uint8_t i = C.size(MD_Cubo::ZAXIS)-1; i >= 1; i--)
    {
      dx = i * corners[cur][3];
      dy = i * corners[cur][4];
      dz = i * corners[cur][5];

      //PRINTC("\nS Cube ", x, y, z);
      //PRINT(" size ", i+1);

      C.drawRPrism(VOX_ON, x, y, z, dx, dy, dz);
      C.update();
      C.animate(delay);
      C.drawRPrism(VOX_OFF, x, y, z, dx, dy, dz);
    }

  }
}

void rain()
// Rain drops passing through the cube, top to bottom
{
  uint16_t delay = 100;

  PRINTS("\nRain");
  C.clear();

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    uint8_t num_drops = random(100) % (C.size(MD_Cubo::YAXIS)-2);

    C.fillPlane(VOX_OFF, MD_Cubo::XYPLANE, C.size(MD_Cubo::ZAXIS)-1);
    for (uint8_t i = 0; i < num_drops; i++)
    {
      uint8_t x = random(C.size(MD_Cubo::XAXIS));
      uint8_t y = random(C.size(MD_Cubo::YAXIS));
      C.setVoxel(VOX_ON, x, y, C.size(MD_Cubo::ZAXIS) - 1);
    }
    C.update();
    C.animate(delay);

    // move all the Z planes down
    for (uint8_t i = 1; i < C.size(MD_Cubo::ZAXIS); i++)
      C.copyPlane(MD_Cubo::XYPLANE, i, i-1);
  }
  return;
}

void randomFill()
// Completely fill the cube with a random pattern until all LEDs lit
{
  int c = 0;
  
  PRINTS("\nRandom fill");
  C.clear();

  while (c < (C.size(MD_Cubo::XAXIS)*C.size(MD_Cubo::YAXIS)*C.size(MD_Cubo::ZAXIS))-1)
  {
    uint8_t x = random(C.size(MD_Cubo::XAXIS));
    uint8_t y = random(C.size(MD_Cubo::YAXIS));
    uint8_t z = random(C.size(MD_Cubo::XAXIS));

    if (C.getVoxel(x, y, z) == VOX_ON)
      continue;
    
    C.setVoxel(VOX_ON, x, y, z);
    C.update();
    //C.animate(75);
    C.animate(5);
    c++;
  }
}

float length(int8_t x1, int8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2)
// Work out the length of a line
{
  int8_t dx = (x1 - x2), dy = (y1 - y2), dz = (z1 - z2);

  return(sqrt((dx*dx) + (dy*dy) + (dz*dz)));
}

void ripples()
// Display a sine wave running out from the center of the cube
// A symmetrical display, so only need to calculate one quarter of the points
{
  const float waveInterval = 1.5;
  uint16_t iteration = 0;
  uint8_t midX = C.size(MD_Cubo::XAXIS) / 2;
  uint8_t midY = C.size(MD_Cubo::YAXIS) / 2;

  // this only works for larger cubes, so return if the cube is not big enough
  if ((C.size(MD_Cubo::XAXIS) < 8 || C.size(MD_Cubo::YAXIS) < 8 || C.size(MD_Cubo::ZAXIS) < 8))
    return;

  PRINTS("\nRipples");

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    C.clear();

    for (uint8_t x = 0; x < midX; x++)
    {
      for (uint8_t y = 0; y < midY; y++)
      {
        float dist = length((C.size(MD_Cubo::ZAXIS) - 1) / 2, (C.size(MD_Cubo::YAXIS) - 1) / 2, 0, x, y, 0) / 9.899495 * C.size(MD_Cubo::ZAXIS);
        float height = (C.size(MD_Cubo::ZAXIS) / 2) + (sin((dist / waveInterval) + (float)iteration / 200) * (C.size(MD_Cubo::ZAXIS) / 2));

        C.setVoxel(VOX_ON, x, y, (int)height);
        C.setVoxel(VOX_ON, C.size(MD_Cubo::XAXIS)-1-x, y, (int)height);
        C.setVoxel(VOX_ON, x, C.size(MD_Cubo::YAXIS)-1-y, (int)height);
        C.setVoxel(VOX_ON, C.size(MD_Cubo::XAXIS)-1-x, C.size(MD_Cubo::YAXIS)-1-y, (int)height);
        iteration++;
      }
    }
    C.update();
    C.animate(0);
  }
}

void oscillation()
// An oscillating wave passing through the cube
{
  uint8_t curZ = 0, dz = 1;

  PRINTS("\nOscillation");

  C.clear();
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    // shift all the planes back by one
    for (uint8_t x = C.size(MD_Cubo::XAXIS) - 1; x > 0; x--)
      C.copyPlane(MD_Cubo::YZPLANE, x - 1, x);
    C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, 0);

    // draw the wave line on the YZ plane
    C.drawLine(VOX_ON, 0, 0, curZ, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1 - curZ);
    curZ += dz;
    if (curZ == C.size(MD_Cubo::ZAXIS) || curZ == 0) dz = -dz;

    C.update();
    C.animate(100);
  }
}

void flagwave()
// Looks like a flag waving in the wind
{
  uint8_t curY = 0, dy = 1;

  PRINTS("\nFlag wave");

  C.clear();
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    // shift all the planes back by one
    for (uint8_t x = C.size(MD_Cubo::XAXIS) - 1; x > 0; x--)
      C.copyPlane(MD_Cubo::YZPLANE, x - 1, x);
    C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, 0);

    // draw the wave line on the YZ plane
    C.drawLine(VOX_ON, 0, curY, 0, 0, curY, C.size(MD_Cubo::ZAXIS) - 1);
    curY += dy;
    if (curY == C.size(MD_Cubo::YAXIS)-1 || curY == 0) dy = -dy;

    C.update();
    C.animate(100);
  }
}

void spiral()
// A spiral wave passing through the cube
{
  uint8_t curZ = 0, dz = 1;
  uint8_t curY = 0, dy = 0;

  PRINTS("\nSpiral");

  C.clear();
  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    // shift all the planes back by one
    for (uint8_t x = C.size(MD_Cubo::XAXIS) - 1; x > 0; x--)
      C.copyPlane(MD_Cubo::YZPLANE, x - 1, x);
    C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, 0);

    // draw the spiral line on the YZ plane
    if (dy == 0)
      C.drawLine(VOX_ON, 0, 0, curZ, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1 - curZ);
    else
      C.drawLine(VOX_ON, 0, curY, C.size(MD_Cubo::ZAXIS) - 1, 0, C.size(MD_Cubo::YAXIS) - 1 - curY, 0);
    curZ += dz;
    curY += dy;
    // reset the Z - note first line of Y and last of Z are the same line so avoid double delays by stopping 
    // Z earlier
    if (curZ == C.size(MD_Cubo::ZAXIS)-1)
    {
      dz = 0;
      dy = 1;
      curZ = 0;
    }
    // reset the Y
    if (curY == C.size(MD_Cubo::YAXIS))
    {
      dz = 1;
      dy = 0;
      curY = 0;
    }

    C.update();
    C.animate(100);
  }
}

void suspension()
// Full plane moving end-to-end losing LEDs 'in suspension' as it goes
{
  const uint16_t delay = 50;  //100;
  // Spread out the LEDs evenly in each layer
  const uint8_t numSpeckles = (C.size(MD_Cubo::YAXIS) * C.size(MD_Cubo::ZAXIS)) / C.size(MD_Cubo::XAXIS);

  PRINTS("\nSuspension");

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    C.clear();

    // fill the end plane
    C.fillPlane(VOX_ON, MD_Cubo::YZPLANE, 0);
    C.update();
    C.animate(delay);

    // * Spreading points through the cube
    // 1. copy the current plane to the next one
    // 2. clear the current plane
    // 3. randomly clear numSpeckle points in the new plane and set the same points in the previous one
    for (uint8_t x = 0; x < C.size(MD_Cubo::XAXIS) - 1; x++)
    {
      C.copyPlane(MD_Cubo::YZPLANE, x, x + 1);
      C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, x);

      for (uint8_t j = 0; j < numSpeckles; j++)
      {
        uint8_t y, z;

        do
        {
          y = random(C.size(MD_Cubo::YAXIS));
          z = random(C.size(MD_Cubo::ZAXIS));
        } while (C.getVoxel(x+1, y, z) == VOX_OFF);

        C.setVoxel(VOX_ON, x, y, z);
        C.setVoxel(VOX_OFF, x + 1, y, z);
      }

      C.update();
      C.animate(delay);
    }

    C.animate(delay * 3);

    // * Collapsing it all together again
    // 1. every point set in this layer needs to be set in the next.
    // 2. clear the current layer.
    for (uint8_t x = 0; x < C.size(MD_Cubo::XAXIS) - 1; x++)
    {
      for (uint8_t y = 0; y < C.size(MD_Cubo::YAXIS); y++)
        for (uint8_t z = 0; z < C.size(MD_Cubo::YAXIS); z++)
          if (C.getVoxel(x, y, z) == VOX_ON) C.setVoxel(VOX_ON, x + 1, y, z);
      C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, x);
      C.update();
      C.animate(delay);
    }

    C.animate(delay * 3);

    // now do it all again in reverse
    // * Spreading points through the cube
    for (uint8_t x = C.size(MD_Cubo::XAXIS) - 1; x > 0; x--)
    {
      C.copyPlane(MD_Cubo::YZPLANE, x, x - 1);
      C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, x);

      for (uint8_t j = 0; j < numSpeckles; j++)
      {
        uint8_t y, z;

        do
        {
          y = random(C.size(MD_Cubo::YAXIS));
          z = random(C.size(MD_Cubo::ZAXIS));
        } while (C.getVoxel(x - 1, y, z) == VOX_OFF);

        C.setVoxel(VOX_ON, x, y, z);
        C.setVoxel(VOX_OFF, x - 1, y, z);
      }

      C.update();
      C.animate(delay);
    }

    C.animate(delay * 3);

    // * Collapsing it all together again
    for (uint8_t x = C.size(MD_Cubo::XAXIS) - 1; x > 0; x--)
    {
      for (uint8_t y = 0; y < C.size(MD_Cubo::YAXIS); y++)
        for (uint8_t z = 0; z < C.size(MD_Cubo::YAXIS); z++)
          if (C.getVoxel(x, y, z) == VOX_ON) C.setVoxel(VOX_ON, x - 1, y, z);
      C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, x);
      C.update();
      C.animate(delay);
    }

    C.animate(delay * 3);
  }
}

void hourglass()
// Looks like an hourglass dropping sand grains as time passes
{
#define GP(x, y, z) (((x & 0x7)<<5) | ((y & 0x7)<<2) | (z & 3))
#define UX(b) (b>>5)
#define UY(b) ((b>>2) & 0x7)
#define UZ(b) (b & 0x3)
  static const uint8_t PROGMEM sand[] = // define sand grains in the order they will disappear/appear
  {
    // end layer
    GP(3, 3, 0), GP(3, 4, 0), GP(4, 4, 0), GP(4, 3, 0), GP(3, 2, 0), GP(2, 3, 0),
    GP(2, 4, 0), GP(3, 5, 0), GP(4, 5, 0), GP(5, 4, 0), GP(5, 3, 0), GP(4, 2, 0),
    GP(3, 1, 0), GP(2, 1, 0), GP(2, 2, 0), GP(1, 1, 0), GP(1, 2, 0), GP(1, 3, 0),
    GP(1, 4, 0), GP(1, 5, 0), GP(2, 5, 0), GP(1, 6, 0), GP(2, 6, 0), GP(3, 6, 0),
    GP(4, 6, 0), GP(5, 6, 0), GP(5, 5, 0), GP(6, 6, 0), GP(6, 5, 0), GP(6, 4, 0),
    GP(6, 3, 0), GP(6, 2, 0), GP(5, 2, 0), GP(6, 1, 0), GP(5, 1, 0), GP(4, 1, 0),

    // end-1 layer
    GP(3, 3, 1), GP(3, 4, 1), GP(4, 4, 1), GP(4, 3, 1), GP(3, 2, 1), GP(2, 3, 1),
    GP(2, 4, 1), GP(3, 5, 1), GP(4, 5, 1), GP(5, 4, 1), GP(5, 3, 1), GP(4, 2, 1),
    GP(3, 1, 1), GP(2, 1, 1), GP(2, 2, 1), GP(1, 1, 1), GP(1, 2, 1), GP(1, 3, 1),
    GP(1, 4, 1), GP(1, 5, 1), GP(2, 5, 1), GP(1, 6, 1), GP(2, 6, 1), GP(3, 6, 1),
    GP(4, 6, 1), GP(5, 6, 1), GP(5, 5, 1), GP(6, 6, 1), GP(6, 5, 1), GP(6, 4, 1),
    GP(6, 3, 1), GP(6, 2, 1), GP(5, 2, 1), GP(6, 1, 1), GP(5, 1, 1), GP(4, 1, 1),

    // end-2 layer
    GP(3, 3, 2), GP(3, 4, 2), GP(4, 4, 2), GP(4, 3, 2), GP(3, 2, 2), GP(2, 2, 2),
    GP(2, 3, 2), GP(2, 4, 2), GP(2, 5, 2), GP(3, 5, 2), GP(4, 5, 2), GP(5, 5, 2),
    GP(5, 4, 2), GP(5, 3, 2), GP(5, 2, 2), GP(4, 2, 2),

    // end-3 layer
    GP(3, 3, 3), GP(3, 4, 3), GP(4, 4, 3), GP(4, 3, 3)
  };
  uint8_t s, x, y, z;
  const uint16_t delay = DEMO_RUNTIME / ARRAY_SIZE(sand) / 3; // 3 delays per action

  // this only works for larger cubes, so return if the cube is not big enough
  if ((C.size(MD_Cubo::XAXIS) != 8 || C.size(MD_Cubo::YAXIS) != 8 || C.size(MD_Cubo::ZAXIS) != 8))
    return;

  PRINTS("\nHourglass");

  // Draw the hourglass cube outline and the sand within it
  C.clear();
  C.drawCube(VOX_ON, 0, 0, 0, C.size(MD_Cubo::XAXIS));
  for (uint8_t i = 0; i < ARRAY_SIZE(sand); i++)
  {
    s = pgm_read_byte(&sand[i]);
    x = UX(s); y = UY(s); z = C.size(MD_Cubo::ZAXIS) - 1 - UZ(s);
    C.setVoxel(VOX_ON, x, y, z);
  }
  C.update();
  C.animate(1000);

  // drop all the grains to the bottom 
  for (uint8_t i = 0; i < ARRAY_SIZE(sand); i++)
  {
    s = pgm_read_byte(&sand[i]);
    x = UX(s); y = UY(s); z = UZ(s);

    // turn it off at the top
    C.setVoxel(VOX_OFF, x, y, C.size(MD_Cubo::ZAXIS) - 1 - z);
    C.update();
    C.animate(delay);

    // drop it down
    for (uint8_t i = (C.size(MD_Cubo::ZAXIS) / 2); i > z; i--)
    {
      C.setVoxel(VOX_ON, 3, 3, i);
      C.update();
      C.animate(delay / 3);
      C.setVoxel(VOX_OFF, 3, 3, i);
    }

    // turn it on at the bottom
    C.setVoxel(VOX_ON, x, y, z);

    // update and wait for next grain
    C.update();
    C.animate(delay);
  }
  C.animate(1000);
}

void boingCube()
// Small cube grows in the direction of one of the axes and then expands out to full size, 
// repeated in reverse and iterates through all the axes.
{
  const uint16_t  delay = 100;
  uint8_t pass = 0;
  uint8_t x, y, z;  // corner coordinates of the rect prism we are drawing
  int8_t  dx, dy, dz; // direction of expansion for each coordinate
  int8_t  sx, sy, sz; // the size of the cube in x, y and z directions

  PRINTS("\nBoing Cube");

  x = (C.size(MD_Cubo::XAXIS) / 2) - 1;
  y = (C.size(MD_Cubo::YAXIS) / 2) - 1;
  z = (C.size(MD_Cubo::ZAXIS) / 2) - 1;
  sx = 2; sy = 2; sz = 2;

  // draw the initial seed cube
  C.clear();
  C.drawRPrism(VOX_ON, x, y, z, sx-1, sy-1, sz-1);
  C.update();
  C.animate(delay);

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    // expand the cube along the chosen direction for this path
    switch (pass)
    {
      case 0: dx = -1; dy = 0; dz = 0; break;
      case 1: dx = 0; dy = -1; dz = 0; break;
      case 2: dx = 0; dy = 0; dz = -1; break;
    }

    while (x != 0 && y != 0 && z != 0)  // stop when we have reached the 'front' face
    {
      x += dx; y += dy; z += dz;
      sx += 2 * abs(dx); sy += 2 * abs(dy); sz += 2 * abs(dz);
      C.clear();
      C.drawRPrism(VOX_ON, x, y, z, sx-1, sy-1, sz-1);
      C.update();
      C.animate(delay);
    }

    // now expand the face at either end until we have a full cube
    switch (pass)
    {
      case 0: dx = 0; dy = -1; dz = -1; break;
      case 1: dx = -1; dy = 0; dz = -1; break;
      case 2: dx = -1; dy = -1; dz = 0; break;
    }
  
    while (sx != C.size(MD_Cubo::XAXIS) || sy != C.size(MD_Cubo::YAXIS) || sz != C.size(MD_Cubo::ZAXIS))
    {
      x += dx; y += dy; z += dz;
      sx += 2 * abs(dx); if (sx == C.size(MD_Cubo::XAXIS)) dx = 0;
      sy += 2 * abs(dy); if (sy == C.size(MD_Cubo::YAXIS)) dy = 0;
      sz += 2 * abs(dz); if (sz == C.size(MD_Cubo::ZAXIS)) dz = 0;
      C.clear();
      C.drawRPrism(VOX_ON, x, y, z, sx - 1, sy - 1, sz - 1);
      C.update();
      C.animate(delay);
    }
    C.animate(2*delay);

    // now shrink it back down
    switch (pass)
    {
    case 0: dx = 0; dy = 1; dz = 1; break;
    case 1: dx = 1; dy = 0; dz = 1; break;
    case 2: dx = 1; dy = 1; dz = 0; break;
    }

    while (sx != 2 && sy != 2 && sz != 2)
    {
      x += dx; y += dy; z += dz;
      sx -= 2 * abs(dx); if (sx == 2) dx = 0;
      sy -= 2 * abs(dy); if (sy == 2) dy = 0;
      sz -= 2 * abs(dz); if (sz == 2) dz = 0;
      C.clear();
      C.drawRPrism(VOX_ON, x, y, z, sx - 1, sy - 1, sz - 1);
      C.update();
      C.animate(delay);
    }

    switch (pass)
    {
    case 0: dx = 1; dy = 0; dz = 0; break;
    case 1: dx = 0; dy = 1; dz = 0; break;
    case 2: dx = 0; dy = 0; dz = 1; break;
    }

    while (sx != 2 || sy != 2 || sz != 2)  // stop when we have reached the 'front' face
    {
      sx -= 2 * abs(dx); sy -= 2 * abs(dy); sz -= 2 * abs(dz);
      x += dx; if (sx == 2) dx = 0;
      y += dy; if (sy == 2) dy = 0;
      z += dz; if (sz == 2) dz = 0;
      C.clear();
      C.drawRPrism(VOX_ON, x, y, z, sx - 1, sy - 1, sz - 1);
      C.update();
      C.animate(delay);
    }

    // increment the pass we are on with wraparound
    pass = (pass + 1) % 3;
  }
}

void spiralLine()
// vertical line spirals from the outside to the inside and then moves back out, to start all over again
{
  uint16_t delay = 50;

  PRINTS("\nSpiral Line");

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    for (uint8_t instep = 0; instep < C.size(MD_Cubo::XAXIS) / 2; instep++)
    {
      for (int8_t x = instep; x < C.size(MD_Cubo::XAXIS) - instep; x++)
      {
        C.clear();
        C.drawLine(VOX_ON, x, instep, 0, x, instep, C.size(MD_Cubo::ZAXIS) - 1);
        C.update();
        C.animate(delay);
      }
      for (int8_t y = 1 + instep; y < C.size(MD_Cubo::YAXIS) - instep; y++)
      {
        C.clear();
        C.drawLine(VOX_ON, C.size(MD_Cubo::XAXIS) - 1 - instep, y, 0, C.size(MD_Cubo::XAXIS) - 1 - instep, y, C.size(MD_Cubo::ZAXIS) - 1);
        C.update();
        C.animate(delay);
      }
      for (int8_t x = C.size(MD_Cubo::XAXIS) - 2 - instep; x >= instep; x--)
      {
        C.clear();
        C.drawLine(VOX_ON, x, C.size(MD_Cubo::YAXIS) - 1 - instep, 0, x, C.size(MD_Cubo::YAXIS) - 1 - instep, C.size(MD_Cubo::ZAXIS) - 1);
        C.update();
        C.animate(delay);
      }
      for (int8_t y = C.size(MD_Cubo::YAXIS) - 2 - instep; y >= 1 + instep; y--)
      {
        C.clear();
        C.drawLine(VOX_ON, instep, y, 0, instep, y, C.size(MD_Cubo::ZAXIS) - 1);
        C.update();
        C.animate(delay);
      }
    }
    for (int8_t i = C.size(MD_Cubo::XAXIS) / 2; i >= 0; i--)
    {
      C.clear();
      C.drawLine(VOX_ON, i, i, 0, i, i, C.size(MD_Cubo::ZAXIS) - 1);
      C.update();
      C.animate(delay);
    }
  }
}

void outsideStack()
// outside square is drawn up the cube, then collapses into the top, the down the cube, repeats 
{
  uint16_t delay = 50;

  PRINTS("\nSpiral Line");

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    C.clear();
    for (uint8_t step = 0; step < 2; step++)
    {
      for (int8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
      {
        C.drawRPrism((step == 0) ? VOX_ON : VOX_OFF, 0, 0, z, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, 1);
        C.update();
        C.animate(delay);
      }
    }
    for (uint8_t step = 0; step < 2; step++)
    {
      for (int8_t z = C.size(MD_Cubo::ZAXIS) - 1; z >= 0; z--)
      {
        C.drawRPrism((step == 0) ? VOX_ON : VOX_OFF, 0, 0, z, C.size(MD_Cubo::XAXIS) - 1, C.size(MD_Cubo::YAXIS) - 1, 1);
        C.update();
        C.animate(delay);
      }
    }
  }
}

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
        C.setVoxel(VOX_ON, 0, y + (C.size(MD_Cubo::YAXIS) - size) / 2, C.size(MD_Cubo::ZAXIS) - 1 - z);

  // Move the character through the X planes front to back. As an added
  // effect also vary the intensity from max to min.
  for (uint8_t i = 0; i < C.size(MD_Cubo::XAXIS); i++)
  {
    // display the update
    C.update();
    C.setIntensity(MAX_INTENSITY - (((MAX_INTENSITY / C.size(MD_Cubo::XAXIS)) * i)));
    C.animate(delay);

    // copy to next plane and delete original
    C.copyPlane(MD_Cubo::YZPLANE, i, i + 1);
    C.fillPlane(VOX_OFF, MD_Cubo::YZPLANE, i);
  }
}

void recedingText()
{
  //const char  message[] = "MDCuboDemo";
  const char  message[] = "presents";
  char  *cp = (char *)message;

  // this only works for larger cubes, so return if the cube is not big enough
  if ((C.size(MD_Cubo::XAXIS) < 8 || C.size(MD_Cubo::YAXIS) < 8 || C.size(MD_Cubo::ZAXIS) < 8))
    return;

  PRINTS("\nReceding text");

  while (*cp != '\0')
    displayChar(*cp++, 100);

  C.setIntensity(MAX_INTENSITY / 2);
}

void scrollFaces(void)
// scroll all the columns across, starting with the second last and working backwards
{
  // XZ Plane at Ymax
  for (int8_t x = 1; x < C.size(MD_Cubo::XAXIS); x++)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      uint32_t p = C.getVoxel(x + 1, C.size(MD_Cubo::YAXIS) - 1, z);
      C.setVoxel(p, x, C.size(MD_Cubo::YAXIS) - 1, z);
    }
  // YZ Plane at Xmax
  for (int8_t y = C.size(MD_Cubo::YAXIS) - 2; y >= 0; y--)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      uint32_t p = C.getVoxel(C.size(MD_Cubo::XAXIS) - 1, y, z);
      C.setVoxel(p, C.size(MD_Cubo::XAXIS) - 1, y + 1, z);
    }
  // XZ Plane at Y0
  for (int8_t x = C.size(MD_Cubo::XAXIS) - 2; x >= 0; x--)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      uint32_t p = C.getVoxel(x, 0, z);
      C.setVoxel(p, x + 1, 0, z);
    }
  // YZ Plane at X0
  for (int8_t y = 1; y < C.size(MD_Cubo::YAXIS); y++)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      uint32_t p = C.getVoxel(0, y, z);
      C.setVoxel(p, 0, y - 1, z);
    }
}

boolean displayMessage(char *mesg, uint16_t delay = 0)
// Display a message on the surface of the cube, scrolling around the outside faces
// Implemented as a non-blocking Finite State Machine.
// Returns VOX_ON when the message has completed.
// The message is only reference by a pointer to the string, 
// so the caller is responsible for making sure it remains stable 
// until the data is displayed.
{
  static enum {S_INIT, S_LOADC, S_SCROLL, S_WAITING, S_TIMER, S_CLEANUP, S_END} state = S_END;
  static uint32_t timeStart;    // time at the start of the wait (ms)
  static uint16_t timeWait;     // waiting time (ms)
  static char  *cp;             // where we are in the message
  static uint8_t cBuf[10];      // column data for characters - this should be bigger than any of the faces being displayed
  static uint8_t getCol = 0;    // current column of the character
  static uint8_t size;          // size of the character (columns)
  static uint16_t countScroll;  // the number of columns being scrolled

  if (mesg != NULL) state = S_INIT; // new message to process

  switch (state)
  {
  case S_INIT: // initialization of a new message
    PRINTS("\nS_INIT");
    cp = mesg;
    timeWait = delay;
    countScroll = (C.size(MD_Cubo::XAXIS) - 1 + C.size(MD_Cubo::YAXIS) - 1) * 2;
    state = S_LOADC;
    break;

  case S_LOADC: // load a character from the font table
    PRINTS("\nS_LOADC");
    if (*cp == '\0')  // reached end of message
    {
      PRINTS(" - end mesg reached");
      state = S_CLEANUP;
    }
    else                // load a character into the buffer
    {
      PRINT(" - loading '", *cp);
      size = getFontChar(*cp++, ARRAY_SIZE(cBuf), cBuf);
      cBuf[size++] = 0;   // inter character blank column
      PRINT("' size ", size);
      getCol = 0;
      state = S_SCROLL;
    }
    break;

  case S_SCROLL: // insert a column into the start and scroll along
    PRINTS("\nS_SCROLL");
    scrollFaces();

    // add in the new column
    for (uint8_t z = 0; z < 8; z++)    // only 8 bits in the height of the font
      C.setVoxel(((cBuf[getCol] & (1 << z)) != 0) ? VOX_ON :VOX_OFF, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1 - z);
    getCol++;

    // update the display
    C.update();
    C.animate(0);
    state = S_WAITING;
    break;

  case S_WAITING: // initialization stage for S_TIMER
    timeStart = millis();
    state = S_TIMER;
    break;

  case S_TIMER: // non blocking wait for the scrolling time to expire
    //PRINTS("\nS_WAITING");
    // finished waiting?
    if (millis() - timeStart < timeWait)
    {
      C.animate(0);
      break;
    }

    // move to a state to do something
    PRINTS("\nS_WAITING Over");
    if (getCol == size) // all the columns processed
    {
      if (*cp == '\0')  // no characters left - clean up
        state = S_CLEANUP;
      else              // otherwise get the next char
        state = S_LOADC;
    }
    else                // more columns to process - do it
      state = S_SCROLL;
    break;

  case S_CLEANUP: // keep scrolling until the message disappears
    PRINTS("\nS_CLEANUP");
    scrollFaces();
    C.update();
    C.animate(0);

    if (countScroll-- != 0)
      state = S_WAITING;
    else
      state = S_END;
    break;

  case S_END:   // nothing left to do except wait for another message
    // PRINTS("\nS_END");
    break;
  }

  return (state == S_END);
}

void scrollingText()
{
  //char message[] = "MD_Cubo         Demo";
  char message[] = "MD_      Cubo         Demo";

  // this only works for larger cubes, so return if the cube is not big enough
  if ((C.size(MD_Cubo::XAXIS) < 8 || C.size(MD_Cubo::YAXIS) < 8 || C.size(MD_Cubo::ZAXIS) < 8))
    return;

  PRINTS("\nScrolling Text")
  C.clear();    // new clean cube
  displayMessage(message, 100);

  while (!displayMessage(NULL))
    ;
}



//*********************************************************************************************************************************************************************************************************
//Below are program codes from other LED cube projects adapted for additional animation effects
const unsigned char bitmaps[12][8] PROGMEM= 
{
  {0x37,0x37,0x04,0x34,0x36,0x34,0xb4,0x64},  //  0 - JF#1
  {0x37,0x04,0x34,0x36,0x34,0xb4,0x64,0x00},  //  1 - JF#2
  {0x34,0x36,0x34,0xb4,0x64,0x00,0x00,0x00},  //  2 - JF#3
  {0x34,0xb4,0x64,0x00,0x00,0x00,0x00,0x00},  //  3 - JF#4
  {0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  //  4 - JF#5
  {0x66,0x66,0x00,0x00,0x81,0xc3,0x7e,0x3c},  //  5 - Smiley
  {0x00,0x04,0x06,0xff,0xff,0x06,0x04,0x00},  //  6 - Arrow
  {0x00,0x9c,0xa2,0xc5,0xc1,0xa2,0x9c,0x00},  //  7 - Fish
  {0x7e,0xbd,0x02,0x1c,0xa1,0x5a,0x24,0x18},  //  8 - Superman
  {0x00,0x22,0x77,0x7f,0x3e,0x3e,0x1c,0x08},  //  9 - Heart
  {0x1C,0x22,0x55,0x49,0x5d,0x22,0x1c,0x00},  // 10 - Face
  {0x18,0x3c,0x7e,0xdb,0xff,0x24,0x5a,0xa5},  // 11 - Space Invader
};

const unsigned char paths[44] PROGMEM = 
{
  // circle, len 16, offset 28
  0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x10,0x20,0x30,
  0x40,0x50,0x60,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
  0x67,0x57,0x47,0x37,0x27,0x17,0x04,0x03,0x12,0x21,0x30,
  0x40,0x51,0x62,0x73,0x74,0x65,0x56,0x47,0x37,0x26,0x15
};

void font_getpath (unsigned char path, unsigned char *destination, int length)
{
  int i;
  int offset = 0;

  if (path == 1)
    offset=28;

  for (i = 0; i < length; i++)
    destination[i] = pgm_read_byte(&paths[i+offset]);
}

void font_getbitmap (char bitmap, unsigned char dst[8])
{
  int i;

  for (i = 0; i < 8; i++)
    dst[i] = (unsigned char)pgm_read_byte((uint8_t*)&bitmaps[(uint8_t)bitmap][(uint8_t)i]);
}

unsigned char font_getbitmappixel ( char bitmap, char x, char y)
{
  uint8_t tmp = pgm_read_byte((uint8_t*)&bitmaps[(uint8_t)bitmap][(uint8_t)x]);

  return (tmp >> y) & 0x01;
}


void effect_pathmove (unsigned char *path, int length)
{
  int i,z;

  for (i=(length-1);i>=1;i--)
  {
    for (z=0;z<8;z++)
    {
      uint32_t state = C.getVoxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
      C.setVoxel(state,((path[i]>>4) & 0x0f), (path[i] & 0x0f), z);
    }
  }
  for (i=0;i<8;i++)
    C.setVoxel(VOX_OFF,((path[0]>>4) & 0x0f), (path[0] & 0x0f),i);
}

#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3

// Shift the entire contents of the cube along an axis
// This is great for effects where you want to draw something
// on one side of the cube and have it flow towards the other
// side. Like rain flowing down the Z axis.
void shift(char axis, int direction)
{
  int i, x ,y;
  int ii, iii;
  int state;

  for (i = 0; i < 8; i++)
  {
    if (direction == -1)
    {
      ii = i;
    } 
    else
    {
      ii = (7-i);
    }

    for (x = 0; x < 8; x++)
    {
      for (y = 0; y < 8; y++)
      {
        if (direction == -1)
        {
          iii = ii+1;
        } 
        else
        {
          iii = ii-1;
        }

        if (axis == AXIS_Z)
        {
          uint32_t state = C.getVoxel(x,y,iii);
          C.setVoxel(state,x,y,ii);
        }

        if (axis == AXIS_Y)
        {
          uint32_t state = C.getVoxel(x,iii,y);
          C.setVoxel(state,x,ii,y);
        }

        if (axis == AXIS_X)
        {
          uint32_t state = C.getVoxel(iii,y,x);
          C.setVoxel(state,ii,y,x);
        }
      }
    }
  }

  if (direction == -1)
  {
    i = 7;
  } 
  else
  {
    i = 0;
  }	

  for (x = 0; x < 8; x++)
  {
    for (y = 0; y < 8; y++)
    {
      if (axis == AXIS_Z)
        C.setVoxel(VOX_OFF,x,y,i);
        
      if (axis == AXIS_Y)
        C.setVoxel(VOX_OFF,x,i,y);

      if (axis == AXIS_X)
        C.setVoxel(VOX_OFF,i,y,x);
    }
  }
}

void fireworks()
{
  const uint8_t NUM_PARTICLES = 25;
  float particles[NUM_PARTICLES][6];   // Particles and their position, x,y,z and their movement, dx, dy, dz
  uint8_t orgX, orgY, orgZ;

  // this only works for larger cubes, so return if the cube is not big enough
  if ((C.size(MD_Cubo::XAXIS) < 8 || C.size(MD_Cubo::YAXIS) < 8 || C.size(MD_Cubo::ZAXIS) < 8))
    return;

  PRINTS("\nFireworks");

  timeStart = millis();
  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    C.clear();

    orgX = random(2, C.size(MD_Cubo::XAXIS) - 3);
    orgY = random(2, C.size(MD_Cubo::YAXIS) - 3);
    orgZ = random(C.size(MD_Cubo::ZAXIS) / 2, C.size(MD_Cubo::ZAXIS)-2);

    // shoot a particle up in the air
    for (uint8_t i = 0; i < orgZ; i++)
    {
      C.setVoxel(VOX_ON, orgX, orgY, i);
      C.update();
      C.animate(60 + (30 * i));
      C.setVoxel(VOX_OFF, orgX, orgY, i);
    }

    // Fill particle array
    for (uint8_t i = 0; i < NUM_PARTICLES; i++)
    {
      // Position
      particles[i][0] = orgX;
      particles[i][1] = orgY;
      particles[i][2] = orgZ;

      // Movement
      particles[i][3] = 1.0 - (float)random(200) / 100.0; // dx
      particles[i][4] = 1.0 - (float)random(200) / 100.0; // dy
      particles[i][5] = 1.0 - (float)random(200) / 150.0; // dz            
    }


    uint8_t rand_duration = random(10,25);
    
    for (uint8_t i = 0; i < rand_duration; i++)
    {
      float slowrate = 1 + tan((i + 0.1) / 20) * 10;      
      float gravity = tan((i + 0.1)/20) / 2;

      for (uint8_t j = 0; j < NUM_PARTICLES; j++)
      {
        particles[j][0] += particles[j][3] / slowrate;
        particles[j][1] += particles[j][4] / slowrate;
        particles[j][2] += particles[j][5] / slowrate;
        particles[j][2] -= gravity;

        C.setVoxel(VOX_ON, particles[j][0], particles[j][1], particles[j][2]);
      }
      
      C.update();
      C.animate(50);
      C.clear();
    }
  }
}

void wormsqueeze()
{
  int size = 2;
  int axis = AXIS_Z;
  int direction = -1;
  int iterations = 100;
  int delay = 1000;

  int x, y, i,j,k, dx, dy;
  int cube_size;
  int origin = 0;

  C.clear();
  if (direction == -1)
    origin = 7;

  cube_size = C.size(MD_Cubo::XAXIS)-(size-1);

  x = rand()%cube_size;
  y = rand()%cube_size;

  for (i=0; i<iterations; i++)
  {
    dx = ((rand()%3)-1);
    dy = ((rand()%3)-1);

    if ((x+dx) > 0 && (x+dx) < cube_size)
      x += dx;

    if ((y+dy) > 0 && (y+dy) < cube_size)
      y += dy;

    shift(axis, direction);

    for (j=0; j<size;j++)
    {
      for (k=0; k<size;k++)
      {
        if (axis == AXIS_Z)
          C.setVoxel(VOX_ON,x+j,y+k,origin);

        if (axis == AXIS_Y)
          C.setVoxel(VOX_ON,x+j,origin,y+k);

        if (axis == AXIS_X)
          C.setVoxel(VOX_ON,origin,y+j,x+k);
      }
    }

    C.update();
    C.animate(100);
  }
}

void CornerToCorner()
{
  unsigned long STEP_TIME = 1000;
  unsigned long timeSinceLastStep = 0;
  int whatstep = 0;
  boolean growing = true;
  boolean xHigh = random(2);
  boolean yHigh = random(2);
  boolean zHigh = random(2);  
  
  C.clear();
  timeStart = millis();

  while (millis() - timeStart <= DEMO_RUNTIME)
  {
    unsigned long dt = 1;

    timeSinceLastStep += dt;
    if (timeSinceLastStep > STEP_TIME)
    {
      if(growing)
      {
        for(uint8_t x = 0 ; x < C.size(MD_Cubo::XAXIS) ; x++)
        {
          for(uint8_t y = 0 ; y < C.size(MD_Cubo::YAXIS) ; y++)
          {
            for(uint8_t z = 0 ; z < C.size(MD_Cubo::ZAXIS) ; z++)
            {
              int xDist = x, yDist = y, zDist = z;
              if(xHigh) xDist = C.size(MD_Cubo::XAXIS) - x - 1;
              if(yHigh) yDist = C.size(MD_Cubo::YAXIS) - y - 1;
              if(zHigh) zDist = C.size(MD_Cubo::ZAXIS) - z - 1;
              
              C.setVoxel((xDist + yDist + zDist <= whatstep) ? VOX_ON : VOX_OFF, x, y, z);
            }
          }
        }
      }
      whatstep++;
      if(whatstep > (C.size(MD_Cubo::XAXIS)-1)*3)
      {
        whatstep = 0;
        growing = false;
      }
    }
    else
    {
      for(uint8_t x = 0 ; x < C.size(MD_Cubo::XAXIS) ; x++)
      {
        for(uint8_t y = 0 ; y < C.size(MD_Cubo::YAXIS) ; y++)
        {
          for(uint8_t z = 0 ; z < C.size(MD_Cubo::ZAXIS) ; z++)
          {
            int xDist = x, yDist = y, zDist = z;
            if(xHigh) xDist = C.size(MD_Cubo::XAXIS) - x - 1;
            if(yHigh) yDist = C.size(MD_Cubo::YAXIS) - y - 1;
            if(zHigh) zDist = C.size(MD_Cubo::ZAXIS) - z - 1;

            C.setVoxel((xDist + yDist + zDist <= whatstep) ? VOX_OFF : VOX_ON,x,y,z);
          }
        }
      }
      whatstep++;
      if(whatstep > (C.size(MD_Cubo::XAXIS)-1)*3)
      {
        whatstep = 0;
        growing = true;
        xHigh = random(2);
        yHigh = random(2);
        zHigh = random(2);
      }
    }
    timeSinceLastStep -= STEP_TIME;

    C.update();
    C.animate(50);
  }
}


void effect_bitmapspin (int count, int delay, char bitmap)
{
  unsigned char dybde[] = {0,1,2,3,4,5,6,7,1,1,2,3,4,5,6,6,2,2,3,3,4,4,5,5,3,3,3,3,4,4,4,4};
  int d = 0;
  int flip = 0;
  int x, y, off;

  C.clear();
  for(int i = 0; i<count; i++)
  {
    flip = 0;
    d = 0;
    off = 0;
    // front:
    for (int s=0;s<7;s++)
    {
      if(!flip)
      {
        off++;
        if (off == 4)
        {
          flip = 1;
          off = 0;
        }
      } 
      else 
      {
        off++;
      }
      for (x=0; x<8; x++)
      {
        d = 0;
        for (y=0; y<8; y++)
        {
          if (font_getbitmappixel ( bitmap, 7-x, y))
          {
            if (!flip)
              C.setVoxel(VOX_ON,y,dybde[8 * off + d++],x);
            else
              C.setVoxel(VOX_ON,y,dybde[31 - 8 * off - d++],x);
          }
          else
          {
            d++;
          }
        }
      }
      C.update();
      C.animate(delay);
      C.clear();
    }

    // side:
    off = 0;
    flip = 0;
    d = 0;
    for (int s=0;s<7;s++)
    {
      if(!flip)
      {
        off++;
        if (off == 4)
        {
          flip = 1;
          off = 0;
        }
      }
      else
      {
        off++;
      }
      for (x=0; x<8; x++)
      {
        d = 0;
        for (y=0; y<8; y++)
	      {
          if (font_getbitmappixel ( bitmap, 7-x, y))
          {
            if (!flip)
              C.setVoxel(VOX_ON,dybde[8 * off + d++], 7 - y,x);
            else
              C.setVoxel(VOX_ON,dybde[31 - 8 * off - d++],7 - y,x);
          } 
          else
          {
            d++;
          }
        }
      }
      C.update();
      C.animate(delay);
      C.clear();
    }

    // back:
    flip = 0;
    d = 0;
    off = 0;
    for (int s=0;s<7;s++)
    {
      if(!flip)
      {
        off++;
        if (off == 4)
        {
          flip = 1;
          off = 0;
        }
      }
      else
      {
        off++;
      }
      for (x=0; x<8; x++)
      {
        d = 0;
        for (y=0; y<8; y++)
        {
          if (font_getbitmappixel ( bitmap, 7-x, 7-y))
          {
            if (!flip)
              C.setVoxel(VOX_ON,y,dybde[8 * off + d++],x);
            else
              C.setVoxel(VOX_ON,y,dybde[31 - 8 * off - d++],x);
          }
          else
          {
            d++;
          }
        }
      }
      C.update();
      C.animate(delay);
      C.clear();
    }

    // other side:
    off = 0;
    flip = 0;
    d = 0;
    for (int s=0;s<7;s++)
    {
      if(!flip)
      {
        off++;
        if (off == 4)
        {
          flip = 1;
          off = 0;
        }
      }
      else
      {
        off++;
      }
      for (x=0; x<8; x++)
      {
        d = 0;
        for (y=0; y<8; y++)
	      {
          if (font_getbitmappixel ( bitmap, 7-x, 7-y))
          {
            if (!flip)
              C.setVoxel(VOX_ON,dybde[8 * off + d++], 7 - y,x);
            else
              C.setVoxel(VOX_ON,dybde[31 - 8 * off - d++],7 - y,x);
          }
          else
          {
            d++;
          }
        }
      }
      C.update();
      C.animate(delay);
      C.clear();
    }
  }
}

void effect_path_bitmap(int delay, char bitmap, int iterations)
{
  int z, i, ii;
  z = 4;
  unsigned char path[28];
  font_getpath(0,path,28);

  C.clear();
  for (i=0; i < iterations; i++)
  {
    for (ii=0;ii<8;ii++)
    {
      for (z=0;z<8;z++)
      {
        if (font_getbitmappixel(bitmap,(7-z),ii))
        {
          C.setVoxel(VOX_ON,0,7,z);
        }
        else
        {
          C.setVoxel(VOX_OFF,0,7,z);
        }
      }
      effect_pathmove(path, 28);
      C.update();
      C.animate(delay);
    }
   
    for (ii=0;ii<20;ii++)
    {
      effect_pathmove(path, 28);
      C.update();
      C.animate(delay);
    }
  }

  for (ii=0;ii<10;ii++)
  {
    effect_pathmove(path, 28);
    C.update();
    C.animate(delay);
  }
}

void effect_path_bitmapSequence1()
{
  effect_path_bitmap (50,7,2);  // 7 - fish
  effect_path_bitmap (50,9,2);  // 9 - heart
}

void effect_path_bitmapSequence2()
{
  effect_path_bitmap (80,6,2);   // 6 - arrow
  effect_path_bitmap (80,11,2);  // 11 - space invader
}

void effect_bitmapspinSequence1()
{
  effect_bitmapspin(3,50,0);  // 0 - JF#1
  effect_bitmapspin(1,5,1);   // 1 - JF#2
  effect_bitmapspin(1,4,2);   // 2 - JF#3
  effect_bitmapspin(1,3,3);   // 3 - JF#4
  effect_bitmapspin(1,1,4);   // 4 - JF#5 
}

void effect_bitmapspinSequence2()
{
  effect_bitmapspin(2,50,5);  // 5 - smiley
  effect_bitmapspin(2,50,6);  // 6 - Arrow
  effect_bitmapspin(2,50,8);  // 8 - Superman
  effect_bitmapspin(2,40,9);  // 9 - Heart
  effect_bitmapspin(2,30,10); // 10 - face
}

void setup()
{  
#if DEBUG
  Serial.begin(57600);

  PRINTS("\n[jolliFactory 8x8x8 jolliCube MD_Cubo library Demo example 1.0]");
#endif
  C.begin();

  randomSeed(analogRead(A0) * analogRead(A5));  // sort of gives different start results.
}


void loop()
{
  static int8_t old_choice = -1;

  void (*demoType[])(void) =
  {    
    //Animation effects adapted from other LED cube projects
    fireworks,
    effect_bitmapspinSequence1,
    recedingText,
    scrollingText,
    effect_path_bitmapSequence1,
    wormsqueeze,
    CornerToCorner,
    effect_bitmapspinSequence2,
    effect_path_bitmapSequence2,


    //Animation effects from MD_Cubo library
    oscillation, 
    firefly, 
    intersectPlanes, 
    brownian, 
    ripples, 
    outsideStack, 
    //recedingText, 
    rain, 
    shrinkCube, 
    randomFill, 
    flagwave, 
    slideFaces, 
    hourglass, 
    //scrollingText, 
    wrapFaces, 
    suspension, 
    spiralLine, 
    boingCube, 
    droplets    
  };

#if RANDOM_CYCLE
  uint8_t choice;

  do
    choice = random(ARRAY_SIZE(demoType));
  while (choice == old_choice);
  old_choice = choice;
#else
  old_choice = (old_choice + 1) % ARRAY_SIZE(demoType);
#endif // RANDOM_CYCLE

  demoType[old_choice]();
}
