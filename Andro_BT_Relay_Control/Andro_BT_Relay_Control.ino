/*
Developer: Shawon Shahryiar
https://www.facebook.com/groups/ArduinoBangla/
https://www.facebook.com/groups/microarena/
https://www.facebook.com/MicroArena
*/


#define Relay1        4
#define Relay2        5
#define Relay3        6
#define Relay4        7
#define statusLED    13


void setup()
{
    pinMode(Relay1, OUTPUT);
    digitalWrite(Relay1, LOW);
    pinMode(Relay2, OUTPUT);
    digitalWrite(Relay2, LOW);
    pinMode(Relay3, OUTPUT);
    digitalWrite(Relay3, LOW);
    pinMode(Relay4, OUTPUT);
    digitalWrite(Relay4, LOW);
    pinMode(statusLED, OUTPUT);
    digitalWrite(statusLED, LOW);
    Serial.begin(9600);
    Serial.flush();
}

void loop()
{
    unsigned char cmd = 0;
    
    digitalWrite(statusLED, HIGH);
    
    if(Serial.available() > 0)
    {
          cmd = Serial.read();
    }
    
    switch(cmd)
    {
          case 'A':
          {
              digitalWrite(Relay1, LOW);  
              break;
          }
          case 'B':
          {
              digitalWrite(Relay2, LOW);  
              break;
          }
          case 'C':
          {
              digitalWrite(Relay3, LOW);  
              break;
          }
          case 'D':
          {
              digitalWrite(Relay4, LOW);  
              break;
          }
          case 'a':
          {
              digitalWrite(Relay1, HIGH);  
              break;
          }
          case 'b':
          {
              digitalWrite(Relay2, HIGH);  
              break;
          }
          case 'c':
          {
              digitalWrite(Relay3, HIGH);  
              break;
          }
          case 'd':
          {
              digitalWrite(Relay4, HIGH);  
              break;
          }
    }
    
    delay(10);
    digitalWrite(statusLED, LOW);
    delay(90);
    
}
