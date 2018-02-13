#include <Arduino.h>
#include "ColorShifter.h"

#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }    // Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } // Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } // Print a string followed by a value (binary)
#define PRINTS(s)     { Serial.print(F(s)); }           // Print a string
#define PRINTC(s, x, y, z) { PRINTS(s); PRINT("(",x); PRINT(",",y); PRINT(",",z); PRINTS(")"); }  // Print coordinate tuple


ColorShifter::ColorShifter(uint32_t* shiftColors, float stepWidth, uint8_t finishMode)
{
  _currentR = (float)R(shiftColors[0]);
  _currentG = (float)G(shiftColors[0]);
  _currentB = (float)B(shiftColors[0]);
  _nextR = (float)R(shiftColors[1]);
  _nextG = (float)G(shiftColors[1]);
  _nextB = (float)B(shiftColors[1]);
  _stepWidth = stepWidth;
  _nextColorIndex = 1;
  _backwards = false;
  _finishMode = finishMode;
};

float ColorShifter::shiftSingleColor( float current, float next, float width)
{
  if ((byte)current != (byte) next) {
    if ( next > current) {
      if ( (float)255 - width < current || current + width > next )
      {
        current = next;
      }
      else {
        current += width;
      }
    }
    if ( next < current) {
      if ( (float)0 + width  > current || current - width < next)
      {
        current = next;
      }
      else {
        current -= width;
      }
    }
  }
  return current;
}




uint32_t ColorShifter::shift()
{
  if (isDone) {
    return RGB ((uint8_t) _currentR, (uint8_t) _currentG, (uint8_t) _currentB);
  }
  _currentR = shiftSingleColor(_currentR, _nextR, _stepWidth);
  _currentG = shiftSingleColor(_currentG, _nextG, _stepWidth);
  _currentB = shiftSingleColor(_currentB, _nextB, _stepWidth);


  if (((byte)_currentR == (byte) _nextR) && ((byte)_currentG == (byte) _nextG) && ((byte)_currentB == (byte) _nextB)) {
    if (!_backwards) {
      _nextColorIndex++;
      if (_nextColorIndex > (ARRAY_SIZE (_shiftColors) - 1))
      {
        if (_finishMode == 0) {
          isDone = true;
          _nextColorIndex--;
        } else if (_finishMode == 1) {
          _nextColorIndex = 1;
        } else if (_finishMode == 2) {
          _nextColorIndex -= 2;
          _backwards = !_backwards;
        }
      }
    }
    if (_backwards) {
      if (_nextColorIndex == 0) {
        _nextColorIndex = 1;
        _backwards = !_backwards;
      } else {
        _nextColorIndex--;
      }
    }

    _nextR = (float)R(_shiftColors[_nextColorIndex]);
    _nextG = (float)G(_shiftColors[_nextColorIndex]);
    _nextB = (float)B(_shiftColors[_nextColorIndex]);
    PRINT("\nNextColor",ARRAY_SIZE (_shiftColors))

  }
  return RGB ((uint8_t) _currentR, (uint8_t) _currentG, (uint8_t) _currentB);
}
