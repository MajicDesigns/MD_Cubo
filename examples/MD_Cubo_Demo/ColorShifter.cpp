#include <Arduino.h>
#include "ColorShifter.h"

/**
   \file
   \brief Main file for the ColorShifter class
*/

#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }      // Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } // Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } // Print a string followed by a value (binary)
#define PRINTS(s)     { Serial.print(F(s)); }                       // Print a string
#define PRINTC(s, x, y, z) { PRINTS(s); PRINT("(",x); PRINT(",",y); PRINT(",",z); PRINTS(")"); }  // Print coordinate tuple


ColorShifter::ColorShifter(uint32_t* shiftColors, uint8_t shiftColorCount, float stepWidth, uint8_t finishMode)
{
  _currentR = (float)R(shiftColors[0]);
  _currentG = (float)G(shiftColors[0]);
  _currentB = (float)B(shiftColors[0]);
  _nextR = (float)R(shiftColors[1]);
  _nextG = (float)G(shiftColors[1]);
  _nextB = (float)B(shiftColors[1]);
  _shiftColors = shiftColors;
  _colorCount = shiftColorCount;
  _stepWidth = stepWidth;
  _nextColorIndex = 1;
  _backwards = false;
  _isDone = false;
  _finishMode = finishMode;
};

static float ColorShifter::shiftSingleColor(float current, float next, float width)
{
  if ((byte)current != (byte) next) 
  {
    if ( next > current) 
    {
      if ( (float)255 - width < current || current + width > next )
        current = next;
      else 
        current += width;
    }
    if ( next < current) 
    {
      if ( (float)0 + width  > current || current - width < next)
        current = next;
      else 
        current -= width;
    }
  }
  return current;
}

uint32_t ColorShifter::dim(uint32_t color, float stepsize)
{
  float goal;

  if (stepsize > 0)
    goal = 255;
  else
    goal = 0;

  stepsize = abs(stepsize);
//  PRINTX("\nIn:", x, y, z)  
  return RGB(shiftSingleColor(R(color),goal,stepsize),shiftSingleColor(G(color),goal,stepsize),shiftSingleColor(B(color),goal,stepsize));
}

uint32_t ColorShifter::shift(void)
{
  //PRINTC("\nIn:", _currentR, _currentG, _currentB);
  if (_isDone)
    return RGB ((uint8_t) _currentR, (uint8_t) _currentG, (uint8_t) _currentB);

  _currentR = shiftSingleColor(_currentR, _nextR, _stepWidth);
  _currentG = shiftSingleColor(_currentG, _nextG, _stepWidth);
  _currentB = shiftSingleColor(_currentB, _nextB, _stepWidth);

  if (((byte)_currentR == (byte) _nextR) && ((byte)_currentG == (byte) _nextG) && ((byte)_currentB == (byte) _nextB)) {
    if (!_backwards) 
    {
      _nextColorIndex++;
      if (_nextColorIndex > ( _colorCount - 1 ))
      {
        if (_finishMode == 0) 
        {
          _isDone = true;
          PRINTS("\nDone.");
          _nextColorIndex--;
        } 
        else if (_finishMode == 1) 
        {
          _nextColorIndex = 0;
        } 
        else if (_finishMode == 2) 
        {
          _nextColorIndex -= 1;
          _backwards = !_backwards;
        }
      }
    }
    if (_backwards) 
    {
      if (_nextColorIndex == 0) 
      {
        _nextColorIndex = 1;
        _backwards = !_backwards;
      } 
      else 
        _nextColorIndex--;
    }

    _nextR = (float)R(_shiftColors[_nextColorIndex]);
    _nextG = (float)G(_shiftColors[_nextColorIndex]);
    _nextB = (float)B(_shiftColors[_nextColorIndex]);
    PRINTC("\nNext color: ", _nextR, _nextG, _nextB);
  }

  return RGB ((uint8_t) _currentR, (uint8_t) _currentG, (uint8_t) _currentB);
}
