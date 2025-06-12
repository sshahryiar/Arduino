#include "TM1637.h"
#include "DHTxx.h"
#include "ZP07_MP503.h"


ZP07 AQ(SCL, SDA);
DHTxx DHT(12);
TM1637 display(11, 10);


float t = 0;
float rh = 0;
uint8_t aq = 0;


void setup(void)
{
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  Serial.begin(115200);
  Serial.flush();
  display.begin();
  DHT.begin();
  AQ.begin();
}


void loop(void)
{
  DHT.get_data(&rh, &t);
  aq = AQ.read();

  Serial.print("Air Quality: ");
  switch(aq)
  {
    case ZP07_heavy_pollution:
    {
      Serial.print("Hazardous!\t");
      aq = ZP07_air_quality_25;
      digitalWrite(7, LOW);
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);      
      break;
    }

    case ZP07_high_pollution:
    {
      Serial.print("Worst.\t");
      aq = ZP07_air_quality_50;
      digitalWrite(7, LOW);
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      break;
    }

    case ZP07_light_pollution:
    {
      Serial.print("Poor.\t");
      aq = ZP07_air_quality_75;
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
      break;
    }

    default:
    {
      Serial.print("Good.\t");
      aq = ZP07_air_quality_100;
      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
      break;
    }
  }

  Serial.print("RH/%: ");
  Serial.print(rh, 1);
  Serial.print("\t T/'C: ");
  Serial.println(t, 1);

  display.text(0, "TEMP");
  delay(2000);
  display.print_f(t);
  delay(2000);
  display.text(0, "RHUM");
  delay(2000);
  display.print_f(rh);
  delay(2000);
  display.text(0, "AIRQ");
  delay(2000);
  display.print_i(aq);
  delay(2000);
}