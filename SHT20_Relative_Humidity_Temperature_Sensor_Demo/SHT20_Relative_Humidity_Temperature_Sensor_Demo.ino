#include <Wire.h>
#include "SHT2x.h"
#include "lcd_keypad.h"


SHT2x sht;
lcd_kbd io;


char buffer[40];
char floatStr[20];
float temperature = 0;
float humidity = 0;


void setup() 
{
  Serial.begin(9600);
  Serial.println("SHT2x Temperature and Humidity Sensor Test");
  io.begin();

  if (!sht.begin()) 
  {
    Serial.println("Could not find a valid SHT2x sensor, check wiring!");
    while (1) 
    {
      delay(10);
    }
  }

  io.load_custom_symbol();
  io.text(0, 0, "T/ C:");
  io.text(0, 1, "RH/%:");
  io.print_custom_symbol(2, 0, 0);
}

void loop() 
{
  sht.read(temperature, humidity);

  if(isnan(temperature) || isnan(humidity)) 
  {
    Serial.println("Failed to read from SHT2x sensor!");
    delay(2000);
    return;
  } 

  else
  {
    dtostrf(temperature, 2, 2, floatStr);
    sprintf(buffer, "%s ", floatStr);
    Serial.print("T/'C: ");
    Serial.print(buffer);
    Serial.println(" ");
    io.text(11, 0, buffer);
    
    dtostrf(humidity, 2, 2, floatStr);
    sprintf(buffer, "%s ", floatStr);
    Serial.print("RH/%: ");
    Serial.print(buffer);
    Serial.println(" ");
    io.text(11, 1, buffer);
  }
  
  delay(1000); 
}
