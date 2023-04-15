#include <ColorLCDShield.h>

LCDShield glcd;


#define Right     digitalRead(A0)
#define Down      digitalRead(A1)
#define Left      digitalRead(A2)
#define Center    digitalRead(A3)
#define Up        digitalRead(A4)

#define backLight 10

#define dly       190

#define _4_bands  0
#define _5_bands  1


unsigned int tmp = 0;

unsigned char s = 0;
unsigned char item = 0;
unsigned char band1 = 0;
unsigned char band2 = 0;
unsigned char band3 = 0;
unsigned char band4 = 0;
unsigned char band5 = 0;
unsigned char temp = 0;

boolean bands = _4_bands;

float multiplying_factor = 1;
float R = 0.0;

unsigned char rValue[2];

char textChar[6];  

const unsigned int bandColour[12] = {BLACK, BROWN, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, GRAY, WHITE, GOLD, SLATE};


void drawBackground()
{
  glcd.setRect(33, 29, 48, 99, 1, SKYBLUE);
  glcd.setRect(39, 9, 42, 28, 1, SLATE);
  glcd.setRect(39, 100, 42, 119, 1, SLATE);
  glcd.setLine(33, 40, 48, 40, BLACK);
  glcd.setLine(33, 43, 48, 43, BLACK);
  glcd.setLine(33, 49, 48, 49, BLACK);
  glcd.setLine(33, 52, 48, 52, BLACK);
  glcd.setLine(33, 58, 48, 58, BLACK);
  glcd.setLine(33, 61, 48, 61, BLACK);
  glcd.setLine(33, 83, 48, 83, BLACK);
  glcd.setLine(33, 86, 48, 86, BLACK);
  
  if(bands == _5_bands)
  {
      glcd.setLine(33, 71, 48, 71, BLACK);
      glcd.setLine(33, 74, 48, 74, BLACK);
  }
  else
  {
      glcd.setLine(33, 71, 47, 71, SKYBLUE);
      glcd.setLine(33, 74, 47, 74, SKYBLUE);
  }
}


unsigned char changeParameter(signed char value, signed char minValue, signed char maxValue)
{
    if(Up == 0)
    {
       delay(dly);
       value++;
    }
    if(Down == 0)
    {
       delay(dly);
       value--;
    }
    
    if(value > maxValue)
    {
      value = minValue;
    }
    if(value < minValue)
    {
      value = maxValue;
    }
    
    return value;
}


void setup()
{
    for(s = A0; s <= A4; s++)
    {
        pinMode(s, INPUT_PULLUP);
    }
    item = 0;
    
    pinMode(backLight, OUTPUT);
    analogWrite(backLight, 254); 
    
    glcd.init(PHILLIPS);
    glcd.contrast(45);
    glcd.clear(BLACK);
    
    drawBackground();
    glcd.setRect(33, 41, 48, 42, 1, (bandColour[band1]));
    glcd.setRect(33, 50, 48, 51, 1, (bandColour[band2]));
    glcd.setRect(33, 59, 48, 60, 1, (bandColour[band3]));
    glcd.setRect(33, 84, 48, 85, 1, (bandColour[band5]));    
}


void loop()
{  
    memset(textChar, 0x00, sizeof(textChar));   
    
    if((millis() % 60) == 0)
    {
      glcd.setStr("    ", 70, 22, CYAN, BLACK); 
    }
  
    if(Right == 0)
    {
      while(Right == 0); 
      item++;
    } 
    if(Left == 0)
    {
      while(Left == 0); 
      if((bands == _4_bands) && (item == 5))
      {
        item -= 2;
      }
      else
      {
        item--;
      }
    }
    
    if((item > 5) || (item < 1))
    {
      glcd.setRect(51, 41, 52, 85, 1, BLACK);
      item = 0;
    }
  
    if(Center == 0)
    {
      while(Center == 0); 
      item = 0;
      bands ^= _5_bands;
      drawBackground();
    }
    
    switch(item)
    {
      case 1:
      {
          band1 = changeParameter(band1, 0, 9);
          glcd.setRect(33, 41, 48, 42, 1, (bandColour[band1]));
          glcd.setRect(51, 41, 52, 42, 1, NAVY);
          glcd.setRect(51, 50, 52, 51, 1, BLACK);
          break; 
      }
      case 2:
      {
          band2 = changeParameter(band2, 0, 9);
          glcd.setRect(33, 50, 48, 51, 1, (bandColour[band2]));
          glcd.setRect(51, 50, 52, 51, 1, NAVY);
          glcd.setRect(51, 41, 52, 42, 1, BLACK);
          glcd.setRect(51, 59, 52, 60, 1, BLACK);
          break; 
      }
      case 3:
      {
          band3 = changeParameter(band3, 0, 11);
          if(bands == _4_bands)
          {
             if((band3 != 7) || (band3 != 8) || (band3 != 9))
             {
               glcd.setRect(33, 59, 48, 60, 1, (bandColour[band3]));
             } 
          }
          else
          {
             glcd.setRect(33, 59, 48, 60, 1, (bandColour[band3]));
          }
          glcd.setRect(51, 59, 52, 60, 1, NAVY);
          glcd.setRect(51, 50, 52, 51, 1, BLACK);
          glcd.setRect(51, 72, 52, 73, 1, BLACK);
          glcd.setRect(51, 84, 52, 85, 1, BLACK);
          break; 
      }
      case 4:
      {
          if(bands == _4_bands)
          {
             item = 5;
          }
          else
          {
             band4 = changeParameter(band4, 0, 11);
             if((band4 != 3) || (band4 != 4) ||(band4 != 7) || (band4 != 8) || (band4 != 9))
             {
               glcd.setRect(33, 72, 48, 73, 1, (bandColour[band4]));
             }
          }
          glcd.setRect(51, 72, 52, 73, 1, NAVY);
          glcd.setRect(51, 59, 52, 60, 1, BLACK);
          glcd.setRect(51, 84, 52, 85, 1, BLACK);
          break; 
      }
      case 5:
      {
          band5 = changeParameter(band5, 1, 11);
          if((band5 != 3) || (band5 != 4) ||(band5 != 7) || (band5 != 8) || (band5 != 9))
          {
              glcd.setRect(33, 84, 48, 85, 1, (bandColour[band5]));
          }
          glcd.setRect(51, 84, 52, 85, 1, NAVY);
          glcd.setRect(51, 72, 52, 73, 1, BLACK);
          break; 
      }
    }
    
    if(bands == _4_bands)
    {
      temp = band3;
    }
    else
    {
      temp = band4;
    }
    
    multiplying_factor = 1;
    
    if((temp >= 0) && (temp <= 6))
    {
      if(temp == 0)
      {
          multiplying_factor = 1;
      }
      else
      {
          for(s = 0; s < temp; s++)
          {
            multiplying_factor *= 10;
          }  
      }
    }
    else
    {
      if(temp == 10)
      {
        multiplying_factor /= 10.0;
      }
      else if(temp == 11)
      {
        multiplying_factor /= 100.0;
      }
    }
    
    if(bands == _4_bands)
    {        
      R = (((band1 * 10.0) + band2) * multiplying_factor);
    }
    else
    {
      R = (((band1 * 100.0) + (band2 * 10.0) + band3) * multiplying_factor);
    }
    
    if(R >= 1000000.0)
    {
       R /= 1000000.0;
       glcd.setStr("M Ohms", 70, 60, CYAN, BLACK); 
    }
    else if((R >= 1000.0) && (R < 1000000.0))
    {
       R /= 1000.0;
       glcd.setStr("K Ohms", 70, 60, CYAN, BLACK); 
    }
    else if((R >= 1.0) && (R < 1000.0))
    {
       glcd.setStr("Ohms  ", 70, 60, CYAN, BLACK);
    }
    else if((R < 1.0) && (R >= 0.001))
    {
      R *= 1000.0;
      glcd.setStr("m Ohms", 70, 60, CYAN, BLACK); 
    }
    
    switch(band5)
    {
      case 1:
      {
        glcd.setStr("1.0% ", 84, 40, CYAN, BLACK);
        break;
      }
      case 2:
      {
        glcd.setStr("2.0 %", 84, 40, CYAN, BLACK);
        break;
      }
      case 5:
      {
        glcd.setStr("0.50%", 84, 40, CYAN, BLACK);
        break;
      }
      case 6:
      {
        glcd.setStr("0.25%", 84, 40, CYAN, BLACK);
        break;
      }
      case 7:
      {
        glcd.setStr("0.10%", 84, 40, CYAN, BLACK);
        break;
      }
      case 8:
      {
        glcd.setStr("0.05%", 84, 40, CYAN, BLACK);
        break;
      }
      case 10:
      {
        glcd.setStr("10.0%", 84, 40, CYAN, BLACK);
        break;
      }
      case 11:
      {
        glcd.setStr("20.0%", 84, 40, CYAN, BLACK);
        break;
      }
      default:
      {
        glcd.setStr("--.-%", 84, 40, CYAN, BLACK);
        break;
      }
    }
    
    if(R < 10)
    {
         tmp = R;
         glcd.setStr(" ", 70, 22, CYAN, BLACK);
         itoa(R, textChar, 10);
         glcd.setStr(textChar, 70, 30, CYAN, BLACK);
         glcd.setStr(".", 70, 38, CYAN, BLACK);
         itoa(((R - tmp) * 10.0), textChar, 10);
         glcd.setStr(textChar, 70, 46, CYAN, BLACK);
    }
    else
    {
        itoa(R, textChar, 10);
        glcd.setStr(textChar, 70, 30, CYAN, BLACK);
    }
}
