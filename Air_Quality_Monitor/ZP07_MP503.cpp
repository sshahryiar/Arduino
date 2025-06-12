#include "Arduino.h"
#include "ZP07_MP503.h"


ZP07::ZP07(int pin_A, int pin_B)
{
  _pin_A = pin_A;
  _pin_B = pin_B;
}



void ZP07::begin(void)
{
  pinMode(_pin_A, INPUT);
  pinMode(_pin_B, INPUT);
  delay(1000);
}


uint8_t ZP07::read(void)
{
  if((digitalRead(_pin_A) == false) && (digitalRead(_pin_B) == true))
  {
    return ZP07_light_pollution;
  }

  else if((digitalRead(_pin_A) == true) && (digitalRead(_pin_B) == false))
  {
    return ZP07_high_pollution;
  }

  else if((digitalRead(_pin_A) == true) && (digitalRead(_pin_B) == true))
  {
    return ZP07_heavy_pollution;
  }

  else 
  {
    return ZP07_clear_air;
  }
}