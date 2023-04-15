#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


#define TRIGGER    2
#define ECHO       3


unsigned char disp = 0;


float avgDistance()
{
    unsigned char s = 0;
    register unsigned long t = 0;
    register float d = 0.0;
    
    while(s < 4)
    {
        digitalWrite(TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGGER, LOW);    
        t += pulseIn(ECHO, HIGH, 100000);
        s += 1;
    }
    t >>= 2;
    d = (t / 58.0);
    
    return d;   
}


void setup()
{
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("DYP-ME007V SONAR");
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIGGER, LOW);
}


void loop()
{   
    float distance = 0.0;
    unsigned char n = 0;
    
    distance = avgDistance();

    if(analogRead(A0) < 20)
    {
       disp += 1;
       if(disp > 3)
       {
           disp = 0;
       }
       for(n = 0; n < 16; n++)
       {
           lcd.setCursor(n, 1);
           lcd.print(" ");
       }
    }
    switch(disp)
    {
        case 1:
        {
            distance /= 100.0;
            lcd.setCursor(5, 1);
            lcd.print(distance);
            lcd.print("m ");
            break;
        }
        case 2:
        {
            distance *= 0.3937;
            lcd.setCursor(4, 1);
            lcd.print(distance);
            lcd.print("in  ");
            break;
        }
        case 3:
        {
            distance *= 0.032803;
            lcd.setCursor(5, 1);
            lcd.print(distance);
            lcd.print("ft  ");
            break;
        }
        default:
        {
            lcd.setCursor(4, 1);
            lcd.print(distance);
            lcd.print("cm  ");
            break;
        }
    }
    delay(100);    
}
