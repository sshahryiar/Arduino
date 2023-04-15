#include <LiquidCrystal.h>


#define SET    P2_2
#define UP     P2_3
#define DOWN   P2_4
#define ESC    P2_5

#define dly    90


LiquidCrystal lcd(P2_0, P2_1, P1_2, P1_3, P1_4, P1_5);


boolean start_flag = false;
boolean state_flag = false;
boolean change_state = false;

signed char menu = 0;
signed char hr_t = 0;
signed char min_t = 0;
signed char sec_t = 0;
signed char hr_on = 0;
signed char min_on = 0;
signed char sec_on = 0;
signed char hr_off = 0;
signed char min_off= 0;
signed char sec_off = 0;

unsigned long previous_s = 0;
unsigned long present_s = 0;


void show_numbers(unsigned char x_pos, unsigned char y_pos, unsigned char value)
{
    if(value < 10)
    {
      lcd.setCursor(x_pos, y_pos);
      lcd.print("0");
      lcd.setCursor((x_pos + 1), y_pos);
      lcd.print(value);
    }
    else
    {
      lcd.setCursor(x_pos, y_pos);
      lcd.print(value);
    }
}


void show_time()
{
    lcd.setCursor(0, 0);
    lcd.print("State:"); 
    lcd.setCursor(2, 1);
    lcd.print("  "); 
    show_numbers(4, 1, hr_t);
    lcd.setCursor(6, 1);
    lcd.print(":");  
    show_numbers(7, 1, min_t);
    lcd.setCursor(9, 1);
    lcd.print(":");  
    show_numbers(10, 1, sec_t);
    lcd.setCursor(12, 1);
    lcd.print("  "); 
}


void on_off_state_display(boolean value)
{
     if(value == 1)
     {
        lcd.setCursor(15, 0);
        lcd.print(" ");  
        delay(dly);       
        lcd.setCursor(15, 0);
        lcd.print("*");  
        delay(dly); 
        lcd.setCursor(15, 1);
        lcd.print(" ");  
     }
     if(value == 0)
     {
        lcd.setCursor(15, 1);
        lcd.print(" ");  
        delay(dly);       
        lcd.setCursor(15, 1);
        lcd.print("*");  
        delay(dly); 
        lcd.setCursor(15, 0);
        lcd.print(" ");  
     }
}


unsigned char inc_dec(signed char value, signed char value_max, signed char value_min,  unsigned char x_pos, unsigned char y_pos, boolean disp)
{
    if(digitalRead(UP) == true)
    {
         value++;
    }
    if(value > value_max)
    {
        value = value_min;
    }
    
    if(digitalRead(DOWN) == true)
    {
         value--;
    }
    if(value < value_min)
    {
        value = value_max;
    }  
  
    if(digitalRead(SET) == true)
    {
         while(digitalRead(SET) == true);
         menu++;
         delay(600);
    }
    if(menu > 8)
    {
        menu = 8;
    }
    
    if(digitalRead(ESC) == true)
    {
         while(digitalRead(ESC) == true);
         menu--;
         delay(600);
    }
    if(menu < 1)
    {
        menu = 8;
    }
    
    if(disp)
    {
         lcd.setCursor(x_pos, y_pos);
         lcd.print("  ");
         delay(dly);
         show_numbers(x_pos, y_pos, value);
         delay(dly);
    }
    else
    {
         on_off_state_display(value);
    }
    
    return ((unsigned char)value);
}


void set_time()
{
    if(digitalRead(SET) == true)
    {
         while(digitalRead(SET) == true);
         lcd.clear(); 
         lcd.setCursor(0, 0);
         lcd.print("T On : 00:00:00 ");
         lcd.setCursor(0, 1);
         lcd.print("T Off: 00:00:00 ");         
         start_flag = false;
         change_state = false;
         menu = 1;
         delay(900);
    }
    while(menu)
    {
        switch(menu)
        {
            case 1:
            {
               hr_on = inc_dec(hr_on, 23, 0, 7, 0, 1); 
               break;   
            }
            case 2:
            {
               min_on = inc_dec(min_on, 59, 0, 10, 0, 1); 
               break;   
            }
            case 3:
            {
               sec_on = inc_dec(sec_on, 59, 0, 13, 0, 1); 
               break;   
            }
            case 4:
            {
               hr_off = inc_dec(hr_off, 23, 0, 7, 1, 1); 
               break;   
            }
            case 5:
            {
               min_off = inc_dec(min_off, 59, 0, 10, 1, 1); 
               break;   
            }
            case 6:
            {
               sec_off = inc_dec(sec_off, 59, 0, 13, 1, 1); 
               break;   
            }
            case 7:
            {
               state_flag = inc_dec(state_flag, 1, 0, 15, 0, 0); 
               break;   
            }  
            case 8:
            {
               lcd.clear(); 
               change_state = true;
               menu = 0; 
            }
            default:
            {
               break;
            }
        }
    }    
}


void setup()
{
  lcd.begin(16, 2);
  lcd.clear(); 
  pinMode(SET, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(ESC, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  change_state = true;
}

void loop()
{
    set_time();
    if(menu == 0)
    {
      show_time(); 
      
      if(digitalRead(ESC) == true)
      {
           while(digitalRead(ESC) == true);
           delay(600);
           change_state = true;
           start_flag ^= true;
      }
      
      if(start_flag == true)
      {
           present_s = millis();
           
           if((present_s - previous_s) >= 999)
           {
               sec_t--;
               if((sec_t < 0) && (min_t > 0))
               {
                   sec_t = 59;
                   min_t--;
               }    
               if((sec_t < 0) && (min_t == 0))
               {
                   sec_t = 0;
               }       
               if((min_t < 0) && (hr_t > 0))
               {
                   min_t = 59;
                   hr_t--;
               }
               if((min_t < 0) && (hr_t == 0))
               {
                   min_t = 0;
               }
               
               if((change_state == false) && (hr_t == 0) && (min_t == 0) && (sec_t == 0)) 
               {
                   state_flag ^= true;
                   change_state = true;
               }
             
               previous_s = present_s;
           }
      }
      
      if(change_state == true)
      {
          if(state_flag == true)
          {
              hr_t = hr_on;
              min_t = min_on;
              sec_t = sec_on;
              digitalWrite(RED_LED, LOW);
              digitalWrite(GREEN_LED, HIGH);
              lcd.setCursor(13, 0);
              lcd.print(" ON"); 
          }
          else
          {
              hr_t = hr_off;
              min_t = min_off;
              sec_t = sec_off;
              digitalWrite(RED_LED, HIGH);
              digitalWrite(GREEN_LED, LOW);
              lcd.setCursor(13, 0);
              lcd.print("OFF");
          }
         change_state = false;
      }
    }    
}
