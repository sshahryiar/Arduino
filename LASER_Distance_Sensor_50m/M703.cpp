#include "M703.h"
#include <string.h>
#include <stdlib.h>


M703::M703(Stream& serial) : lidarSerial(serial) 
{
}


void M703::begin(void) 
{
  buffer_index = 0;
  memset(lidar_buffer, 0, sizeof(lidar_buffer));
}


void M703::read(void) 
{
  if(lidarSerial.available()) 
  { 
    buffer_index = lidarSerial.readBytesUntil('\n', lidar_buffer, (sizeof(lidar_buffer) - 1));
    lidar_buffer[buffer_index] = '\0'; 
    process_raw_data(lidar_buffer);
    buffer_index = 0; 
  }
  else 
  {
    buffer_index = 0; 
  }
}


float M703::get_range(void) 
{
  return distance;
}


uint16_t M703::get_signal_info(void) 
{
  return signal;
}


float M703::get_temperature(void) 
{
  return temperature;
}


float M703::get_voltage(void) 
{
  return voltage;
}


bool M703::get_status(void) 
{
  return status;
}


void M703::send_command(const char *cmd)
{
  lidarSerial.print(cmd);
}


void M703::process_raw_data(const char* data) 
{
  const char *start = nullptr;
  const char *chr = nullptr;
  const char *schr = nullptr;
  const char *comma = nullptr;

  char tmpStr_1[10] = {0};
  char tmpStr_2[10] = {0};

  int tmp1 = 0;
  float tmp2 = 0;
  float tmp3 = 0;

  if(strncmp(data, "D:", 2) == 0) 
  {
    start = strchr(data, ':');
    chr = strchr(data, 'm');
    comma = strchr(data, ',');

    if((start && chr && comma) && (chr > start) && (comma > chr)) 
    {
      strncpy(tmpStr_1, (comma + 1), 5); 
      strncpy(tmpStr_2, (start + 2), (chr - (start + 2)));  
                    
      tmp1 = atoi(tmpStr_1);
      tmp2 = atof(tmpStr_2);

      Serial.print("Distance (m): ");
      Serial.println(tmp2, 3);
      Serial.print("Signal Info: ");
      Serial.println(tmp1);
      strcat(tmpStr_2, " m ");

      signal = tmp1;
      distance = tmp2;
      status = true;
    } 
    else 
    {
      Serial.println("Parsing failed (structure mismatch).");
      status = false;
    }
  }
 
  else if(strncmp(data, "S:", 2) == 0) 
  {
    start = strchr(data, ':');
    chr = strchr(data, '`'); 
    comma = strchr(data, ',');
    schr = strchr(data, 'V');    

    if((start && chr && comma && schr) && 
      (chr > start) && (comma > chr) && (schr > comma)) 
    {
        chr = '\0'; 
        tmp2 = atof(start + 1);
        chr = '°'; 

        schr = '\0'; 
        tmp3 = atof(comma + 1);
        schr = 'V'; 

        Serial.print("Temp (°C): ");
        Serial.println(tmp2, 2);
        Serial.print("BV (V): ");
        Serial.println(tmp3, 2);

        temperature = tmp2;
        voltage = tmp3;
        status = true;
    }  

    else 
    {
      Serial.println("Parsing failed (structure mismatch).");
      status = false;
    }
  }

  else if((strncmp(data, "O,OK!", 5) == 0) || strncmp(data, "C,OK!", 5) == 0) 
  {
    Serial.print("Status: ");
    Serial.println(data + 2);    
    status = true;
  }

  else
  {
    Serial.println("Error! Unknown Code!");
    status = false;
  }
}
