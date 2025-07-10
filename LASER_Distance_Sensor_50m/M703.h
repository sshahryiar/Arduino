#pragma once


#include <Arduino.h>


class M703 
{
  public:
    M703(Stream& serial);

    void begin(void);
    void read(void);
    float get_range(void);
    uint16_t get_signal_info(void);
    float get_temperature(void);
    float get_voltage(void);
    bool get_status(void);      
    void send_command(const char *cmd); 

  private:
    Stream& lidarSerial;

    char lidar_buffer[32];
    uint8_t buffer_index = 0;
    float distance = 0;
    uint16_t signal = 0;
    float temperature = 0;
    float voltage = 0;
    bool status;

    void process_raw_data(const char* data);
};

