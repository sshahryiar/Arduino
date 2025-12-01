#include "Arduino.h"
#include "LM35.h"


LM35::LM35(int pin, float vdd, int adc_cnt, int samples)
{
  _pin = pin;
  _vdd = vdd;
  _samples = samples;
  _adc_cnt = adc_cnt;
}


int LM35::raw_value(void)
{
  return (analogRead(_pin));
}


float LM35::get_T(void)
{
    float scale = ((10.0 * _adc_cnt) / (_vdd * 1000.0));
    int tmp = 0x0000;
    char i = 0x00;

    for(i = 0; i < _samples; i++)
    {
        tmp += raw_value();
        delayMicroseconds(20);
    }
    
    return ((float)tmp / (scale * (float)_samples));
}