#pragma once


#include <Arduino.h>
#include <Wire.h>


#define SHT2x_I2C_ADDRESS                         0x40

// SHT2x Commands
#define SHT2x_TRIGGER_T_MEASUREMENT_HOLD          0xE3
#define SHT2x_TRIGGER_RH_MEASUREMENT_HOLD         0xE5
#define SHT2x_TRIGGER_T_MEASUREMENT_NO_HOLD       0xF3
#define SHT2x_TRIGGER_RH_MEASUREMENT_NO_HOLD      0xF5
#define SHT2x_WRITE_USER_REGISTER                 0xE6
#define SHT2x_READ_USER_REGISTER                  0xE7
#define SHT2x_SOFT_RESET                          0xFE

// User Register Bits
#define SHT2x_USER_REGISTER_RESOLUTION_RH12_T14   0x00
#define SHT2x_USER_REGISTER_RESOLUTION_RH8_T12    0x01
#define SHT2x_USER_REGISTER_RESOLUTION_RH10_T13   0x80
#define SHT2x_USER_REGISTER_RESOLUTION_RH11_T11   0x81

#define SHT2x_USER_REGISTER_END_OF_BATTERY        0x40
#define SHT2x_USER_REGISTER_HEATER_ENABLE         0x04
#define SHT2x_USER_REGISTER_DISABLE_OTP_RELOAD    0x02


class SHT2x 
{
  public:
    SHT2x();

    bool begin(TwoWire &wirePort = Wire, uint8_t deviceAddress = SHT2x_I2C_ADDRESS);
    float get_T(void);
    float get_RH(void);
    bool read(float &temperature, float &humidity);
    uint8_t read_reg(void);
    bool write_reg(uint8_t value);
    void reset(void);
    bool low_battery(void);
    
    // Configuration methods
    void set_resolution(uint8_t resolution);
    void set_heater(bool enable);
    void disable_OTP_reload(bool disable);
    
  private:
    TwoWire *_i2cPort;
    uint8_t _deviceAddress;
    
    uint16_t read_measurement(uint8_t command);
    float convert_T(uint16_t value);
    float convert_RH(uint16_t value);
    uint8_t crc8(const uint8_t* data, uint8_t length) ;
};
