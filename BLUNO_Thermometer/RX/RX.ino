#include <LiquidCrystal.h>


#define statusLED        13
#define scaleFactor      0.096


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


float t = 0.0;
unsigned int temp = 0;


void setup()
{
  pinMode(statusLED, OUTPUT);
  digitalWrite(statusLED, LOW);
  Serial.begin(9600);
  Serial.flush();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BLE Thermometer."); 
  lcd.setCursor(0, 1);
  lcd.print("T/ C:"); 
  lcd.setCursor(2, 1);
  lcd.write(0xDF); 
}


void loop()
{
  if(Serial.available() > 0)
  {
    temp = Serial.parseInt();
    digitalWrite(statusLED, HIGH);
    delay(60);
    digitalWrite(statusLED, LOW);
    delay(60);
  }
  
  t = temp;
  t *= scaleFactor;
  lcd.setCursor(6, 1);
  lcd.print(t);
}
