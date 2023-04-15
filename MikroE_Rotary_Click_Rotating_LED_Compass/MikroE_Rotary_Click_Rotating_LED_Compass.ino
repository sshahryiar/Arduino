#include <SoftwareSerial.h>

SoftwareSerial HMC1022_port(18, 19); 


#define GND_pin                         2
#define MR_pin                         16
#define SCK_pin                        13  
#define MISO_pin                       12
#define MOSI_pin                       11
#define CS_pin                          9

#define Get_Angular_Measurement        49
#define no_of_data_bytes_returned       8
#define degrees_per_LED              23.9


const unsigned int LEDs[16] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000};


float H = 0.0;
unsigned char pos = 0;


void setup() 
{
    pinMode(GND_pin, OUTPUT);
    pinMode(MR_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);
    pinMode(MOSI_pin, OUTPUT);
    pinMode(MISO_pin, INPUT);
    pinMode(CS_pin, OUTPUT);
    
    digitalWrite(MR_pin, HIGH);
    digitalWrite(SCK_pin, LOW);
    digitalWrite(MOSI_pin, LOW);
    digitalWrite(CS_pin, LOW);
    digitalWrite(GND_pin, LOW);
    
    Serial.begin(9600);
    Serial.flush();
    
    HMC1022_port.begin(9600);
    HMC1022_port.flush();
}

void loop() 
{
    H = read_heading();
    
    if((H > -1) && (H < 360))
    {
        pos = (H / degrees_per_LED);
        write_word_74HC595(LEDs[pos]);
    }
    else
    {
        write_word_74HC595(0);
    }
    
    Serial.println(H);
    delay(1000);
}


float read_heading()
{                         
     unsigned char s = 0;  
     unsigned int CRC = 0x0000;
     register float heading = 0.0;
     
     unsigned char data_bytes[no_of_data_bytes_returned] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
                                                            
     HMC1022_port.write(Get_Angular_Measurement);  
               
     for(s = 0; s < no_of_data_bytes_returned; s++)
     {                
        data_bytes[s] = HMC1022_port.read();
        if(s < 7)   
        {
            CRC += data_bytes[s];
        }
     }                                                       
     
     CRC = (CRC & 0xFF); 
     
     if(CRC == data_bytes[7])
     {                                     
        for(s = 2; s < 7; s++)                        
        {                                       
            data_bytes[s] -= 0x30;               
        }
        
        heading = (data_bytes[2] * 100.0);
    	heading += (data_bytes[3] * 10.0);                       
    	heading += (data_bytes[4] * 1.0);                 
    	heading += (data_bytes[6] * 0.1);  
     }     
     
     else
     {
        heading = -1;
     }
                                              
     return heading;
} 


void write_74HC595(unsigned char value)
{                      
    unsigned char clk = 0x08; 
                                  
    while(clk > 0) 
    {                                                   
        digitalWrite(SCK_pin, LOW);  
        
        if((value & 0x80) != 0x00)
        {
            digitalWrite(MOSI_pin, HIGH);
        }
        else
        {
            digitalWrite(MOSI_pin, LOW);
        }
        
        digitalWrite(SCK_pin, HIGH);          
        value <<= 1;         
          
        clk--;                              
    }                                   
}


void update_display()
{
    digitalWrite(CS_pin, HIGH);
    digitalWrite(CS_pin, LOW); 
}


void write_word_74HC595(unsigned int value)
{   
    unsigned char hb = 0;
    unsigned char lb = 0;

    lb = (value & 0x00FF);
    hb = ((value & 0xFF00) >> 8);

    write_74HC595(lb);
    write_74HC595(hb);
    
    update_display();
}
