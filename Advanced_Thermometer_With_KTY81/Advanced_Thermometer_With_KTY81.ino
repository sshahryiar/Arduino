/*
1. Show temperature in 'C and 'F
2. Show temperature in K
3. Show Max and min Temp (with adjustable update interval)
4. Show Registered Max and Min Temp (can be reset)
5. High and low temp alarm with output and adjustable threshold either in temp or time (Alarm can be individually turned)
6. Show update interval
*/


/*
1. no alarm
2. high temp alarm state
3. low temp alam state
4. alarm muted
5. high temp alarm value
6. low temp alarm value
*/



#include <EEPROM.h>
#include <LiquidCrystal.h>


#define constA           0.0011
#define constB          -1.3724
#define constC           363.63

#define minTemp          -40
#define maxTemp          140
#define thresMin           0
#define thresMax          10

#define EEPROMdly          6
#define debounce          40
#define refreshInterval1 600
#define refreshInterval2 900
#define menuDly          400

#define timeMin          0
#define timeMax          3600

#define setBtn           digitalRead(8)
#define incBtn           digitalRead(9)
#define decBtn           digitalRead(10)

#define outL             11
#define outH             12

#define noInput          0
#define set              1
#define inc              2
#define dec              3

#define EEenlAlarm       0x00
#define EEenhAlarm       0x01
#define EEtThreshold     0x02
#define EEaMax           0x03
#define EEaMin           0x05 
#define EEupdate         0x07 


LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


boolean toggle;
boolean startUp;
boolean enhAlarm;
boolean enlAlarm;
boolean lowAlarm;
boolean highAlarm;

unsigned char tThreshold = 0;
unsigned char systemState = 0;

signed int aMax = 0;
signed int aMin = 0;

unsigned int presentTime = 0;
unsigned int previousTime = 0;
unsigned int updateInterval = 0;

float tAvg = 0.0;
float tMax = 0.0;
float tMin = 0.0;


void initializeVariables()
{
    toggle = 1;
    startUp = 1;
    lowAlarm = 0;
    enhAlarm = 0;
    enlAlarm = 0;
    highAlarm = 0;
}


void resetRecords()
{
    tMax = tAvg;
    tMin = tMax;
}


unsigned char getBtn()
{
    if(setBtn)
    {
        delay(debounce);
        return set;
    }
    else if(incBtn)
    {
        delay(debounce);
        return inc;
    }
    else if(decBtn)
    {
        delay(debounce);
        return dec;
    }
    else
    {
        return noInput;
    }
}


signed int changeValue(unsigned char lcdX, unsigned char lcdY, signed int value, signed int valueMax, signed int valueMin, unsigned char changeRate)
{
    for(;;)
    {
        lcd.setCursor(lcdX, lcdY);
        lcd.print(value);
        lcd.print("   "); 
        
        if(getBtn() == inc)
        {
            value += changeRate;
        }
        if(value > valueMax)
        {
            value = valueMin;
        }
        
        if(getBtn() == dec)
        {
            value -= changeRate;
        }
        if(value < valueMin)
        {
            value = valueMax;
        }
        
        if(getBtn() == set)
        {
            return value;
        }
    }    
}


void EEPROMwriteInt(unsigned int address, unsigned long value)
{
    EEPROM.write(address, (lowByte(value)));
    delay(EEPROMdly);
    EEPROM.write((address + 1), (highByte(value)));
    delay(EEPROMdly);
}


unsigned int EEPROMreadInt(unsigned int address)
{
    unsigned int hb = 0;
    unsigned int lb = 0;
    unsigned int wordValue = 0;
    
    lb = EEPROM.read(address);
    delay(EEPROMdly);
    hb = EEPROM.read((address + 1));
    delay(EEPROMdly);
    wordValue = word(hb, lb);
    return wordValue;
}


void readMemory()
{
    enlAlarm = EEPROM.read(EEenlAlarm);
    delay(EEPROMdly);
    if((enlAlarm > 1) || (enlAlarm < 0))
    {
        EEPROM.write(EEenlAlarm, 0);
        delay(EEPROMdly);
        
        enlAlarm = EEPROM.read(EEenlAlarm);
        delay(EEPROMdly);
    }
    
    enhAlarm = EEPROM.read(EEenhAlarm);
    delay(EEPROMdly);
    if((enhAlarm > 1) || (enhAlarm < 0))
    {
        EEPROM.write(EEenhAlarm, 0);
        delay(EEPROMdly);
        
        enhAlarm = EEPROM.read(EEenhAlarm);
        delay(EEPROMdly);
    }
    
    tThreshold = EEPROM.read(EEtThreshold);
    delay(EEPROMdly);
    if((tThreshold > thresMax) || (tThreshold < thresMin))
    {
        EEPROM.write(EEtThreshold, 4);
        delay(EEPROMdly);
        tThreshold = EEPROM.read(EEtThreshold);
        delay(EEPROMdly);
    }
    
    aMin = ((signed int)EEPROMreadInt(EEaMin));
    if((aMin > maxTemp) || (aMin < minTemp || aMin == -1))
    {
        EEPROMwriteInt(EEaMin, 0);
        aMin = ((signed int)EEPROMreadInt(EEaMin));
    }
    
    aMax = ((signed int)EEPROMreadInt(EEaMax));
    if((aMax > maxTemp) || (aMax < minTemp || aMax == -1))
    {
        EEPROMwriteInt(EEaMax, 60);
        aMax = ((signed int)EEPROMreadInt(EEaMax));
    }
    
    updateInterval = EEPROMreadInt(EEupdate);
    if((updateInterval > timeMax) || (updateInterval < timeMin))
    {
        EEPROMwriteInt(EEupdate, 30);
        updateInterval = EEPROMreadInt(EEupdate);
    }
}


void setupMenu()
{
    boolean done;
    done = 0;
    if(getBtn() == set)
    {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Setup Menu");
        delay(refreshInterval2);
        while(getBtn() == set);
        lcd.clear();
        while(!done)
        {
           lcd.setCursor(0, 0); 
           lcd.print("T Refresh Rate?");  
           lcd.setCursor(0, 1); 
           lcd.print("T/s: ");
           updateInterval = changeValue(5, 1, updateInterval, timeMax, timeMin, 10);
           EEPROMwriteInt(EEupdate, updateInterval);  
           delay(menuDly);
           
           lcd.clear();
           lcd.setCursor(0, 0); 
           lcd.print("Reset Max-Min?");  
           lcd.setCursor(0, 1); 
           lcd.print("Yes=1  No=0");    
           startUp = changeValue(14, 1, startUp, 1, 0, 1);
           delay(menuDly);
          
           lcd.clear();
           lcd.setCursor(0, 0); 
           lcd.print("Low Temp Alarm?");  
           lcd.setCursor(0, 1); 
           lcd.print("Yes=1  No=0");    
           enlAlarm = changeValue(14, 1, enlAlarm, 1, 0, 1);
           EEPROM.write(EEenlAlarm, enlAlarm);
           delay(EEPROMdly);
           delay(menuDly);
           
           if(enlAlarm)
           {
               lcd.clear();
               lcd.setCursor(0, 0); 
               lcd.print("Low Temp Value?"); 
               lcd.setCursor(0, 1); 
               lcd.print("Al min/");  
               lcd.write(0xDF);
               lcd.print("C: ");   
               aMin = changeValue(11, 1, aMin, maxTemp, minTemp, 1); 
               EEPROMwriteInt(EEaMin, aMin);  
               delay(EEPROMdly); 
           }
           delay(menuDly);
           
           lcd.clear();
           lcd.setCursor(0, 0); 
           lcd.print("High Temp Alarm?");  
           lcd.setCursor(0, 1); 
           lcd.print("Yes=1  No=0");    
           enhAlarm = changeValue(14, 1, enhAlarm, 1, 0, 1);
           EEPROM.write(EEenhAlarm, enhAlarm);
           delay(EEPROMdly);
           delay(menuDly);
           
           if(enhAlarm)
           {
               lcd.clear();
               lcd.setCursor(0, 0); 
               lcd.print("High Temp Value?"); 
               lcd.setCursor(0, 1); 
               lcd.print("Al max/");  
               lcd.write(0xDF);
               lcd.print("C: ");   
               aMax = changeValue(11, 1, aMax, maxTemp, minTemp, 1); 
               EEPROMwriteInt(EEaMax, aMax);  
               delay(EEPROMdly); 
           }
           delay(menuDly);
           
           if((enhAlarm) || (enlAlarm))
           {
               lcd.clear();
               lcd.setCursor(0, 0); 
               lcd.print("Threshold Value?"); 
               lcd.setCursor(0, 1); 
               lcd.print("Thres/");  
               lcd.write(0xDF);
               lcd.print("C: ");   
               tThreshold = changeValue(10, 1, tThreshold, thresMax, thresMin, 1); 
               EEPROM.write(EEtThreshold, tThreshold);  
               delay(EEPROMdly); 
           }
           delay(menuDly);
           
           lcd.clear();
           done = true;
        }
    }  
}


unsigned int adcAvg()
{
    unsigned char samples = 0;
    unsigned int tmp = 0;
    
    while(samples < 64)
    {
        tmp += analogRead(0);
        delayMicroseconds(4);
        samples++;
    }
    tmp >>= 6;
    
    return tmp;
}


float getTempinC(unsigned int v)
{
    float tmp = 0.0;
    tmp = (constA * ((float)v) * ((float)v));
    tmp += (constB * ((float)v));
    tmp += constC;
    return tmp;
}


float getTempinF(float tC)
{
    float tF = 0.0;
    tF = ((tC * 1.8) + 32.0);
    return tF;
}


float getTempinK(float tC)
{
    float tK = 0.0;
    tK = (273.0 + tC);
    return tK;
}


void readTemp()
{
    tAvg = adcAvg();
    tAvg = getTempinC(tAvg);
}


void compareTemp()
{
    if(startUp)
    {
        resetRecords();
        startUp = false;
    }
    else
    {
        if((tAvg > tMax) && (tAvg < maxTemp)) 
        {
            tMax = tAvg;
        }
        if((tAvg < tMin) && (tAvg > minTemp))
        {
            tMin = tAvg;
        }
    }
    refreshData();
}


void monitorAlarms()
{
    alarmMute();
    
    if(enhAlarm)
    {
        if((tAvg >= aMax) && (!highAlarm))
        {
            highAlarm = true;
            systemState = 1;
            digitalWrite(outH, HIGH);
        }
        if((tAvg < (aMax - tThreshold)) && (highAlarm))
        {
            highAlarm = false;
            digitalWrite(outH, LOW);
        }
    }
    if(enlAlarm)
    {
        if((tAvg <= aMin) && (!lowAlarm))
        {
            lowAlarm = true;
            systemState = 2;
            digitalWrite(outL, HIGH);
        }
        if((tAvg > (aMin + tThreshold)) && (lowAlarm))
        {
            lowAlarm = false;
            digitalWrite(outL, LOW);
        }
    }
    
    if((!enlAlarm) && (!enhAlarm))
    {
        systemState = 0;
    }
}


void alarmMute()
{
    if((getBtn() == inc) && (enhAlarm))
    { 
        digitalWrite(outH, LOW); 
        highAlarm = false;
        enhAlarm = false;
        systemState = 3;
    }
    if((getBtn() == dec) && (enlAlarm))
    {
        digitalWrite(outL, LOW);
        lowAlarm = false;   
        enlAlarm = false;
        systemState = 3;
    }
}


void refreshData()
{
    presentTime = (millis() / 1000); 
    if((presentTime - previousTime) >= updateInterval)
    {
        previousTime = presentTime;
        startUp = true;
    }   
}


void outputDisplay()
{
    static char disp;
    unsigned int sec = 0;
    
    sec = (updateInterval + previousTime - presentTime);
    lcd.setCursor(0, 0); 
    switch(disp)
    {
        case 1:
        {
            lcd.print("T Avg: ");
            lcd.setCursor(7, 0);
            lcd.print(getTempinF(tAvg));
            lcd.setCursor(11, 0);
            lcd.write(0xDF);
            lcd.print("F  ");
            break;
        }
        case 2:
        {
            lcd.print("T Avg: ");
            lcd.setCursor(7, 0);
            lcd.print(getTempinK(tAvg));
            lcd.setCursor(12, 0);
            lcd.print("K  ");
            break;
        }
        case 3:
        {
            lcd.print("T Max: ");
            lcd.setCursor(7, 0);
            lcd.print(tMax);
            lcd.setCursor(11, 0);
            lcd.write(0xDF);
            lcd.print("C  ");
            break;
        }
        case 4:
        {
            lcd.print("T Min: ");
            lcd.setCursor(7, 0);
            lcd.print(tMin);
            lcd.setCursor(11, 0);
            lcd.write(0xDF);
            lcd.print("C  ");
            break;
        }
        default:
        { 
            lcd.print("T Avg: ");
            lcd.setCursor(7, 0);
            lcd.print(tAvg);
            lcd.setCursor(11, 0);
            lcd.write(0xDF);
            lcd.print("C  ");
            break;
        }
    }
    
    lcd.setCursor(0, 1);
    if(toggle)
    {
        switch(systemState)
        {
            case 1: 
            {
                lcd.print("<<H Temp Alarm>>");
                break;
            }
            case 2: 
            {
                lcd.print("<<L Temp Alarm>>");
                break;
            }
            case 3:
            {
                lcd.print("<<Alarm Muted!>>");
                break;
            }
            default:
            {
                lcd.print("No Alarm  s:");
                lcd.setCursor(12, 1);
                lcd.print(sec);
                lcd.print("   ");
                break;
            }
        }  
    }
    else
    {
         if(highAlarm)
         {
             lcd.print("H. T. Al/");
             lcd.write(0xDF);
             lcd.print("C: ");
             lcd.print(aMax);
             lcd.print("   ");
             delay(refreshInterval1);
         }
         if(lowAlarm)
         {
             lcd.print("L. T. Al/");
             lcd.write(0xDF);
             lcd.print("C: ");
             lcd.print(aMin);
             lcd.print("   ");
             delay(refreshInterval1);
         }
    }
    
    disp++;
    if(disp > 4)
    {
        disp = 0;
    }
    toggle = ~toggle;
    delay(refreshInterval2);
}


void setup()
{
    initializeVariables();
    pinMode(setBtn, INPUT);
    pinMode(incBtn, INPUT);
    pinMode(decBtn, INPUT);
    pinMode(outL, OUTPUT);
    pinMode(outH, OUTPUT);
    digitalWrite(outL, LOW);
    digitalWrite(outL, LOW);
    readMemory();
    lcd.begin(16, 2);
}


void loop()
{
    setupMenu();
    readTemp();
    compareTemp();
    monitorAlarms();
    outputDisplay();
}

