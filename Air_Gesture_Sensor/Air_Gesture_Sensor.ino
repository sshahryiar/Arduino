#include <LiquidCrystal.h>


#define U_sensor    1
#define D_sensor    2
#define R_sensor    3
#define L_sensor    4

#define threshold   900


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


unsigned char s = 0;
unsigned char pattern = 0;


void setup()
{
    lcd.begin(16, 2);
    lcd.clear(); 
    lcd.setCursor(1, 0);
    lcd.print("Gesture Sensor");
}

void loop()
{
    pattern = 0;
    for(s = U_sensor; s <= L_sensor; s += 1)
    {
        pattern <<= 1;
        if(analogRead(s) < threshold)
        {
           pattern |= 1;
        }
    }

    if(pattern == 0x0B) 
    {
        lcd.setCursor(2, 1);
        lcd.print(" Up - Down   ");
    }
    if(pattern == 0x07) 
    {
        lcd.setCursor(2, 1);
        lcd.print(" Down - Up   ");
    }
    if(pattern == 0x0E) 
    {
        lcd.setCursor(2, 1);
        lcd.print("Right - Left");
    }
    if(pattern == 0x0D) 
    {
        lcd.setCursor(2, 1);
        lcd.print("Left - Right");
    }
}
