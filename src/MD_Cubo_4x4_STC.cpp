#include <MD_Cubo.h>
#include "MD_Cubo_4x4_STC.h"

void MD_Cubo_STC::begin()
{
  byte i;
  Serial.begin(57600);
  for (i = 0; i < 50; i++)
  {
    Serial.write(0xAD);
    delay(100);
  }
  this->clear();
  this->update();
  delay(1000);
}


void MD_Cubo_STC::update()
//Update the cube LEDs
{
  Serial.write(0xEA);
  Serial.write(_columns, COLUMN_COUNT);
  Serial.write(0xEB);
  Serial.write(0xEB);
}


void MD_Cubo_STC::setVoxel(boolean p, uint8_t x, uint8_t y, uint8_t z)
{
  uint8_t pixelcolor;
  if ((x>CUBE_SIZE) || (y>CUBE_SIZE) || (z>CUBE_SIZE))
    return;
  
  if (p==true)
  {
    pixelcolor=0xff;
  }
  else
  {
    pixelcolor=_intensity;
  }
  _columns[x*12+y*48+z*3]=pixelcolor;
  _columns[x*12+y*48+z*3+1]=pixelcolor;
  _columns[x*12+y*48+z*3+2]=pixelcolor;
  
}

boolean MD_Cubo_STC::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  if ((x>CUBE_SIZE) || (y>CUBE_SIZE) || (z>CUBE_SIZE))
    return(false);

  
  return((_columns[x*12+y*48+z*3]+_columns[x*12+y*48+z*3+1]+_columns[x*12+y*48+z*3+2])>0);
}
