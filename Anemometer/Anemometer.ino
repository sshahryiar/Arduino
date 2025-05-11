#include "lcd_keypad.h"


#define PIN           A5
#define scaleFactor   1.0//3.5


lcd_kbd lcd_io;

float windSpeed = 0;
unsigned long lastTime = 0;
unsigned int pulseCount = 0;
unsigned int frequency = 0;
int currentState = HIGH;
int lastState = LOW;

char buffer[20];

void setup(void) 
{
  pinMode(PIN, INPUT);
  Serial.begin(9600);
  lcd_io.begin();
  lcd_io.text(0, 0, "Anemometer");
}

void loop(void) 
{
  currentState = digitalRead(PIN);

  if((lastState == LOW) && (currentState == HIGH)) 
  {
    pulseCount++;
  }

  lastState = currentState;

   if((millis() - lastTime) > 999) 
   {
    frequency = pulseCount;  
    windSpeed = (frequency / scaleFactor);
    lastTime = millis();
    pulseCount = 0;

    sprintf(buffer, "%d km/hr  ", (uint64_t)windSpeed);
    lcd_io.text(0, 1, buffer);
    Serial.println(windSpeed);
   }
}
