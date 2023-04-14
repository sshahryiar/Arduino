#include <LiquidCrystal.h>


#define dly1          2200
#define dly2          1900
#define dly3           120
#define dly4           600

#define N                0
#define R                1
#define U                2
#define D                3
#define L                4
#define Sel              5

#define testPin         22


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


unsigned char partNumber[5] = {0x00, 0x00, 0x00, 0x00, 0x00};


void setup();
void loop();
void welcomeMessage();
unsigned char getKeyboard();
void getPartNumber();
void testPart();
void configurePins(unsigned int ioStates, unsigned int pinLogic);
void testResult(unsigned char res);


void loop()
{
    getPartNumber();
    testPart();
}


void setup()
{
    lcd.begin(16, 2);
    lcd.clear(); 
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    welcomeMessage();
    lcd.clear(); 
}


void welcomeMessage()
{
    lcd.setCursor(2, 0);
    lcd.print("Arduino Mega");
    lcd.setCursor(3, 1);
    lcd.print("I.C Tester");
    delay(dly1);
    lcd.clear(); 
    lcd.setCursor(2, 0);
    lcd.print("Developed by");
    lcd.setCursor(0, 1);
    lcd.print("Shawon Shahryiar");
    delay(dly1);
}


unsigned char getKeyboard()
{
    unsigned long adcValue = 0;
    
    adcValue = analogRead(A0);
    delay(dly3);
    
    if((adcValue >= 0) && (adcValue < 60))
    {
        return R; 
    }
    else if(((adcValue >= 80) && (adcValue < 160)))
    {
        return U; 
    }
    else if((adcValue >= 220) && (adcValue < 340))
    {
        return D; 
    }
    else if(((adcValue >= 360) && (adcValue < 530)))
    {
        return L; 
    }
    else if(((adcValue >= 600) && (adcValue < 800)))
    {
        return Sel; 
    }
    else
    {
        return N;
    }
}


void getPartNumber()
{
    signed char s = 0;
    signed char num = 0;
    
    unsigned char n = 0;
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("IC Part Number");
    
    do
    {
        n = getKeyboard();
        
        if(n == L)
        {
            s -= 1;
            num = 0;
        }
        
        if(n == R)
        {
            s += 1;
            num = 0;
        }
        
        if(s > 4)
        {
            s = 4;
        }
        if(s <= 0)
        {
            s = 0;
        }
        
        if(n == U)
        {
            num += 1;
        }
        
        if(n == D)
        { 
            num -= 1;
        }
        
        if(num > 10)
        {
            num = 0;
        }
        if(num < 0)
        {
            num = 10;
        }
        
        partNumber[s] = num;
        lcd.setCursor((s + 5), 1);
        
        if((num >= 0) && (num <= 9))
        {
            lcd.print(num);
        }
        else
        {
            lcd.print(" ");
        }
    }while(n != Sel);
}


void testPart()
{
    unsigned char x = 0;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Part No. ");
    
    for(x = 0; x < 5; x += 1)
    {
        lcd.setCursor((11 + x), 0);
        {
            if((partNumber[x] >= 0) && (partNumber[x] <= 9))
            {
                lcd.print(partNumber[x]);
            }
        }
    }
    delay(dly4);
    if((partNumber[0] == 0x07))
    {    
        if(partNumber[1] == 0x04)
        {
            if((partNumber[2] == 0x01) && (partNumber[3] >= 0x0A) && (partNumber[4] >= 0x0A))
            {
                configurePins(0xFB1F, 0xF62F);
                
                if(digitalRead(9 + testPin) == 0)
                {
                    testResult(1);    
                }
                else
                {
                    testResult(2);
                }
            }
            else if((partNumber[2] <= 0x09) && (partNumber[3] <= 0x09))
            {
                    if(((partNumber[2] == 0x00) && (partNumber[3] == 0x00)) || ((partNumber[2] == 0x00) && (partNumber[3] == 0x03)) || ((partNumber[2] == 0x02) && (partNumber[3] == 0x06)) || ((partNumber[2] == 0x03) && (partNumber[3] == 0x07)) || ((partNumber[2] == 0x03) && (partNumber[3] == 0x08)) || ((partNumber[2] == 0x01) && (partNumber[3] == 0x03) && (partNumber[4] == 0x02)))
                    {
                         configurePins(0x1224, 0xFFBF);
                         
                         if((digitalRead(testPin + 2) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 12) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else if(((partNumber[2] == 0x00) && (partNumber[3] == 0x02)) || ((partNumber[2] == 0x02) && (partNumber[3] == 0x08)) || ((partNumber[2] == 0x03) && (partNumber[3] == 0x03)))
                    {
                         configurePins(0x4989, 0xFFBF);
                         
                         if((digitalRead(testPin) == 0) && (digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 11) == 0) && (digitalRead(testPin + 14) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else if(((partNumber[2] == 0x00) && (partNumber[3] == 0x04)) || ((partNumber[2] == 0x00) && (partNumber[3] == 0x05)) || ((partNumber[2] == 0x00) && (partNumber[3] == 0x06))  || ((partNumber[2] == 0x01) && (partNumber[3] == 0x04)) || ((partNumber[2] == 0x01) && (partNumber[3] == 0x06)))
                    {
                         configurePins(0x2BAA, 0xFFBF);
                         
                         if((digitalRead(testPin + 1) == 0) && (digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 11) == 0) && (digitalRead(testPin + 13) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else if((partNumber[2] == 0x00) && (partNumber[3] == 0x07))
                    {
                         configurePins(0x2BAA, 0x8000);
                         
                         if((digitalRead(testPin + 1) == 0) && (digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 11) == 0) && (digitalRead(testPin + 13) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else if(((partNumber[2] == 0x00) && (partNumber[3] == 0x08)) || ((partNumber[2] == 0x00) && (partNumber[3] == 0x09)))
                    {
                         configurePins(0x1224, 0xD5AA);
                         
                         if((digitalRead(testPin + 2) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 12) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else if((partNumber[2] == 0x03) && (partNumber[3] == 0x02))
                    {
                         configurePins(0x1224, 0x8000);
                         
                         if((digitalRead(testPin + 2) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 12) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else if((partNumber[2] == 0x08) && (partNumber[3] == 0x06))
                    {
                         configurePins(0x1224, 0xFFBF);
                         
                         if((digitalRead(testPin + 2) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 12) == 0))
                         {
                             testResult(1);    
                         }
                         else
                         {
                             testResult(2);
                         }
                    }
                    else
                    {
                        testResult(0);  
                    }
                }
                else
                {
                    testResult(0);  
                }
            }
        }
        else if((partNumber[0] == 0x04) && (partNumber[1] == 0x00))
        {
            if((partNumber[2] <= 0x09) && (partNumber[3] <= 0x09))
            {
                if((partNumber[2] == 0x00) && (partNumber[3] == 0x01))  
                {
                    configurePins(0x8C19, 0xFF7F);
                    
                    if((digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 11) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if(((partNumber[2] == 0x01) && (partNumber[3] == 0x01)) || ((partNumber[2] == 0x09) && (partNumber[3] == 0x03)))  
                {
                    configurePins(0x8C19, 0xFF7F);
                    
                    if((digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 11) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if((partNumber[2] == 0x04) && (partNumber[3] == 0x09))  
                {
                    configurePins(0x4A2A, 0xFF7F);
                    
                    if((digitalRead(testPin + 1) == 0) && (digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 11) == 0) && (digitalRead(testPin + 14) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if((partNumber[2] == 0x05) && (partNumber[3] == 0x00))  
                {
                    configurePins(0x4A2A, 0x8000);
                    
                    if((digitalRead(testPin + 1) == 0) && (digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0) && (digitalRead(testPin + 11) == 0) && (digitalRead(testPin + 14) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if(((partNumber[2] == 0x06) && (partNumber[3] == 0x09)) || ((partNumber[2] == 0x01) && (partNumber[3] == 0x00) && (partNumber[4] == 0x06)))  
                {
                    configurePins(0x9555, 0xFF7F);
                    
                    if((digitalRead(testPin + 2) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 6) == 0) && (digitalRead(testPin + 8) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 12) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if((partNumber[2] == 0x07) && (partNumber[3] == 0x00))  
                {
                    configurePins(0x8C19, 0xFF7F);
                    
                    if((digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 11) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if((partNumber[2] == 0x07) && (partNumber[3] == 0x01))  
                {
                    configurePins(0x8C19, 0x8000);
                    
                    if((digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 11) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if((partNumber[2] == 0x07) && (partNumber[3] == 0x07))  
                {
                    configurePins(0x8C19, 0x5555);
                    
                    if((digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 11) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
                else if((partNumber[2] == 0x08) && (partNumber[3] == 0x01))  
                {
                    configurePins(0x8C19, 0x5555);
                    
                    if((digitalRead(testPin + 3) == 0) && (digitalRead(testPin + 4) == 0) && (digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 11) == 0))
                    {
                        testResult(1);    
                    }
                    else
                    {
                        testResult(2);
                    }
                }
            }
            else
            {
                testResult(0);  
            }
        }
        else if(partNumber[0] == 0x03)
        {
            if(((partNumber[1] == 0x05) && (partNumber[2] == 0x08)) || ((partNumber[1] == 0x09) && (partNumber[2] == 0x03)))
            {
                configurePins(0xF41F, 0xFE3F); 
             
                if((digitalRead(testPin + 10) == 0) && (digitalRead(testPin + 4) == 0))
                {
                    testResult(1);    
                }
                else
                {
                    testResult(2);
                }   
            }
            else if((partNumber[1] == 0x02) && (partNumber[2] == 0x04))
            {
                configurePins(0xC183, 0xE3D7);
                    
                if((digitalRead(testPin + 1) == 0) && (digitalRead(testPin + 7) == 0) && (digitalRead(testPin + 8) == 0) && (digitalRead(testPin + 14) == 0))
                {
                    testResult(1);    
                }
                else
                {
                    testResult(2);
                }
            }
            else if((partNumber[1] == 0x03) && (partNumber[2] == 0x09))
            {
                configurePins(0xE007, 0xE55E);
                    
                if((digitalRead(testPin + 1) == 0) && (digitalRead(testPin + 2) == 0) && (digitalRead(testPin + 13) == 0) && (digitalRead(testPin + 14) == 0))
                {
                    testResult(1);    
                }
                else
                {
                    testResult(2);
                }
            }
            else
            {
                testResult(0);  
            }
        }
        else if((partNumber[0] == 0x05) && (partNumber[1] == 0x05))
        {
            if(partNumber[2] == 0x05)
            {
                configurePins(0xF04F, 0xFAEF); 
             
                if((digitalRead(testPin + 6) == 0))
                {
                    testResult(1);    
                }
                else
                {
                    testResult(2);
                }  
            }
            else if(partNumber[2] == 0x06)
            {
                configurePins(0x8221, 0xD775); 
             
                if((digitalRead(testPin + 5) == 0) && (digitalRead(testPin + 9) == 0))
                {
                    testResult(1);    
                }
                else
                {
                    testResult(2);
                }  
            }
            else
            {
                testResult(0);  
            }
        }
        else
        {
            testResult(0);  
        }
}


void configurePins(unsigned int ioStates, unsigned int pinLogic) 
{
    unsigned char s = 0;
    unsigned int tmp1 = 0;
    unsigned int tmp2 = 0;
    
    tmp1 = ioStates;
    tmp2 = pinLogic;
    
    for(s = 0; s < 16; s += 1)
    {
        if((tmp1 & 0x0001) != 0)
        {
            pinMode((s + testPin), INPUT_PULLUP);
        }
        else
        {
            pinMode((s + testPin), OUTPUT);
            if((tmp2 & 0x0001) != 0)
            {
                digitalWrite((testPin + s), HIGH);
            }
            else
           {
                digitalWrite((testPin + s), LOW);
           } 
        }
        tmp1 >>= 1;
        tmp2 >>= 1;
    }
}


void testResult(unsigned char res)
{
    switch(res)
    {
        case 1:
        {
            lcd.setCursor(4, 1);
            lcd.print("IC Okay.");
            break;
        }
        case 2:
        {
            lcd.setCursor(3, 1);
            lcd.print("Faulty IC.");
            break;
        }
        default:
        {
            lcd.setCursor(2, 1);
            lcd.print("Unknown IC!");
            break;
        }
    }
    configurePins(0xFFFF, 0xFFFF);
    delay(dly2);
}



