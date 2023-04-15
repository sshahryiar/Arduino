#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


byte fill[8] = {0, 14, 14, 14, 14, 14, 0, 0};

unsigned char s = 0;
unsigned char n = 0; 
unsigned int adc_value = 0;
float UV_value = 0;


unsigned int adcAverage()
{
    unsigned char samples = 64;
    unsigned long avg = 0;
    while(samples > 0)
    {
        avg = (avg + analogRead(A0));
        delayMicroseconds(4);
        samples--;
    }
    avg >>= 6;
    return avg;
}


void setup()
{
    lcd.begin(16, 2);
    lcd.createChar(0, fill);
    lcd.setCursor(0, 0);
    lcd.print("UV Index: ");
    lcd.setCursor(0, 1);
    lcd.print("UVI ");
}


void loop()
{   
    adc_value = adcAverage();
    UV_value = ((adc_value * 0.0341) - 1.3894);
    
    if(UV_value > 11)
    {
        UV_value = 11;
    }
    if(UV_value <= 0)
    {
        UV_value = 0;
    }
    
    lcd.setCursor(11, 0);
    lcd.print("   ");
    lcd.setCursor(10, 0);
    lcd.print(UV_value, DEC);
    
    for(s = 4; s <= (((unsigned int)UV_value) + 4); s++)
    {
        lcd.setCursor(s, 1);
        lcd.write(n);
    }
    for(s = (((unsigned int)UV_value) + 5); s < 16; s++)
    {
        lcd.setCursor(s, 1);
        lcd.print(" ");
    }
    
    delay(400);    
}
