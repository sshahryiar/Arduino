#include <UTFT.h>

extern uint8_t SmallFont[];

UTFT GLCD(ITDB18SP,7,6,5,3,4);


float t_present = 0;
float t_previous = 0;

unsigned char c = 0;
unsigned char s = 0;

unsigned char R_Value[11] = {0, 2, 5, 10, 19, 40, 160, 220, 240, 250, 255};
unsigned char G_Value[11] = {0, 160, 240, 255, 255, 250, 240, 220, 160, 110, 10};
unsigned char B_Value[11] = {255, 250, 240, 200, 160, 130, 45, 25, 5, 2, 0};


void drawBackground()
{
  GLCD.setBackColor(0, 0, 0);
  GLCD.setColor(255, 255, 255);
  GLCD.drawCircle(59, 124, 16);
  GLCD.setColor(0, 0, 255);
  GLCD.fillCircle(59, 124, 15);
  GLCD.setColor(255, 255, 255);
  GLCD.drawLine(56, 7, 56, 108);
  GLCD.drawLine(62, 7, 62, 108);
  GLCD.drawLine(57, 6, 61, 6);
  GLCD.drawLine(60, 5, 58, 5);
   
  for(s = 0; s < 11; s++)
  {
      GLCD.setColor(B_Value[s], G_Value[s], R_Value[s]);
      GLCD.drawLine(78, (108 - (s * 10)), 73, (108 - (s * 10)));
      GLCD.drawLine(73, (103 - (s * 10)), 76, (103 - (s * 10)));
  }
  
  GLCD.setColor(0, 0, 255);
  GLCD.drawLine(57, 109, 61, 109); 
  GLCD.drawLine(57, 108, 61, 108);
  
  GLCD.setColor(0, 0, 0);
  GLCD.drawLine(73, 3, 76, 3);
  
  GLCD.setColor(B_Value[0], G_Value[0], R_Value[0]);
  GLCD.print("0'C", 84, 103);
  GLCD.setColor(B_Value[2], G_Value[2], R_Value[2]);
  GLCD.print("20'C", 84, 83);
  GLCD.setColor(B_Value[4], G_Value[4], R_Value[4]);
  GLCD.print("40'C", 84, 63);
  GLCD.setColor(B_Value[6], G_Value[6], R_Value[6]);
  GLCD.print("60'C", 84, 43);
  GLCD.setColor(B_Value[8], G_Value[8], R_Value[8]);
  GLCD.print("80'C", 84, 23);
  GLCD.setColor(B_Value[10], G_Value[10], R_Value[10]);
  GLCD.print("100'C", 84, 3);
}


unsigned int adc_avg()
{
  unsigned int samples = 512;
  unsigned long avg = 0;
  
  while(samples > 0)
  {
    avg += analogRead(A0);
    delayMicroseconds(1);
    samples--;
  }
  
  avg >>= 9;
  
  return ((unsigned int)avg);
}


void setup()
{
  GLCD.InitLCD(PORTRAIT);
  GLCD.setFont(SmallFont);
  GLCD.clrScr();
  GLCD.setContrast(64);
  drawBackground();
}


void loop()
{
    t_present = adc_avg();
    t_present *= 0.5;
    
    if(t_present >= 100.0)
    {
      t_present = 100.0;
    }
    if(t_present <= 0.0)
    {
      t_present = 0.0;
    }   
    
    if(t_present != t_previous)
    {
      GLCD.setColor(0, 0, 0);
      GLCD.fillRect(60, (109 - ((unsigned char)t_previous)), 58, 9);
      c = t_present / 10.0;
      GLCD.setColor(0, 0, 255);
      GLCD.fillRect(58, 109, 60, (109 - ((unsigned char)t_present)));
      GLCD.setColor(B_Value[c], G_Value[c], R_Value[c]);
      GLCD.print("T/'C:        ", 25, 145);
      GLCD.printNumF(t_present, 2, 65, 145);
    }
    
    t_previous = t_present;
    delay(400);
}


