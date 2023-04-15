#include <EEPROM.h>


#define LED_H_PIN       13
#define LED_C_PIN       12
#define LED_E1_PIN      11
#define LED_E2_PIN      10
#define BEEPER_PIN      3       
#define LATCH_PIN       4
#define RLA1_PIN        5
#define RLA2_PIN        6
#define CLK_PIN         7
#define DATA_PIN        8

#define POT_PIN         A0
#define LM35_PIN        A4    
#define SET_BUTTON_PIN  A1
#define INC_BUTTON_PIN  A2
#define DEC_BUTTON_PIN  A3

#define T_max           999
#define T_min           000
#define TL_nom          200
#define TH_nom          400
#define TD_nom          100

#define TL_LB_EE        0x00
#define TL_HB_EE        0x01
#define TH_LB_EE        0x02
#define TH_HB_EE        0x03
#define TD_LB_EE        0x04
#define TD_HB_EE        0x05


unsigned char pos = 0;
unsigned char menu = 0;

signed int TL = TL_nom;
signed int TH = TH_nom;
signed int TD = TD_nom;
signed int TC = 000;


byte buf[0x04] = {0xFF, 0xFF, 0xFF, 0xFF};

const byte segment_map[0x04] = {0xF1, 0xF2, 0xF4, 0xF8};
const byte num[0x0A] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};


ISR(TIMER1_OVF_vect)
{
    TCNT1H = 0xFF;
    TCNT1L = 0x06;

    write_bytes_74HC595(segment_map[pos], buf[pos]);

    pos++;

    if(pos >= 4)
    {
      pos = 0;
    }
}

void setup() 
{
    unsigned char pin = 0;

    noInterrupts(); 
    
    for(pin = 3; pin < 14; pin++)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    for(pin = A0; pin <= A4; pin++)
    {
        pinMode(pin, INPUT);
    }

    TCCR1A = 0x00;
    TCCR1B = 0x03;
    TCNT1H = 0xFF;
    TCNT1L = 0x06;
    TIMSK1 = 0x01;
    
    interrupts();

     read_memory();
}

void loop()
{
    settings();
    get_temperature();
    compare_temperatures();
    check_alarms();
}


void read_memory()
{
    TL = read_ROM(TL_LB_EE);
    delay(40);

    TH = read_ROM(TH_LB_EE);
    delay(40);

    TD = read_ROM(TD_LB_EE);
    delay(40);

    if((TL > T_max) || (TL < T_min))
    {
        TL = TL_nom;
        wrtite_ROM(TL_LB_EE, TL);        
    }

    if((TH > T_max) || (TH < T_min))
    {
        TH = TH_nom;
        wrtite_ROM(TH_LB_EE, TH);        
    }

    if((TD > T_max) || (TD < T_min))
    {
        TD = TD_nom;
        wrtite_ROM(TD_LB_EE, TD);        
    }
}


void wrtite_ROM(unsigned char address, unsigned int value)
{
    unsigned char lb = 0x00;
    unsigned char hb = 0x00;

    lb = (value & 0x00FF);
    hb = ((value & 0xFF00) >> 0x08);

   EEPROM.write(address, lb);
   delay(20);
   
   EEPROM.write((address + 1), hb); 
   delay(20);
}


unsigned int read_ROM(unsigned char address)
{
    unsigned char lb = 0x00;
    unsigned char hb = 0x00;

    unsigned int res = 0x0000;
    
    lb = EEPROM.read(address);
    delay(20);
    
    hb = EEPROM.read(address + 1);
    delay(20);

    res = make_word(hb, lb);

    return res;
}


unsigned int make_word(unsigned char hb, unsigned char lb)
{
    unsigned int temp = 0x0000;

    temp = hb;
    temp <<= 0x08;
    temp |= lb;

    return temp;
}


void update_display()
{
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(LATCH_PIN, LOW); 
}


void write_74HC595(unsigned char value)
{                      
    unsigned char clk = 0x08; 
                                  
    while(clk > 0) 
    {                                                   
        digitalWrite(CLK_PIN, LOW);  
        
        if((value & 0x80) != 0x00)
        {
            digitalWrite(DATA_PIN, HIGH);
        }
        else
        {
            digitalWrite(DATA_PIN, LOW);
        }
        
        digitalWrite(CLK_PIN, HIGH);          
        value <<= 1;         
          
        clk--;                              
    }                                   
}


void write_bytes_74HC595(unsigned char hb, unsigned char lb)
{      
    write_74HC595(lb);
    write_74HC595(hb);

    update_display();
}


void make_number(signed int value, unsigned char disp_type)
{
    unsigned char ch = 0;

    memset(buf, 0xFF, sizeof(buf));

    if((value >= 0) && (value <= 9))
    {
        buf[0x01] = (num[0] & 0x7F);
        ch = value;
        buf[0x02] = num[ch];
    }

    else if((value > 9) && (value <= 99))
    {
        ch = (value / 10);
        buf[0x01] = (num[ch] & 0x7F);
        ch = (value % 10);
        buf[0x02] = num[ch];
    }

    else if((value > 99) && (value <= 999))
    {
        ch = (value / 100);
        buf[0x00] = num[ch];
        ch = ((value / 10) % 10);
        buf[0x01] = (num[ch] & 0x7F);
        ch = (value % 10);
        buf[0x02] = num[ch];
    }
    else
    {
        memset(buf, 0xBF, 0x03);
    }

    switch(disp_type)
    {
        case 1:
        {
            buf[0x03] = 0xC7;
            break;
        }
        case 2:
        {
            buf[0x03] = 0x89;
            break;
        }
        case 3:
        {
            buf[0x03] = 0xA1;
            break;
        }
        default:
        {
            buf[0x03] = 0xC6;
            break;
        }
    }
}


unsigned int adc_avg()
{
    unsigned char samples = 32;
    unsigned int avg = 0;

    while(samples > 0)
    {
        avg += analogRead(LM35_PIN);
        delayMicroseconds(10);
        samples--;
    };

    avg *= 10;
    avg >>= 6;

    return avg;
}


unsigned int change_value(signed int value, signed int value_max, signed int value_min)
{
    if(digitalRead(INC_BUTTON_PIN) == false)
    {
      delay(200);
      
      if(digitalRead(INC_BUTTON_PIN) == false)
      {
        value += 10;
      }
      else
      {
        value++;
      }
      digitalWrite(BEEPER_PIN, LOW);
      delay(4);
      digitalWrite(BEEPER_PIN, HIGH);
    }

    if(value > value_max)
    {
       value = value_min;
    }

    if(digitalRead(DEC_BUTTON_PIN) == false)
    {
      delay(200);
      
      if(digitalRead(DEC_BUTTON_PIN) == false)
      {
        value -= 10;
      }
      else
      {
        value--;
      }
      digitalWrite(BEEPER_PIN, LOW);
      delay(4);
      digitalWrite(BEEPER_PIN, HIGH);
    }

    if(value < value_min)
    {
       value = value_max;
    }

    return value;
}


void get_temperature()
{
    if(menu == 0)
    {
        TC = adc_avg();
        make_number(TC, 0);
        delay(400);
    }
}


void settings()
{
    if(digitalRead(SET_BUTTON_PIN) == false)
    {
       digitalWrite(BEEPER_PIN, LOW);
       delay(40);
       digitalWrite(BEEPER_PIN, HIGH);
       while(digitalRead(SET_BUTTON_PIN) == false);   
       menu++;
    }

    switch(menu)
    {
        case 1:
        {
            TH = change_value(TH, 999, 0);
            make_number(TH, 2);
            break;
        }
        case 2:
        {
            TL = change_value(TL, TH, 0);
            make_number(TL, 1);
            break;
        }

        case 3:
        {
            TD = change_value(TD, 100, 0);
            make_number(TD, 3);
            break;
        }

        default:
        {
            wrtite_ROM(TL_LB_EE, TL); 
            wrtite_ROM(TH_LB_EE, TH);
            wrtite_ROM(TD_LB_EE, TD);  
            menu = 0;
            break; 
        }
    }
}


void compare_temperatures()
{
    if(TC > TH)
    {
        digitalWrite(RLA1_PIN, LOW);
        digitalWrite(RLA2_PIN, HIGH);
        digitalWrite(LED_C_PIN, LOW);
        digitalWrite(LED_H_PIN, HIGH);      
    }
    
    if(TC < TL)
    {
        digitalWrite(RLA1_PIN, HIGH);
        digitalWrite(RLA2_PIN, LOW);
        digitalWrite(LED_C_PIN, HIGH);
        digitalWrite(LED_H_PIN, LOW); 
    }

    if(TC >= T_max)
    {
        TC = T_max;
    }

    if(TC <= T_min)
    {
        TC = T_min;
    }
}


void check_alarms()
{
    if((TC - TH) >= TD)
    {
        digitalWrite(RLA1_PIN, HIGH);
        digitalWrite(RLA2_PIN, HIGH);
        digitalWrite(LED_C_PIN, HIGH);
        digitalWrite(LED_H_PIN, HIGH); 
        digitalWrite(LED_E1_PIN, LOW); 
        digitalWrite(BEEPER_PIN, LOW);
    }

    if((TL - TC) >= TD)
    {
        digitalWrite(RLA1_PIN, HIGH);
        digitalWrite(RLA2_PIN, HIGH);
        digitalWrite(LED_C_PIN, HIGH);
        digitalWrite(LED_H_PIN, HIGH);
        digitalWrite(LED_E2_PIN, LOW); 
        digitalWrite(BEEPER_PIN, LOW); 
    }

    if((digitalRead(INC_BUTTON_PIN) == false) && (digitalRead(DEC_BUTTON_PIN) == false))
    {
        digitalWrite(LED_E1_PIN, HIGH); 
        digitalWrite(LED_E2_PIN, HIGH);
        digitalWrite(RLA1_PIN, HIGH);
        digitalWrite(RLA2_PIN, HIGH);
        digitalWrite(LED_C_PIN, HIGH);
        digitalWrite(LED_H_PIN, HIGH); 
        digitalWrite(BEEPER_PIN, HIGH); 
    }
}


