#pragma once

#include <Arduino.h>


#define ZP07_clear_air            0
#define ZP07_light_pollution      1
#define ZP07_high_pollution       2
#define ZP07_heavy_pollution      3

#define ZP07_air_quality_100      100
#define ZP07_air_quality_75       75
#define ZP07_air_quality_50       50
#define ZP07_air_quality_25       25


class ZP07
{
  public:
    ZP07(int pin_A, int pin_B);
    void begin(void);
    uint8_t read(void);

  private:
    int _pin_A;
    int _pin_B;
};