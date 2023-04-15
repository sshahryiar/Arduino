#include <LiquidCrystal.h>


#define fixed_R      10000.0
#define ADC_Max       4095.0
#define B             3470.0
#define _25C_K         298.0

#define t_sense_pin        3

#define power_pin_1        4
#define power_pin_2        5

#define LED_pin           33


LiquidCrystal lcd(6, 7, 8, 9, 10, 11);


float tmp1 = 0.0;
float tmp2 = 0.0;
float R_inf = 0.0;

byte symbol1[8]= 
{        
    0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00
};
byte symbol2[8]= 
{        
    0x00, 0x00, 0x0E, 0x11, 0x11, 0x0A, 0x1B, 0x00
};


unsigned int adcAvg()
{
    unsigned int samples = 512;
    unsigned long avg = 0;
    
    while(samples > 0)
    {
        avg += analogRead(t_sense_pin);
        delayMicroseconds(1);
        samples--;
    }
    avg >>= 9; 
    return avg;
}


void power_mode_1()
{
  digitalWrite(power_pin_1, HIGH);  
  digitalWrite(power_pin_2, LOW);
  digitalWrite(LED_pin, HIGH);
  delay(90);
}


void power_mode_2()
{
  digitalWrite(power_pin_2, HIGH);  
  digitalWrite(power_pin_1, LOW);
  digitalWrite(LED_pin, LOW);
  delay(90);
}


float RT_calculation_1()
{
  float tmp = 0.0;  
  
  power_mode_1();
  
  tmp = ((float)adcAvg());  
  tmp = ((ADC_Max / tmp) - 1);
  tmp *= fixed_R;
  
  return tmp;
}


float RT_calculation_2()
{
  float tmp = 0.0; 
  
  power_mode_2();
    
  tmp = ((float)adcAvg());  
  tmp = ((ADC_Max / tmp) - 1);
  tmp = (fixed_R / tmp);
  
  return tmp;
}


float RT_avg(float RT1, float RT2)
{
    return ((RT1 + RT2) / 2.0);
}


void lcd_tasks()
{
  lcd.setCursor(7, 0);
  lcd.print("C");
  lcd.setCursor(6, 0);
  lcd.write(byte(0));
  lcd.setCursor(6, 1);
  lcd.print("k");
  lcd.setCursor(7, 1);
  lcd.write(byte(1));
}


void setup()
{
  lcd.begin(16, 2);
  lcd.clear(); 
  lcd.createChar(0, symbol1);
  lcd.createChar(1, symbol2);
  lcd.noBlink(); 
  pinMode(t_sense_pin, INPUT_ANALOG);
  pinMode(LED_pin, OUTPUT);
  pinMode(power_pin_1, OUTPUT);  
  pinMode(power_pin_2, OUTPUT);   
  digitalWrite(LED_pin, LOW);
  digitalWrite(power_pin_1, LOW);  
  digitalWrite(power_pin_2, LOW); 
  R_inf = (-1.0 * (B / _25C_K));
  R_inf = exp(R_inf); 
  R_inf *= fixed_R;
  lcd_tasks();
}


void loop()
{
  tmp1 = RT_calculation_1();
  tmp2 = RT_calculation_2();
  
  tmp1 = RT_avg(tmp1, tmp2);
  tmp2 = (B / (log (tmp1 / R_inf)));
  tmp2 = (tmp2 - 273.0);
  tmp1 /= 1000.0;
  
  lcd.setCursor(0, 0);
  lcd.print("     ");
  lcd.setCursor(0, 0);
  lcd.print(tmp2, 2);
    
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print(tmp1, 2);
}
