#include "Arduino.h"
#include "DHTxx.h"


DHTxx::DHTxx(int pin, sensor type)
{
  _pin = pin;
  _type = type;
}


void DHTxx::begin(void)
{
    pinMode(_pin, INPUT);

    switch(_type)
    {
        case DHT11:
        {
          dly1 = 30;
          dly2 = 26;
          break;
        }
        default:
        {
          dly1 = 32;
          dly2 = dly1;
          break;
        }
    }

    delay(100);
}


unsigned char DHTxx::get_byte(void)
{
    signed char s = 0x08;
    uint_fast32_t t = 0x00000;
    unsigned char value = 0x00;

    while(s > 0)
    {
        value <<= 1;

        t = timeout;
        while((digitalRead(_pin) == false) && (t > 0))
        {
          t--;
          delay_us(1);
        }
        delay_us(dly1);

        if(digitalRead(_pin) == true)
        {
            value |= 1;
        }

        t = timeout;
        while((digitalRead(_pin) == true) && (t > 0))
        {
          t--;
          delay_us(1);
        }

        s--;
    }

    return value;
}


unsigned char DHTxx::get_data(float *RH, float *T, float T_Correction, float RH_Correction)
{
    unsigned char s = 0x00;
    unsigned char check_sum = 0x00;


    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
    digitalWrite(_pin, LOW);
    delay_ms(20);

    digitalWrite(_pin, HIGH);
    pinMode(_pin, INPUT);
    delay_us(dly2);
    
    if(digitalRead(_pin) == true)
    {
        return DHT_Sensor_Error_1;
    }
    delay_us(80);
    
    if(digitalRead(_pin) == false)
    {
        return DHT_Sensor_Error_2;
    }
    delay_us(80);

    for(s = 0; s <= 4; s += 1)
    {
        values[s] = get_byte();
    }
    
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
    
    for(s = 0; s < 4; s += 1)
    {
        check_sum += values[s];
    }
    
    if(check_sum != values[4])
    {
        return DHT_CRC_Error;
    }
    else
    {
        switch(_type)
        {
            case DHT11:
            {
              *RH = ((float)(values[0]) + RH_Correction);
              *T = ((float)(values[2]) + T_Correction);
              break;
            }
            default:
            {
              *RH = ((float)(((values[0] * 256.0) + (values[1])) * 0.1) + RH_Correction);
              *T = ((float)(((values[2] * 256.0) + (values[3])) * 0.1) + T_Correction);
              break;
            }
        }

        return DHT_Okay;
    }  
}


void DHTxx::delay_us(uint_fast32_t value)
{
    register uint_fast32_t t = micros();
    while((micros() - t) < value);
}


void DHTxx::delay_ms(uint_fast32_t value)
{
    register uint_fast32_t t = millis();
    while((millis() - t) < value);
}