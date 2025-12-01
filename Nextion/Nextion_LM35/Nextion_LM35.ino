#include <SoftwareSerial.h>
#include "LM35.h"


float t = 0;

LM35 temp_sensor(A2);
SoftwareSerial nextionSerial(7, 8);


void sendToNextion(String cmd_str);
void sendNumber(uint8_t objectID);
void setPointer(uint8_t objectID);
void setBar(uint8_t objectID);
void drawGraph(uint8_t objectID, uint8_t channelID);


void setup(void) 
{
  Serial.begin(9600);
  nextionSerial.begin(9600);
}

void loop(void) 
{
  t = temp_sensor.get_T();
  t = constrain(t, 0, 100);
  Serial.print("T/'C: ");
  Serial.println(t);
  Serial.println(" ");
  sendNumber(0);
  setPointer(0);
  setBar(0);
  drawGraph(5, 0);
  delay(600);
}


void sendToNextion(String cmd_str) 
{
  nextionSerial.print(cmd_str);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
}


void sendNumber(uint8_t objectID)
{
  String text = "n" + String(objectID) + ".val=" + String((int)t);
  sendToNextion(text);
}


void setPointer(uint8_t objectID)
{
  int t_deg = 0;

  if(t >= 16.67)
  {
    t_deg = map(t, 50, 100, 90, 225);
  }
  else
  {
    t_deg = map(t, 16.67, 0, 0, -50);
  }

  if(t_deg < 0)
  {
    t_deg += 360;
  }

  sendToNextion("vis z" + String(objectID) + ",0");
  sendToNextion("z" + String(objectID) + ".val=" + String(t_deg));
  sendToNextion("vis z" + String(objectID) + ",1");
}


void setBar(uint8_t objectID)
{
  sendToNextion("j" + String(objectID) + ".val=" + String((int)t));
}

void drawGraph(uint8_t objectID, uint8_t channelID)
{
  int t_graph = 0;

  t_graph = map(t, 0, 100, 0, 255);
  sendToNextion("add " + String(objectID) + ","  + String(channelID) + "," + String(t_graph));
}