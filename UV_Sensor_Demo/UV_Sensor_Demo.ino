#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


byte fill[8] = {0, 14, 14, 14, 14, 14, 0, 0};

unsigned char s = 0;
unsigned char n = 0; 
unsigned int UV_value = 0;


unsigned int adcAverage()
{
    unsigned char samples = 0;
    unsigned long avg = 0;
    for(samples = 0; samples < 50; samples += 1)
    {
        avg = (avg + analogRead(A1));
        delayMicroseconds(4);
    }
    avg = (avg / 50.0);
    return avg;
}


void setup()
{
    lcd.begin(16, 2);
    lcd.createChar(0, fill);
    lcd.setCursor(2, 0);
    lcd.print("UV Index: ");
    lcd.setCursor(0, 1);
    lcd.print("UVI ");
}


void loop()
{   
    UV_value = adcAverage();
    
    if((UV_value <= 10))
    {
        UV_value = 0;
    }
    else if((UV_value > 10) && (UV_value <= 46))
    {
        UV_value = 1;
    }
    else if((UV_value > 46) && (UV_value <= 65))
    {
        UV_value = 2;
    }
    else if((UV_value > 65) && (UV_value <= 83))
    {
        UV_value = 3;
    }
    else if((UV_value > 83) && (UV_value <= 103))
    {
        UV_value = 4;
    }
    else if((UV_value > 103) && (UV_value <= 124))
    {
        UV_value = 5;
    }
    else if((UV_value > 124) && (UV_value <= 142))
    {
        UV_value = 6;
    }
    else if((UV_value > 142) && (UV_value <= 163))
    {
        UV_value = 7;
    }
    else if((UV_value > 163) && (UV_value <= 180))
    {
        UV_value = 8;
    }
    else if((UV_value > 180) && (UV_value <= 200))
    {
        UV_value = 9;
    }
    else if((UV_value > 200) && (UV_value <= 221))
    {
        UV_value = 9;
    }
    else if((UV_value > 221) && (UV_value <= 239))
    {
        UV_value = 10;
    }
    else
    {
        UV_value = 11;
    }
    
    lcd.setCursor(12, 0);
    lcd.print("  ");
    lcd.setCursor(12, 0);
    lcd.print(UV_value, DEC);
    
    for(s = 4; s <= (UV_value + 4); s++)
    {
        lcd.setCursor(s, 1);
        lcd.write(n);
    }
    for(s = (UV_value + 5); s < 16; s++)
    {
        lcd.setCursor(s, 1);
        lcd.print(" ");
    }
    
    delay(100);    
}
