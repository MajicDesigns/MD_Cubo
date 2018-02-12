#include <Arduino.h>
#include "ColorShifter.h"

ColorShifter::ColorShifter(uint32_t[] shiftColors, float stepWidth, uint8_t finishMode) 
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

ColorShifter::shift()
{ 
    if (isDone) {
        return RGB ((uint8_t) _currentR,(uint8_t) _currentG, (uint8_t) _currentB);
    }
   if ((byte)_currentR != (byte) _nextR) {
     if ( _nextR > _currentR) {
        if ( _currentR + _stepWidth > _nextR )
        {
           _currentR = _nextR; 
        }
        else {
            _currentR += _stepWidth
        }
     }
     if ( _nextR < _currentR) {
        if ( _currentR - _stepWidth < _nextR )
        {
           _currentR = _nextR; 
        }
        else {
            _currentR -= _stepWidth
        }
     }
   }

   if ((byte)_currentG != (byte) _nextG) {
     if ( _nextG > _currentG) {
        if ( _currentG + _stepWidth > _nextG )
        {
           _currentG = _nextG; 
        }
        else {
            _currentG += _stepWidth
        }
     }
     if ( _nextG < _currentG) {
        if ( _currentG - _stepWidth < _nextG )
        {
           _currentG = _nextG; 
        }
        else {
            _currentG -= _stepWidth
        }
     }
   }
   
   if ((byte)_currentB != (byte) _nextB) {
     if ( _nextB > _currentB) {
        if ( _currentB + _stepWidth > _nextB )
        {
           _currentB = _nextR; 
        }
        else {
            _currentB += _stepWidth
        }
     }
     if ( _nextB < _currentB) {
        if ( _currentB - _stepWidth < _nextB )
        {
           _currentB = _nextB; 
        }
        else {
            _currentB -= _stepWidth
        }
     }
   }      
   if (((byte)_currentR = (byte) _nextR) && ((byte)_currentG = (byte) _nextG) && ((byte)_currentB = (byte) _nextB)) {
      if (!_backwards) {
          _nextColorIndex++; 
          if (_nextColorIndex > (ARRAY_SIZE (_shiftColors)-1))
          {
            if (_finishMode==0) {
                isDone = true;
                _nextColorIndex--;
            }
            elseif (_finishMode==1) {
                _nextColorIndex=1;
            }
            elseif (_finishMode==2) {
                _nextColorIndex-=2;
                _backwards = !_backwards;
            }
          }
      }
        if (_backwards) {
          _nextColorIndex--; 
          if (_nextColorIndex < 0) {
                _nextColorIndex=1;
                _backwards = !_backwards;
            }
          }
      
     _nextR = (float)R(_shiftColors[_nextColorIndex]);
       _nextG = (float)G(_shiftColors[_nextColorIndex]);
       _nextB = (float)B(_shiftColors[_nextColorIndex]);
       
   }


  return RGB ((uint8_t) _currentR,(uint8_t) _currentG, (uint8_t) _currentB);
}