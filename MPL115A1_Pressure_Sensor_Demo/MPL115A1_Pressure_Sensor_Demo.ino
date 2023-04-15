#include <SPI.h>
#include <LiquidCrystal.h>


#define PRESH              0x80
#define PRESL              0x82
#define TEMPH              0x84
#define TEMPL              0x86

#define A0_H               0x88                                                
#define A0_L               0x8A
#define B1_H               0x8C
#define B1_L               0x8E
#define B2_H               0x90             
#define B2_L               0x92
#define C12_H              0x94
#define C12_L              0x96                   

#define conv_cmd           0x24                                                                    

#define SDN               48 
#define CSN               53


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


float p = 0.0; 
float t = 0.0;


struct
{
  float A0_;           
  float B1_;   
  float B2_;    
  float C12_;
}coefficients; 
                
                 
void MPL115A1_init();
unsigned char MPL115A1_read(unsigned char address);
void MPL115A1_write(unsigned char address, unsigned char value);
void get_coefficients(); 
void get_data(float *pres, float *temp);  


void setup()
{
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("P/kPA:");
    lcd.setCursor(0, 1);
    lcd.print("T/  C:");
    lcd.setCursor(3, 1);
    lcd.write(0xDF);
    SPI.begin();
    MPL115A1_init();
}


void loop()
{
  get_data(&p, &t);
  lcd.setCursor(7, 0);
  lcd.print(p);
  lcd.setCursor(7, 1);
  lcd.print(t);
  delay(600);
}


void MPL115A1_init()
{
  pinMode(CSN, OUTPUT);   
  pinMode(SDN, OUTPUT);  
  digitalWrite(CSN, HIGH); 
  digitalWrite(SDN, HIGH);
  delay(10); 
  get_coefficients();
}       
                                        

unsigned char MPL115A1_read(unsigned char address)
{
  unsigned char value = 0x00;
  digitalWrite(CSN, LOW);       
  delay(10);
  SPI.transfer(address); 
  value = SPI.transfer(0x00); 
  digitalWrite(CSN, HIGH);                               
  return value;        
}                                           
                                               
    
void MPL115A1_write(unsigned char address, unsigned char value)
{                                                     
  digitalWrite(CSN, LOW);       
  delay(3);
  SPI.transfer((address & 0x7F)); 
  SPI.transfer(value);                           
  digitalWrite(CSN, HIGH);  
} 


void get_coefficients()
{     
  unsigned int hb = 0;               
  unsigned int lb = 0; 
    
  get_bytes(&hb, &lb, A0_H); 
  coefficients.A0_ = ((hb << 5) + (lb >> 3) + ((lb & 7) / 8.0));    
            
  get_bytes(&hb, &lb, B1_H);              
  coefficients.B1_ = (((((hb & 0x1F) * 0x0100) + lb) / 8192.0) - 3.0);    
    
  get_bytes(&hb, &lb, B2_H);    
  coefficients.B2_ = (((((hb - 0x80) << 8) + lb) / 16384.0) - 2.0);   
                                        
  get_bytes(&hb, &lb, C12_H);                 
  coefficients.C12_ = (((hb * 0x100) + lb) / 16777216.0);   
}  


void get_bytes(unsigned int *upper, unsigned int *lower, unsigned char address) 
{ 
  *upper = MPL115A1_read(address); 
  *lower = MPL115A1_read((address + 2)); 
}
                                                                                                          
                 
void get_data(float *pres, float *temp)   
{   
   unsigned int hb = 0;
   unsigned int lb = 0;

   unsigned long Padc = 0; 
   unsigned long Tadc = 0;
   
   MPL115A1_write(conv_cmd, 0x00); 

   get_bytes(&hb, &lb, PRESH);
   Padc = (((hb << 8) + lb) >> 6);
   
   get_bytes(&hb, &lb, TEMPH); 
   Tadc = (((hb << 8) + lb) >> 6);
   
   *pres = ( coefficients.A0_ + (( coefficients.B1_ + ( coefficients.C12_ * Tadc)) * Padc) + ( coefficients.B2_ * Tadc));      
   *pres = (((*pres * 65.0) / 1023.0) + 50.0);      
   
   *temp = (30.0 + ((Tadc - 472) / (-5.35)));
} 



