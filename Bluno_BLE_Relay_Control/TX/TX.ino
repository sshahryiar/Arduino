#include <LiquidCrystal.h>


#define SELECT              0
#define LEFT                1
#define DOWN                2
#define UP                  3
#define RIGHT               4

#define LED                13


boolean toggle = 0;
boolean states = 0;
boolean state_1 = 0;
boolean state_2 = 0;
boolean state_3 = 0;
boolean state_4 = 0;
boolean state_5 = 0;
boolean state_6 = 0;
boolean state_7 = 0;
boolean state_8 = 0;

signed char s = 0;


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


unsigned char getKey()
{
    unsigned int adcValue = 0;
    
    adcValue = analogRead(A0);
    
    if((adcValue >= 0) && (adcValue < 60))
    {
        return RIGHT; 
    }
    else if(((adcValue >= 80) && (adcValue < 160)))
    {
        return UP; 
    }
    else if((adcValue >= 220) && (adcValue < 340))
    {
        return DOWN; 
    }
    else if(((adcValue >= 360) && (adcValue < 530)))
    {
        return LEFT; 
    }
    else if(((adcValue >= 600) && (adcValue < 800)))
    {
        return SELECT; 
    }
    else
    {
        return 255;
    }
}


void show_state(boolean state)
{
      lcd.setCursor(13, 1);
      if(state)
      {
        lcd.print(" ON");
      }
      else
      {
        lcd.print("OFF");
      }
}


boolean change_state(boolean state)
{
      if(getKey() == UP)
      {
          state = true; 
      }
      if(getKey() == DOWN)
      {
          state = false;
      }
      return state;
}


void setup()
{
    lcd.begin(16, 2);
    lcd.clear(); 
    Serial.begin(9600);
    Serial.flush();
    lcd.setCursor(0, 0);
    lcd.print("Relay:");
    lcd.setCursor(0, 1);
    lcd.print("State:");
    pinMode(LED, OUTPUT);
}


void loop()
{
        if(getKey() == SELECT)
        {
            switch(s)
            {
              case 1:
              {
                if(states == true)
                {
                    Serial.println(2);
                } 
                else
                {
                    Serial.println(22);
                } 
                break;
              }
              case 2:
              {
                if(states == true)
                {
                    Serial.println(3);
                } 
                else
                {
                    Serial.println(33);
                } 
                break;
              }
              case 3:
              {
                if(states == true)
                {
                    Serial.println(4);
                } 
                else
                {
                    Serial.println(44);
                } 
                break;
              }
              case 4:
              {
                if(states == true)
                {
                    Serial.println(5);
                } 
                else
                {
                    Serial.println(55);
                } 
                break;
              }
              case 5:
              {
                if(states == true)
                {
                    Serial.println(6);
                } 
                else
                {
                    Serial.println(66);
                } 
                break;
              }
              case 6:
              {
                if(states == true)
                {
                    Serial.println(7);
                } 
                else
                {
                    Serial.println(77);
                } 
                break;
              }
              case 7:
              {
                if(states == true)
                {
                    Serial.println(8);
                } 
                else
                {
                    Serial.println(88);
                } 
                break;
              }
              default:
              {
                if(states == true)
                {
                    Serial.println(1);
                } 
                else
                {
                    Serial.println(11);
                } 
                break;
              }
            }
        }
  
        if(getKey() == RIGHT)
        {
            s++;
        } 
        if(getKey() == LEFT)
        {
            s--;
        }
        
        if(s > 7)
        {
          s = 0;
        }
        if(s  < 0)
        {
          s = 7; 
        }
  
        lcd.setCursor(15, 0);
        lcd.print((s + 1));
        switch(s)
        {
          case 1:
          {
            state_2 = change_state(state_2);
            show_state(state_2);
            states = state_2; 
            break;
          }
          case 2:
          {
            state_3 = change_state(state_3);
            show_state(state_3);
            states = state_3; 
            break;
          }
          case 3:
          {
            state_4 = change_state(state_4);
            show_state(state_4);
            states = state_4; 
            break;
          }
          case 4:
          {
            state_5 = change_state(state_5);
            show_state(state_5);
            states = state_5; 
            break;
          }
          case 5:
          {
            state_6 = change_state(state_6);
            show_state(state_6);
            states = state_6; 
            break;
          }
          case 6:
          {
            state_7 = change_state(state_7);
            show_state(state_7);
            states = state_7; 
            break;
          }
          case 7:
          {
            state_8 = change_state(state_8);
            show_state(state_8);
            states = state_8; 
            break;
          }
          default:
          {
            state_1 = change_state(state_1);
            show_state(state_1);
            states = state_1; 
            break;
          }
        }
        
       digitalWrite(LED, toggle); 
       delay(99); 
       toggle = ~toggle;
}
