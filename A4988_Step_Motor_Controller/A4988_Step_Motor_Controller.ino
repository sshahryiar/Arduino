#include <LiquidCrystal.h>


#define SLEEP               0
#define ENABLE              1
#define DIR                 2
#define STEP                3
#define MS1                11
#define MS2                12
#define MS3                13 

#define SELECT              0
#define LEFT                1
#define DOWN                2
#define UP                  3
#define RIGHT               4


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


const unsigned char pos[4] = {0, 4, 8, 12};
const unsigned char microStep[5] = {0, 1, 2, 3, 7};
const unsigned char outPins[7] = {0, 1, 2, 3, 11, 12, 13};

const unsigned int adcValue[5] = {720, 460, 290, 140, 40};

boolean state = 0;
boolean startUp = 1;

unsigned char val = 0;
unsigned char s = 0;
unsigned char MS = 0;
unsigned char dir = 1;
unsigned char STS = 0;
unsigned char index = 0;

unsigned int SPD = 100;


void welcomeMessage()
{
    lcd.setCursor(0, 0);
    lcd.print("A4988 Step Motor");
    lcd.setCursor(3, 1);
    lcd.print("Controller");
    delay(4000);
    lcd.clear();
}


unsigned char getKey()
{
    unsigned char n = 0;
    unsigned char retVal = 255;
    
    unsigned int key = 0;
    
    key = analogRead(A0);
    
    for(n = 0; n <= 4; n += 1)
    {
        if(key < (adcValue[n]))
        {
            retVal = n;        
        }
    }
    
    return retVal;
}


unsigned int changeValue(unsigned char x_pos, signed int value, signed int valueMax, signed int valueMin, unsigned char type)
{    
        if((getKey() == UP))
        {
            delay(200);
            value += 1;
        }
        
        if((getKey() == DOWN))
        {
            delay(200);
            value -= 1;
        }
        
        if(value > valueMax)
        {
            value = valueMin;
        }
        
        if(value < valueMin)
        {
            value = valueMax;
        }
        
        lcd.setCursor(x_pos, 1);
        
        switch(type)
        {
            case 1:
            {
                if(value == 0)
                {
                    lcd.print("1 ");
                }
                else if(value == 1)
                {
                    lcd.print("2 ");
                }
                else if(value == 2)
                {
                    lcd.print("4 ");
                }
                if(value == 3)
                {
                    lcd.print("8 ");
                }
                if(value == 4)
                {
                    lcd.print("16");
                }
                break;
            }
            case 2:
            {
                lcd.print(value, DEC);
                if((value >= 10) && (value < 100))
                {

                  lcd.setCursor((x_pos + 2), 1);
                  lcd.print(" ");
                }
                else if((value >= 0) && (value < 10))
                {
                  lcd.setCursor((x_pos + 1), 1);
                  lcd.print("  ");
                }
                break;
            }
            case 3:
            {
                if(value == 1)
                {
                    lcd.print("+");
                }
                else
                {
                    lcd.print("-");
                }
                break;
            }
            default:
            {
                if(value == 1)
                {
                     lcd.print("EN ");
                }
                else if(value == 2)
                {
                    lcd.print("RUN");
                }
                else
                {
                    lcd.print("SLP");  
                }
                break;
            }
        }
        return value;
}


void setup()
{
    lcd.begin(16, 2);
    for(s = 0; s <= 6; s += 1)
    {
        pinMode(outPins[s], OUTPUT);
        digitalWrite(outPins[s], LOW);
    }
    welcomeMessage();
    lcd.setCursor(2, 0);
    lcd.print("MS DIR SPD STS");
    lcd.setCursor(0, 1);
    lcd.print("1/");
}


void loop()
{
    if(startUp)
    {
        index += 1;
    }  
    
    if((startUp) && (index > 4))
    {
        index = 0;
        startUp = 0;
    }  
    
    if((getKey() == LEFT) && (index > 0))        
    {
        delay(200);
        index -= 1;
        while((getKey() == LEFT));
    }
    
    if((getKey() == RIGHT) && (index <= 4))
    {
        delay(200);
        index += 1;
        while((getKey() == RIGHT));
    }
    
    if(index > 4)
    {
        index = 0;
    }
    
    lcd.setCursor((pos[index]), 0);
    lcd.print(">");
    lcd.setCursor((pos[(index - 1)]), 0);
    lcd.print(" ");
    lcd.setCursor((pos[(index + 1)]), 0);
    lcd.print(" ");
    
    switch(index)
    {
        case 1:
        {
            dir = changeValue(6, dir, 1, 0, 3);
            break;
        }
        case 2:
        {
            SPD = changeValue(9, SPD, 999, 0, 2);
            break;
        }
        case 3:
        {
            STS = changeValue(13, STS, 2, 0, 0);
            break;
        }
        default:
        {
            MS = changeValue(2, MS, 4, 0, 1);
            break;
        }
    }
    
    val = 1;
    for(s = MS1; s <= MS3; s += 1)
    { 
        digitalWrite(s, (microStep[MS] & val));
        val <<= 1;
    }
    
    switch(dir)
    {
        case 1:
        {
            digitalWrite(DIR, HIGH);
            break;
        }
        default:
        {
            digitalWrite(DIR, LOW);
            break;
        }        
    }
    
    digitalWrite(STEP, state);
    delayMicroseconds((999 - SPD));
    
    switch(STS)
    {
        case 0:
        {
            digitalWrite(SLEEP, LOW);
            break;
        }
        case 1:
        {
            digitalWrite(ENABLE, HIGH);
            break;
        }
        default:
        {
            digitalWrite(SLEEP, HIGH);
            digitalWrite(ENABLE, LOW);
            break;
        }
    }
    
    state = ~state;
}


