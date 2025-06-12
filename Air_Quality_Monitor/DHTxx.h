#pragma once

#include <Arduino.h>


#define DHT_Okay             0 
#define DHT_Sensor_Error_1   1 
#define DHT_Sensor_Error_2   2 
#define DHT_CRC_Error        3 


#define timeout   1000


enum sensor {
              DHT11 = 0, 
              DHT22 = 1,
              DHT23 = 2,
            };


class DHTxx
{
    public:
        DHTxx(int pin, sensor type = DHT11);
        void begin(void);
        unsigned char get_byte(void);
        unsigned char get_data(float *RH, float *T, float T_Correction = 0, float RH_Correction = 0);

    private:
        int _pin;
        uint_fast32_t dly1;
        uint_fast32_t dly2;
        unsigned char _type;
        unsigned char values[5]; 

        void delay_us(uint_fast32_t value);
        void delay_ms(uint_fast32_t value);
};