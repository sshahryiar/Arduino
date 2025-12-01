#include "SHT2x.h"


SHT2x::SHT2x() 
{
  _i2cPort = &Wire;
  _deviceAddress = SHT2x_I2C_ADDRESS;
}


bool SHT2x::begin(TwoWire &wirePort, uint8_t deviceAddress) 
{
  uint8_t reg = 0;
  
  _i2cPort = &wirePort;
  _deviceAddress = deviceAddress;
  
  _i2cPort->begin();
  
  reset();
  delay(50);
  disable_OTP_reload(true);
  set_resolution(SHT2x_USER_REGISTER_RESOLUTION_RH12_T14);
  set_heater(true);
  delay(1000);
  set_heater(false);
  
  reg = read_reg();
  return (reg != 0xFF); 
}

float SHT2x::get_T() 
{
  uint16_t value = read_measurement(SHT2x_TRIGGER_T_MEASUREMENT_HOLD);
  
  if(value == 0xFFFF) 
  {
    return NAN;
  }
  
  return convert_T(value);
}


float SHT2x::get_RH() 
{
  uint16_t value = read_measurement(SHT2x_TRIGGER_RH_MEASUREMENT_HOLD);
  
  if(value == 0xFFFF) 
  {
    return NAN; 
  }

  return convert_RH(value);
}


bool SHT2x::read(float &temperature, float &humidity) 
{
  uint16_t raw_T = 0;
  uint16_t raw_RH = 0;

  reset();
  delay(50);
  
  raw_T = read_measurement(SHT2x_TRIGGER_T_MEASUREMENT_NO_HOLD);

  if(raw_T == 0xFFFF) 
  {
    return false; 
  }

  delay(10);

  raw_RH = read_measurement(SHT2x_TRIGGER_RH_MEASUREMENT_NO_HOLD);

  if(raw_RH == 0xFFFF) 
  {
    return false; 
  }
  
  temperature = convert_T(raw_T);
  humidity = convert_RH(raw_RH);
  
  return true;
}


uint16_t SHT2x::read_measurement(uint8_t command) 
{
  uint8_t bytes[3] = {0, 0, 0};
  uint8_t i = 0;
  uint16_t value = 0;
  unsigned long start_time = 0; 

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(command);
  if(_i2cPort->endTransmission() != 0) 
  {
    return 0xFFFF; 
  }

  start_time = millis();
  while((millis() - start_time) < 100) 
  { 
    _i2cPort->requestFrom(_deviceAddress, (uint8_t)3);
    
    if(_i2cPort->available() >= 3) 
    {
      break;
    }
    
    delay(5);
  }

  if(_i2cPort->available() < 3) 
  {
    return false;
  }

  for(i = 0; i < 3; i++) 
  {
    bytes[i] = _i2cPort->read();
  }
    
  
  if(crc8(bytes, 2) != bytes[2]) 
  {
    return false;
  }

  value = (bytes[0] << 8) | bytes[1];
  value &= 0xFFFC;
  
  return value;
}


float SHT2x::convert_T(uint16_t value)
{
  return ((175.72f * (value / 65536.0f)) - 46.85f);
}


float SHT2x::convert_RH(uint16_t value) 
{
  return ((125.0f * (value / 65536.0f)) - 6.0f);
}


uint8_t  SHT2x::crc8(const uint8_t* data, uint8_t length) 
{
    uint8_t i = 0;
    uint8_t crc = 0;
    uint8_t bit = 0;
    
    for(i = 0; i < length; i++) 
    {
      crc ^= data[i];
      
      for(bit = 0; bit < 8; bit++) 
      {
        if(crc & 0x80) 
        {
          crc = (crc << 1) ^ 0x131;
        } 
        else 
        {
          crc <<= 1;
        }
      }
    }

    return crc;
}


uint8_t SHT2x::read_reg(void) 
{
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(SHT2x_READ_USER_REGISTER);

  if(_i2cPort->endTransmission() != 0) 
  {
    return 0xFF; 
  }
  
  _i2cPort->requestFrom(_deviceAddress, (uint8_t)1);

  if(_i2cPort->available()) 
  {
    return _i2cPort->read();
  }
  
  return 0xFF; 
}


bool SHT2x::write_reg(uint8_t value) 
{
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(SHT2x_WRITE_USER_REGISTER);
  _i2cPort->write(value);
  return (_i2cPort->endTransmission() == 0);
}


void SHT2x::reset(void) 
{
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(SHT2x_SOFT_RESET);
  _i2cPort->endTransmission();
  delay(15);
}


bool SHT2x::low_battery(void) 
{
  return (read_reg() & SHT2x_USER_REGISTER_END_OF_BATTERY) != 0;
}


void SHT2x::set_resolution(uint8_t resolution) 
{
  uint8_t reg = read_reg();

  if(reg == 0xFF) 
  {
    return;
  } 

  reg &= ~(0x81);
  reg |= (resolution & 0x81);
  
  write_reg(reg);
}


void SHT2x::set_heater(bool enable) 
{
  uint8_t reg = read_reg();

  if(reg == 0xFF) 
  {
    return;
  } 
  
  if(enable) 
  {
    reg |= SHT2x_USER_REGISTER_HEATER_ENABLE;
  } 
  else 
  {
    reg &= ~SHT2x_USER_REGISTER_HEATER_ENABLE;
  }
  
  write_reg(reg);
}


void SHT2x::disable_OTP_reload(bool disable) 
{
  uint8_t reg = read_reg();
  
  if(reg == 0xFF) 
  {
    return;
  } 
  
  if(disable) 
  {
    reg |= SHT2x_USER_REGISTER_DISABLE_OTP_RELOAD;
  } 
  else 
  {
    reg &= ~SHT2x_USER_REGISTER_DISABLE_OTP_RELOAD;
  }
  
  write_reg(reg);
}