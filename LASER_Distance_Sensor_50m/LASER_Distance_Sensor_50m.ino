#include <SoftwareSerial.h>
#include "lcd_keypad.h"
#include "M703.h"


lcd_kbd io;
SoftwareSerial lidarSerial(11, 12); 
M703 lidar(lidarSerial);


float d = 0;
float t = 0;
float bv = 0;
uint16_t s = 0;
bool state = 0;
bool boot = true;
uint8_t cmd_cnt = 0;
char buffer[40];
char float_to_str[20];


void setup(void) 
{  
  io.begin();
  io.clear();

  Serial.begin(9600);
  lidarSerial.begin(19200); 
  lidar.begin();

  io.text(1, 0, "M703 50m LIDAR");
  io.text(0, 1, "UP Key > Measure");
}

void loop(void) 
{
  if((io.get_key() == up_key) || boot)
  {
    while(cmd_cnt < 3)
    {
      if(cmd_cnt == 0)
      {
        lidar.send_command("C");
      }

      else if(cmd_cnt == 1)
      {
        lidar.send_command("S");
      }

      else 
      {
        lidar.send_command("D");
      }

      lidar.read();
      
      if(cmd_cnt == 0)
      {
        state = lidar.get_status();
      }

      else if(cmd_cnt == 1)
      {
        t = lidar.get_temperature();
        bv = lidar.get_voltage();
      }

      else 
      {
        d = lidar.get_range();
        s = lidar.get_signal_info();
        state = lidar.get_status();
      }

      cmd_cnt++;
      boot = false;
      delay(1000);
    }

    cmd_cnt = 0;
  }

  if(state)
  {
    io.clear();

    dtostrf(d, 2, 3, float_to_str);
    sprintf(buffer, "%s ", float_to_str);
    strcat(buffer, "m ");
    io.text(0, 0, buffer);

    io.text(10, 0, "S:");
    sprintf(buffer, "%04d ", s);
    io.text(12, 0, buffer);

    dtostrf(t, 2, 2, float_to_str);
    sprintf(buffer, "%s", float_to_str);
    strcat(buffer, "'C ");
    io.text(0, 1, buffer);

    dtostrf(bv, 1, 2, float_to_str);
    sprintf(buffer, "%s ", float_to_str);
    strcat(buffer, "V");
    io.text(10, 1, buffer);

    state = false;
  }

  delay(1000);
}


