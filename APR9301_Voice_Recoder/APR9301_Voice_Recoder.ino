#include <LiquidCrystal.h>


#define CE              13
#define PLAY            12
#define RECORD          11

#define maxTime         20

#define PL               1
#define REC              2
#define NONE             3
#define STOP             4


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned char s = 0;
unsigned char t = 0;
unsigned char time = 0;


unsigned char readSwitches()
{    
    unsigned int sValue = 0;
  
    sValue = analogRead(A0);
    
    if((sValue < 40) && (sValue >= 0))
    {
        return PL;
    }
    else if((sValue >= 330) && (sValue <= 460))
    {
        return REC;
    }
    else if((sValue >= 540) && (sValue <= 660))
    {
        return STOP;
    }
    else 
    {
        return NONE;
    }
}


void setup()
{
    for(s = 11; s <= 13; s += 1)
    {
         pinMode(s, OUTPUT);
         digitalWrite(s, HIGH);
    }
    
    lcd.begin(16, 2);
    lcd.clear(); 
    
    lcd.setCursor(3, 0);
    lcd.print("APR9301-V2");  
    lcd.setCursor(3, 1);
    lcd.print("Arducorder");  
    
    delay(2000);
    lcd.clear();
}


void loop()
{
    s = readSwitches();
        
    if((s == STOP) || (s == NONE))
    {
        digitalWrite(CE, HIGH);
        digitalWrite(PLAY, HIGH);
        digitalWrite(RECORD, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Stopped....");  
    }
    
    if(s == REC)
    {
        t = 0;
        lcd.clear();
        digitalWrite(CE, LOW);
        digitalWrite(PLAY, HIGH);
        digitalWrite(RECORD, LOW);
        lcd.setCursor(0, 0);
        lcd.print("Recording....");
        lcd.setCursor(0, 1);
        lcd.print("Time/s:");
        
        while((readSwitches() != STOP) && (t <= maxTime))
        {
            lcd.setCursor(8, 1);
            lcd.print(t);
            t += 1;
            delay(990);
        }
        lcd.clear();
    }
   
    
    if((s == PL) && (t > 0))
    {
        lcd.clear();
        digitalWrite(CE, LOW);
        digitalWrite(PLAY, LOW);
        digitalWrite(RECORD, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Playing....");
        lcd.setCursor(0, 1);
        lcd.print("Time/s:");
        
        while((readSwitches() != STOP) && (time < t))
        {
            lcd.setCursor(8, 1);
            lcd.print(time);
            time += 1;
            delay(990);
        }
        lcd.clear();
        time = 0;
    }
}
