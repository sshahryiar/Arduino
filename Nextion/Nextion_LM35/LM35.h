#pragma once

#include <Arduino.h>


class LM35
{
    public:
        LM35(int pin = A0, float vdd = 5.0, int adc_cnt = 1023, int samples = 16);
        int raw_value(void);
        float get_T(void);
        
    private:
        int _pin;
        int _adc_cnt;
        int _samples;
        float _vdd;
};