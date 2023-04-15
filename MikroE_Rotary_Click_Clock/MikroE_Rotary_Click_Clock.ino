#define SW_pin           3
#define ENCA_OUT_pin     5
#define ENCB_OUT_pin    15

#define RST_pin         16
#define LATCH_pin       9
#define SCK_pin         13
#define SDO_pin         12
#define SDI_pin         11


boolean toggler = false;
boolean set_time = false;
boolean time_pause = true;

unsigned char s = 10;
unsigned char hr = 10;
unsigned char mn = 10;
unsigned char chg = 0;
unsigned char temp2 = 0;

signed long long present_millis = 0;
signed long long previous_millis = 0;


void setup() 
{
    pinMode(ENCA_OUT_pin, INPUT);
    pinMode(ENCB_OUT_pin, INPUT);
    pinMode(SW_pin, INPUT);
    
    pinMode(RST_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);
    pinMode(SDI_pin, OUTPUT);
    pinMode(LATCH_pin, OUTPUT);

    digitalWrite(RST_pin, HIGH);
    digitalWrite(SCK_pin, LOW);
    digitalWrite(SDI_pin, LOW);
    digitalWrite(LATCH_pin, LOW);
}

void loop() 
{
    settings();    
    time_keeper();
    show_time();
}


void update_display()
{
    digitalWrite(LATCH_pin, HIGH);
    delay(1);
    digitalWrite(LATCH_pin, LOW); 
}


void write_74HC595(unsigned char value)
{                      
    unsigned char clk = 0x00; 
                                  
    for(clk = 0; clk < 0x08; clk++) 
    {                                                   
        if((value & 0x80) != 0x00)
        {
            digitalWrite(SDI_pin, HIGH);
        }
        else
        {
            digitalWrite(SDI_pin, LOW);
        }
        
        digitalWrite(SCK_pin, HIGH);           
        value <<= 1;         
        digitalWrite(SCK_pin, LOW);                                  
    }                                   
}


void write_byte_74HC595(unsigned char byte_value)
{
    write_74HC595(byte_value);
    update_display();
}


void write_word_74HC595(unsigned char hb, unsigned char lb)
{   
    write_74HC595(hb);
    write_74HC595(lb);
    update_display();
}


unsigned char get_encoder(signed char min_value, unsigned char max_value)
{
     static signed char i;
     
     static boolean ENCA; 
     static boolean ENCB;

     if((ENCA != digitalRead(ENCA_OUT_pin)) || (ENCB != digitalRead(ENCB_OUT_pin)))
     {                                                           
        if(ENCB | (digitalRead(ENCA_OUT_pin) == HIGH))
        {                              
            i++;
        }
    
        if(ENCA | (digitalRead(ENCB_OUT_pin) == HIGH))
        {                               
            i--;
        }
    
        ENCA = digitalRead(ENCA_OUT_pin);                                     
        ENCB = digitalRead(ENCB_OUT_pin);                                         
     } 

     if(i > max_value)
     {
          i = min_value; 
     }
     if(i < min_value)
     {
          i = max_value; 
     }
     
     return ((unsigned char)i);
}


void settings()
{
    if(digitalRead(SW_pin) == HIGH)
    {
        write_word_74HC595(0xAA, 0xAA);
        delay(600);
        while(digitalRead(SW_pin) == HIGH);
        chg++;
    }

    switch(chg)
    {
        case 1:
        {
            mn = get_encoder(0, 59);
            break;
        }
        case 2:
        {
            hr = get_encoder(0, 23);
            break;
        }
        case 3:
        {
            chg = 0;
            break;
        }
    }
}


void time_keeper()
{
    present_millis = (millis() / 1000);
    
    if(((present_millis - previous_millis) == 1) || ((present_millis - previous_millis) == 1)) 
    {
        s++;
        toggler ^= true;
        previous_millis = present_millis; 
    }

    if(s > 59)
    {
        s = 0;
        mn++;
    }

    if(mn > 59)
    {
        mn = 0;
        hr++;
    }

    if(hr > 23)
    {
        hr = 0;
    }
}


void show_time()
{
    if(toggler == true)
    {
        temp2 = (hr | 0x40);
    }
    else
    {
        temp2 = (hr & 0xBF);
    }
    
    write_word_74HC595(temp2, mn); 
    delay(30); 
}

