#include <Arduino.h>
#include "ColorShifter.h"

/**
   \file
   \brief Main file for the ColorShifter class
*/

#define	CS_DEBUG	1		///< Enable or disable (default) debugging output

#if CS_DEBUG
#define	PRINT(s, v)		{ Serial.print(F(s)); Serial.print(v); }		  ///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)	{ Serial.print(F(s)); Serial.print(v, HEX); }	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)	{ Serial.print(F(s)); Serial.print(v, BIN); }	///< Print a string followed by a value (binary)
#define	PRINTS(s)		  { Serial.print(F(s)); }						///< Print a string
#define PRINTC(s, r, g, b) \
  { \
    Serial.print(F(s)); Serial.print(F("(0x")); Serial.print(r, HEX); \
    Serial.print(F(s)); Serial.print(F(",0x")); Serial.print(g, HEX); \
    Serial.print(F(s)); Serial.print(F(",0x")); Serial.print(b, HEX); \ 
    Serial.print(F(")")); \
  }	///< Print an RGB color
#else
#define	PRINT(s, v)		///< Print a string followed by a value (decimal)
#define	PRINTX(s, v)	///< Print a string followed by a value (hex)
#define	PRINTB(s, v)	///< Print a string followed by a value (binary)
#define	PRINTS(s)		  ///< Print a string
#define PRINTC(s, r, g, b) 	///< Print an RGB color
#endif

ColorShifter::ColorShifter(uint32_t* shiftColors, uint8_t shiftColorCount, int16_t stepWidth, uint8_t finishMode) :
_shiftColors(shiftColors), _colorCount(shiftColorCount), _stepWidth(stepWidth), _finishMode(finishMode),
_nextColorIndex(1), _backwards(false), _isDone(false)
{
  _currentR = R(shiftColors[0]);
  _currentG = G(shiftColors[0]);
  _currentB = B(shiftColors[0]);
  _nextR    = R(shiftColors[1]);
  _nextG    = G(shiftColors[1]);
  _nextB    = B(shiftColors[1]);
};

uint32_t ColorShifter::shiftSingleColor(uint8_t current, uint8_t next, int16_t width)
{
  if (next > current) 
  {
    if (255 - width < current || current + width > next )
      current = next;
    else 
      current += width;
  }
  if (next < current) 
  {
    if (width  > current || current - width < next)
      current = next;
    else 
      current -= width;
  }

  return((uint32_t) current);
}

uint32_t ColorShifter::dim(uint32_t color, int16_t stepsize)
{
  uint8_t goal;

  if (stepsize > 0)
    goal = 255;
  else
    goal = 0;

  stepsize = abs(stepsize);

  return RGB(shiftSingleColor(R(color), goal, stepsize),
             shiftSingleColor(G(color), goal, stepsize),
             shiftSingleColor(B(color), goal, stepsize));
}

uint32_t ColorShifter::shift(void)
{
  PRINTC("\nIn:", (uint8_t)_currentR, (uint8_t)_currentG, (uint8_t)_currentB);
  if (!_isDone)
  {
    _currentR = shiftSingleColor(_currentR, _nextR, _stepWidth);
    _currentG = shiftSingleColor(_currentG, _nextG, _stepWidth);
    _currentB = shiftSingleColor(_currentB, _nextB, _stepWidth);

    if ((_currentR == _nextR) &&
        (_currentG == _nextG) &&
        (_currentB == _nextB))
    {
      if (!_backwards)
      {
        _nextColorIndex++;
        if (_nextColorIndex > (_colorCount - 1))
        {
          switch (_finishMode)
          {
          case 0:   // do once only
              _isDone = true;
              PRINTS("\nDone.");
              _nextColorIndex--;
            break;

          case 1:   // wrap around
            _nextColorIndex = 0;
            break;

          case 2:   // reverse
            _nextColorIndex -= 1;
            _backwards = !_backwards;
            break;
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

      _nextR = R(_shiftColors[_nextColorIndex]);
      _nextG = G(_shiftColors[_nextColorIndex]);
      _nextB = B(_shiftColors[_nextColorIndex]);
      PRINTC("\nNext color: ", _nextR, _nextG, _nextB);
    }
  }

  return RGB (_currentR, _currentG, _currentB);
}