#define RLY_pin            2
#define SW_pin             3
#define ENCA_OUT_pin       5
#define ENCB_OUT_pin      15  
#define R_RST_pin          6
#define R_CLK_pin         14
#define MR_pin            17
#define SCK_pin           13  
#define SDO_pin           12
#define SDI_pin           11
#define LATCH_M_pin       10
#define LATCH_R_pin        9


const unsigned int m_LEDs[17] = {0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000};

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


unsigned char n = 0;
signed char m = 0;
signed char s = 0;

boolean invert = false;
boolean set_time = false;
boolean select_display = false;


ISR(TIMER1_OVF_vect)
{
    TCNT1H = 0xFF;
    TCNT1L = 0x00;
    
    write_bytes_74HC595(display_buffer[n], display_buffer[n + 7]); 
    update_matrix_display();
    digitalWrite(R_CLK_pin, LOW);
    digitalWrite(R_CLK_pin, HIGH);

    write_word_74HC595(m_LEDs[m]);
    update_encoder_display();
    
    n++;
    if(n > 6)
    {
        n = 0;
        digitalWrite(R_RST_pin, HIGH);
        digitalWrite(R_RST_pin, LOW);
    }
}


void setup() 
{
    noInterrupts(); 
    
    pinMode(SW_pin, INPUT);
    pinMode(ENCA_OUT_pin, INPUT);
    pinMode(ENCB_OUT_pin, INPUT);
    pinMode(RLY_pin, OUTPUT);
    pinMode(MR_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);
    pinMode(SDI_pin, OUTPUT);
    pinMode(LATCH_M_pin, OUTPUT);
    pinMode(LATCH_R_pin, OUTPUT);
    pinMode(R_RST_pin, OUTPUT);
    pinMode(R_CLK_pin, OUTPUT);
    pinMode(SDO_pin, INPUT_PULLUP);

    digitalWrite(MR_pin, HIGH);
    digitalWrite(RLY_pin, LOW);
    digitalWrite(SCK_pin, LOW);
    digitalWrite(SDI_pin, LOW);
    digitalWrite(R_RST_pin, LOW);
    digitalWrite(R_CLK_pin, LOW);
    digitalWrite(LATCH_M_pin, LOW);
    digitalWrite(LATCH_R_pin, LOW);
    
    TCCR1A = 0x00;
    TCCR1B = 0x03;
    TCNT1H = 0xFF;
    TCNT1L = 0x00;
    TIMSK1 = 0x01;
    
    interrupts();
    clear_buffer();
}

void loop() 
{ 
    make_number(s);
    
    if(digitalRead(SW_pin) == HIGH)
    {
        while(digitalRead(SW_pin) == HIGH);

        set_time ^= true;
        invert ^= true;
    }

    switch(set_time)
    {
        case 1:
        {
            s = get_encoder(0, 59); 
            digitalWrite(RLY_pin, LOW);           
            break;
        }
        
        default:
        {
            s--;

            if(s < 0)
            {
                if(m > 0)
                {
                  m--;
                  s = 59;
                }
                
                else
                {
                  s = 0;
                  m = 0;
                  digitalWrite(RLY_pin, HIGH);
                }
            }

            delay(1000);  
            break;
        }
    }
}


void clear_buffer()
{
    signed char pos = 0x0D;
    unsigned char value = 0x00;

    if(invert != false)
    {
        value = 0xFF;
    }
    
    else
    {
        value = 0x00;
    }

    while(pos > -1)
    {
        display_buffer[pos]= value;
        pos--;
    }
}


void write_74HC595(unsigned char value)
{                      
    unsigned char clk = 0x08; 
                                  
    while(clk > 0) 
    {                                                   
        digitalWrite(SCK_pin, LOW);  
        
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
          
        clk--;                              
    }                                   
}


void update_matrix_display()
{
    digitalWrite(LATCH_M_pin, HIGH);
    digitalWrite(LATCH_M_pin, LOW); 
}


void update_encoder_display()
{
    digitalWrite(LATCH_R_pin, HIGH);
    digitalWrite(LATCH_R_pin, LOW); 
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
}


void write_word_74HC595(unsigned int value)
{   
    unsigned char hb = 0;
    unsigned char lb = 0;

    lb = (value & 0x00FF);
    hb = ((value & 0xFF00)>> 8);

    write_bytes_74HC595(lb, hb);
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
    
    while(c >= 0)
    {
        display_buffer[c] = (numbers[(ch1 * 0x07) + c]);
        display_buffer[c + 0x07] = (numbers[(ch2 * 0x07) + c]);  
        c--;
    }
}


unsigned char get_encoder(signed char min_value, unsigned char max_value)
{
     static signed char i;
     
     static boolean ENCA; 
     static boolean ENCB;

     if((ENCA != digitalRead(ENCA_OUT_pin)) || (ENCB != digitalRead(ENCB_OUT_pin)))
     {                                                           
        if((ENCB | (digitalRead(ENCA_OUT_pin)) == HIGH))
        {                              
            i++;
        }
    
        if((ENCA | (digitalRead(ENCB_OUT_pin)) == HIGH))
        {                               
            i--;
        }
    
        ENCA = digitalRead(ENCA_OUT_pin);                                     
        ENCB = digitalRead(ENCB_OUT_pin);                                         
     } 

     if(i > max_value)
     {
          i = min_value; 
          m++;
          
          if(m > 16)
          {
            m = 16;
          }
     }
     if(i < min_value)
     {
          i = max_value; 
          m--;
          
          if(m <= 0)
          {
            m = 0;
          }
     }
     
     return ((unsigned char)i);
}
