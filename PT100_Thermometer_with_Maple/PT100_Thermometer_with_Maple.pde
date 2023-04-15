#include <LiquidCrystal.h>

#define analogPin                   3
#define ledPin                     33

#define R_ext                  1000.0
#define r850                   390.48
#define rn200                   18.52

LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

const signed int temp[22] = {
                              -200, -150, -100, -50, 0, 50, 100, 150, 200, 250, 300,
                              350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850
                            };

const float res[22] = {
                        18.52, 39.72, 60.26, 80.31, 100.0, 119.4, 138.51, 157.33, 175.86, 194.1, 212.05,
                        229.72, 247.09, 264.18, 280.98, 297.49, 313.71, 329.64, 345.28, 360.64, 375.7, 390.48
                      };   
    
unsigned long adcAvg()
{
    unsigned char samples = 128;
    unsigned long avg = 0;
    
    while(samples > 0)
    {
        avg += analogRead(analogPin);
        delay(1);
        samples--;
    }
    avg >>= 7; 
    return avg;
}
                     

float getTemperature(float r)
{
    float t = 0.0;
    unsigned char cnt = 0;
    
    if(r >= r850)
    {
      t = 850.0;
    }  
    else if(r <= rn200) 
    {
      t = -200.0;
    }
    else
    {
      while(r >= res[cnt])
      {
        cnt++;
      }
      t = ((temp[cnt]) + ((((temp[cnt + 1]) - (temp[cnt]))/((res[cnt + 1]) - (res[cnt]))) * (r - (res[cnt]))));  
    }
    return t;
}


void setup() {
  
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(analogPin, INPUT_ANALOG);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  float T = 0.0;
  float PT = 0.0;
  float Vadc = 0.0;
   
  digitalWrite(ledPin, HIGH);
  Vadc = (float)adcAvg(); 
  PT = (R_ext * ((4095.0 / Vadc) - 1));
  T = getTemperature(PT);
  delay(40);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("r/R :");
  lcd.setCursor(0, 1);
  lcd.print("T/'C:");
  lcd.setCursor(6, 0);
  lcd.print(PT);
  lcd.setCursor(6, 1);
  lcd.print(T);
  digitalWrite(ledPin, LOW);
  delay(400);
}

