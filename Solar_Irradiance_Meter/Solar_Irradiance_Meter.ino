#include <LiquidCrystal.h>


#define BPW34pin        A0
#define samples         16


LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


unsigned int sample[samples];

unsigned char s = 0;
unsigned int avg = 0;
unsigned long sum = 0;
float irValue = 0;


void setup()
{
  memset(sample, 0, samples);
  lcd.begin(16, 2);
}



void loop()
{
   sum -= sample[s];
   sample[s] = analogRead(BPW34pin);  
   sum += sample[s];
   s += 1;
   
   if(s >= samples) 
   {
      s = 0;      
      avg = (sum >> 4); 
      irValue = ((16.3366 * avg) - 30.69);
      irValue *= 0.0079;
      
      if(irValue < 0)
      {
        irValue = 0;
      }
      
      lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print("Irradiance Meter");
      lcd.setCursor(0, 1);
      lcd.print("Ir W/m2: ");
   
      lcd.setCursor(9, 1);
      lcd.print(irValue);
      delay(600);
   }
}



