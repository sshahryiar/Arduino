#include <SoftwareSerial.h>
#include "DHTxx.h"


float t = 0;
float rh = 0;
unsigned char status = 0;


DHTxx dht11(4, DHT11);
SoftwareSerial nextionSerial(7, 8);


void sendToNextion(String cmd_str);
void sendText(void);
void setPointer(void);


void setup(void) 
{
  dht11.begin();
  Serial.begin(9600);
  nextionSerial.begin(9600);
}

void loop(void) 
{
  status = dht11.get_data(&rh, &t);

  rh = constrain(rh, 0, 100);
  t = constrain(t, -20, 100);

  Serial.print("DHT11 Status: ");
  Serial.println(status);
  Serial.print("RH/%: ");
  Serial.println(rh);
  Serial.print("T/'C: ");
  Serial.println(t);
  Serial.println(" ");
  sendText();
  setPointer();
  delay(1000);
}


void sendToNextion(String cmd_str) 
{
  nextionSerial.print(cmd_str);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
}


void sendText(void)
{
  String text1 = "t1.txt=\"" + String((int)rh) + "\"";
  String text2 = "t2.txt=\"" + String((int)t) + "\"";

  sendToNextion(text1);
  sendToNextion(text2);
}


void setPointer(void)
{
  int t_deg = 0;
  int rh_deg = 0;

  rh_deg = ((2.7 * rh) - 45);

  if(rh_deg < 0)
  {
    rh_deg = (360 - rh_deg);
  }

  t_deg = (2.25 * t);

  if(t_deg < 0) 
  {
    t_deg = (360 - t_deg);
  }

  sendToNextion("vis z0,0");
  sendToNextion("z0.val=" + String(rh_deg));
  sendToNextion("vis z0,1");

  sendToNextion("vis z1,0");
  sendToNextion("z1.val=" + String(t_deg));
  sendToNextion("vis z1,1");
}