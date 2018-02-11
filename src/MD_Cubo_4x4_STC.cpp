#include <MD_Cubo.h>
#include "MD_Cubo_4x4_STC.h"

void MD_Cubo_STC::begin()
{
  byte i;

  _CubeSerial.begin(_bps);
  for (i = 0; i < 50; i++)
  {
    _CubeSerial.write(HW_INIT);
    delay(100);
  }
  this->clear();
  this->update();
  delay(1000); // ### MC20180211 LIBRARIES SHOULD NEVER CONTAIN DELAYS. WHY IS THIS HERE?
}

void MD_Cubo_STC::update()
// Update the cube LEDs
// ### MC20180211 WHAT HAPPENS IF THE RGB VALUES INCLUDE 2 CONSECUTIVE BYTES WITH SAME
// VALUE AS HW_END_MSG??
{
  _CubeSerial.write(HW_START_MSG);
  _CubeSerial.write(_columns, COLUMN_COUNT);
  _CubeSerial.write(HW_END_MSG);
  _CubeSerial.write(HW_END_MSG);
}

void MD_Cubo_STC::setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z)
{
  if ((x > CUBE_SIZE) || (y > CUBE_SIZE) || (z > CUBE_SIZE))
    return;

  // #### MC 20180211 NOT SURE WHICH ORDER RGB IS IN THE TABLE
  // MAY BE BETTER TO HAVE TH TABLE AS A UINT32_T AND JUST SAVE THE VALUE?
  _columns[x*12 + y*48 + z*3] = R(p);
  _columns[x*12 + y*48 + z*3 + 1] = G(p);
  _columns[x*12 + y*48 + z*3 + 2] = B(p);
}

uint32_t MD_Cubo_STC::getVoxel(uint8_t x, uint8_t y, uint8_t z)
{
  if ((x > CUBE_SIZE) || (y > CUBE_SIZE) || (z > CUBE_SIZE))
    return(VOX_OFF);

  // #### MC 20180211 NOT SURE WHICH ORDER RGB IS IN THE TABLE
  // ALSO DEPENDS IF THE TABLE IS JUST UINT32_T. COULD BE EASIER TO HANDLE.
  return(RGB(_columns[x*12+y*48+z*3], _columns[x*12+y*48+z*3+1], _columns[x*12+y*48+z*3+2]));
}
