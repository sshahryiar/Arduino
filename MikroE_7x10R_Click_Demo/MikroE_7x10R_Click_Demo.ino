#define R_RST_pin          5
#define R_CLK_pin         15
#define MR_pin            16
#define LATCH_pin          9
#define SCK_pin           13  
#define SDO_pin           12
#define SDI_pin           11
#define button_pin        10
#define LM35_pin          A4

#define no_of_smaples     32
#define max_temp          99
#define min_temp           0


boolean invert = false;

unsigned char i = 0;
unsigned char s = 0;
unsigned char ms = 0;

const unsigned char numbers[70] = 
{
  0x0E, 0x0A, 0x0A, 0x0A, 0x0E, 0x00, 0x00,   //0
  0x06, 0x04, 0x04, 0x04, 0x0E, 0x00, 0x00,   //1
  0x0E, 0x08, 0x0E, 0x02, 0x0E, 0x00, 0x00,   //2
  0x0E, 0x08, 0x0C, 0x08, 0x0E, 0x00, 0x00,   //3
  0x0A, 0x0A, 0x0E, 0x08, 0x08, 0x00, 0x00,   //4
  0x0E, 0x02, 0x0E, 0x08, 0x0E, 0x00, 0x00,   //5
  0x0E, 0x02, 0x0E, 0x0A, 0x0E, 0x00, 0x00,   //6
  0x0E, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,   //7
  0x0E, 0x0A, 0x0E, 0x0A, 0x0E, 0x00, 0x00,   //8
  0x0E, 0x0A, 0x0E, 0x08, 0x0E, 0x00, 0x00    //9
};

unsigned char display_buffer[14];

unsigned int tmp = 0;

unsigned long cnt_previous = 0;
unsigned long cnt_present = 0;


void setup() 
{
    pinMode(MR_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);
    pinMode(SDI_pin, OUTPUT);
    pinMode(LATCH_pin, OUTPUT);
    pinMode(R_RST_pin, OUTPUT);
    pinMode(R_CLK_pin, OUTPUT);
    pinMode(SDO_pin, INPUT_PULLUP);
    pinMode(button_pin, INPUT_PULLUP);

    digitalWrite(MR_pin, HIGH);
    digitalWrite(SCK_pin, LOW);
    digitalWrite(SDI_pin, LOW);
    digitalWrite(R_RST_pin, LOW);
    digitalWrite(R_CLK_pin, LOW);
    digitalWrite(LATCH_pin, LOW);
    
    clear_buffer();
}

void loop() 
{
    cnt_present = millis();
    
    if((cnt_present - cnt_previous) >= 1)
    {
        digitalWrite(R_CLK_pin, LOW);
        write_bytes_74HC595(display_buffer[i], display_buffer[i + 7]); 
        digitalWrite(R_CLK_pin, HIGH);
        
        i++;
        ms++;
        if(i > 6)
        {
            i = 0;
            digitalWrite(R_RST_pin, HIGH);
            digitalWrite(R_RST_pin, LOW);
        }
        
        cnt_previous = cnt_present;
    }

    if(ms > 99)
    {
        tmp += analogRead(LM35_pin);
        s++;
        ms = 0;
        if(s > no_of_smaples)
        {
            tmp >>= 6;

            if(tmp >= max_temp)
            {
                tmp = max_temp;
            }
            if(tmp <= min_temp)
            {
                tmp = min_temp;
            }
                          
            make_number(tmp);
            tmp = 0;
            s = 0; 
        }

        if(digitalRead(button_pin) == false)
        {
            invert ^= true;
        }
    }
}


void clear_buffer()
{
    signed char pos = 0x0D;
    unsigned char value = 0x00;

    if(invert != false)
    {
        value = 0x00;
    }
    else
    {
        value = 0xFF;
    }

    while(pos > -1)
    {
        display_buffer[pos]= value;
        pos--;
    }
}


void update_display()
{
    digitalWrite(LATCH_pin, HIGH);
    digitalWrite(LATCH_pin, LOW); 
}


void write_74HC595(unsigned char value)
{                      
    unsigned char clk = 0x08; 
                                  
    while(clk > 0) 
    {                                                   
        digitalWrite(SCK_pin, LOW);  
        
        if((value & 0x80) != 0x00)
        {
            digitalWrite(SDI_pin, LOW);
        }
        else
        {
            digitalWrite(SDI_pin, HIGH);
        }
        
        digitalWrite(SCK_pin, HIGH);          
        value <<= 1;         
          
        clk--;                              
    }                                   
}


void write_bytes_74HC595(unsigned char hb, unsigned char lb)
{   
    if(invert != false)
    {
        hb = ~hb;
        lb = ~lb;
    }
    
    write_74HC595(lb);
    write_74HC595(hb);
    
    update_display();
}


void make_number(unsigned char value)
{
    signed char c = 0x06;
    unsigned char ch1 = 0x00;
    unsigned char ch2 = 0x00;
    
    unsigned int ch_temp = 0x0000;

    clear_buffer();

    ch1 = (value / 10);
    ch2 = (value % 10);

    while(c > -1)
    {
        display_buffer[c] = (numbers[(ch1 * 0x07) + c]);
        display_buffer[c + 0x07] = (numbers[(ch2 * 0x07) + c]);  
        c--;
    }
}

