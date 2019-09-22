#include <MD_Cubo.h>
#include "MD_Cubo_8x8_ICS574.h"

// Library for the ICS574 by Ruud vd Meer the Netherlands.
// DEBUG must be 0, works not with this HW.

volatile unsigned char cube[8][8];
volatile int current_layer = 0;

// everything works direct on the uP ports.
void MD_Cubo_ICS574::begin(void)
{
  int i;
  
  for(i=0; i<14; i++)
    pinMode(i, OUTPUT);
  
  // pinMode(A0, OUTPUT) as specified in the arduino reference didn't work. So I accessed the registers directly.
  DDRC = 0xff;
  PORTC = 0x00;
  
  // Reset any PWM configuration that the arduino may have set up automagically!
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  TCCR2A |= (0x01 << WGM21); // CTC mode. clear counter on TCNT2 == OCR2A
  OCR2A = 10; // Interrupt every 25600th cpu cycle (256*100)
  TCNT2 = 0x00; // start counting at 0
  TCCR2B |= (0x01 << CS22) | (0x01 << CS21); // Start the clock with a 256 prescaler
  TIMSK2 |= (0x01 << OCIE2A);
}

void MD_Cubo_ICS574::animate(uint32_t wait)
// a simple wait, need nothing more.
{
  delay(wait);
}

// VOX_OFF or VOX_ON x,y,z  Led On or Off
void MD_Cubo_ICS574::setVoxel(uint32_t p, uint8_t x, uint8_t y, uint8_t z){
  if ((x >= CUBE_XSIZE) || (y >= CUBE_YSIZE) || (z >= CUBE_ZSIZE))
    return;   
  
  cube[z][7-x] &= ~(1 << y);// led off
  if (p != VOX_OFF) 
    cube[z][7-x] |= (1 << y);// led on
}

// is x,y,z LED ON or OFF??
uint32_t MD_Cubo_ICS574::getVoxel(uint8_t x, uint8_t y, uint8_t z){
  if ((x >= CUBE_XSIZE) || (y >= CUBE_YSIZE) || (z >= CUBE_ZSIZE)) return(VOX_OFF);
    return((cube[z][7-x] & (1 << y)) ? VOX_ON : VOX_OFF);
}

// size of the used Cube.
uint8_t MD_Cubo_ICS574::size(axis_t axis)
{
  switch(axis)
  {
    case XAXIS: return(CUBE_XSIZE);
    case YAXIS: return(CUBE_YSIZE);
    case ZAXIS: return(CUBE_ZSIZE);
  }
}

// direct port control to the uP.
ISR(TIMER2_COMPA_vect)
{
  int i;
  
  // all layer selects off
  PORTC = 0x00;
  PORTB &= 0x0f;
  PORTB |= 0x08; // output enable off.
  for (i=0; i<8; i++) 
  {
    PORTD = cube[current_layer][i];
    PORTB = (PORTB & 0xF8) | (0x07 & (i+1));
  }
  PORTB &= 0b00110111; // Output enable on.
  if      (current_layer <  6) 
    PORTC = (0x01 << current_layer); 
  else if (current_layer == 6) 
    digitalWrite(12, HIGH); 
  else 
    digitalWrite(13, HIGH);
  
  current_layer++;
  if (current_layer == 8) 
    current_layer = 0;
}

// end
