// LED Cube outer face scrolling text demo
// 
// Letters appear to scroll in at one corner of the cube and 
// scroll across all the faces until they disappear at the other 
// side of the corner they entered.
// Font file used is the same as for the MD_MAX72xx library at
// https://github.com/MajicDesigns/MD_MAX72xx
// Font builder utility and documentation on the font format can be 
// found with the MD_MAX72xx library.
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
#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }      // Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } // Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } // Print a string followed by a value (binary)
#define PRINTS(s)     { Serial.print(F(s)); }     // Print a string
#define PRINTC(s, x, y, z) { PRINTS(s); PRINT("(",x); PRINT(",",y); PRINT(",",z); PRINTS(")"); }  // Print coordinate tuple
#else
#define PRINT(s, v)   ///< Print a string followed by a value (decimal)
#define PRINTX(s, v)  ///< Print a string followed by a value (hex)
#define PRINTB(s, v)  ///< Print a string followed by a value (binary)
#define PRINTS(s)     ///< Print a string
#define PRINTC(s, x, y, z)  ///< Print coordinate tuple
#endif

// Global message buffers shared by Serial and Scrolling functions
#define	BUF_SIZE	75
char curMessage[BUF_SIZE] = "Hello!";
char newMessage[BUF_SIZE] = "";
bool newMessageAvailable = false;

void readSerial(void)
{
  static uint8_t	putIndex = 0;

  while (Serial.available())
  {
    newMessage[putIndex] = (char)Serial.read();
    if ((newMessage[putIndex] == '\n') || (putIndex >= BUF_SIZE - 3))	// end of message character or full buffer
    {
      // put in a message separator and end the string
      newMessage[putIndex++] = ' ';
      newMessage[putIndex] = '\0';
      // restart the index for next filling spree and flag we have a message waiting
      putIndex = 0;
      newMessageAvailable = true;
    }
    else
      // Just save the next char in next location
      newMessage[putIndex++];
  }
}

void scrollFaces(void)
// scroll all the columns across, starting with the second last and working backwards
{
  // XZ Plane at Ymax
  for (int8_t x = 1; x < C.size(MD_Cubo::XAXIS); x++)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      boolean p = C.getVoxel(x + 1, C.size(MD_Cubo::YAXIS) - 1, z);
      C.setVoxel(p, x, C.size(MD_Cubo::YAXIS) - 1, z);
    }
  // YZ Plane at Xmax
  for (int8_t y = C.size(MD_Cubo::YAXIS) - 2; y >= 0; y--)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      boolean p = C.getVoxel(C.size(MD_Cubo::XAXIS) - 1, y, z);
      C.setVoxel(p, C.size(MD_Cubo::XAXIS) - 1, y + 1, z);
    }
  // XZ Plane at Y0
  for (int8_t x = C.size(MD_Cubo::XAXIS) - 2; x >= 0; x--)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      boolean p = C.getVoxel(x, 0, z);
      C.setVoxel(p, x + 1, 0, z);
    }
  // YZ Plane at X0
  for (int8_t y = 1; y < C.size(MD_Cubo::YAXIS); y++)
    for (uint8_t z = 0; z < C.size(MD_Cubo::ZAXIS); z++)
    {
      boolean p = C.getVoxel(0, y, z);
      C.setVoxel(p, 0, y - 1, z);
    }
}

boolean displayMessage(char *mesg, uint16_t delay = 0)
// Display a message on the surface of the cube, scrolling around the outside faces
// Implemented as a non-blocking Finte State Machine.
// Returns true when the message has completed.
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
  case S_INIT: // initialisation of a new message
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
      C.setVoxel((cBuf[getCol] & (1 << z)) != 0, 0, C.size(MD_Cubo::YAXIS) - 1, C.size(MD_Cubo::ZAXIS) - 1 - z);
    getCol++;

    // update the display
    C.update();
    C.animate(0);
    state = S_WAITING;
    break;

  case S_WAITING: // initialisation stage for S_TIMER
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

void setup()
{
  Serial.begin(57600);
  Serial.print("\n[MD_Cubo Text Scrolling Demo]\nType a message for the scrolling display\nEnd message line with a newline");
  C.begin();
}

void loop()
{
  if (displayMessage(NULL))
  {
    if (newMessageAvailable)	// there is a new message waiting
    {
      strcpy(curMessage, newMessage);	// copy it in
      newMessageAvailable = false;
    }

    C.clear();    // new clean cube
    displayMessage(curMessage, 100);
  }

  // do other stuff ...
  readSerial();
}

