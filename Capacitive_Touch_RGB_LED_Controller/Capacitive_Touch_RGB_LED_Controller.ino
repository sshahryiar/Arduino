#define redLED                 9
#define blueLED               10
#define greenLED              11

#define redTouchPad            2
#define blueTouchPad           3
#define resetTouchPad          4
#define greenTouchPad          5


signed char redChange = 1;
signed char blueChange = 1;
signed char greenChange = 1;

signed int redDuty = 0x00;
signed int blueDuty = 0x00;
signed int greenDuty = 0x00;


void setup()
{
      pinMode(redTouchPad, INPUT);
      pinMode(blueTouchPad, INPUT);
      pinMode(resetTouchPad, INPUT);
      pinMode(greenTouchPad, INPUT);
      pinMode(redLED, OUTPUT);
      pinMode(blueLED, OUTPUT);
      pinMode(greenLED, OUTPUT);
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, LOW);
}


void loop()
{
    if(digitalRead(redTouchPad) == true)
    {
          redDuty += redChange;
    }
     if(digitalRead(blueTouchPad) == true)
    {
         blueDuty += blueChange;
    }
     if(digitalRead(greenTouchPad) == true)
    {
          greenDuty += greenChange;
    }
    if(digitalRead(resetTouchPad) == true)
    {
         redDuty = 0x00;
         blueDuty = 0x00;
         greenDuty = 0x00;     
    }
    
    if(redDuty >= 255)
    {
          redChange = -1;
    }
    if(redDuty <= 0)
    {
          redChange = 1;
    }
    
    if(blueDuty >= 255)
    {
          blueChange = -1;
    }
    if(blueDuty <= 0)
    {
          blueChange = 1;
    }
    
    if(greenDuty >= 255)
    {
          greenChange = -1;
    }
    if(greenDuty <= 0)
    {
          greenChange = 1;
    }
    
    analogWrite(redLED, redDuty);
    analogWrite(blueLED, blueDuty);
    analogWrite(greenLED, greenDuty);
    
    delay(10);
}
