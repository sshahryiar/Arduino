#define  data      13 
#define  clk       12 
#define  strobe    11   

#define  R         1
#define  G         2
#define  B         3


unsigned long Loops = 0;
unsigned char colour = R;
unsigned char s = 0;
unsigned char i = 0;
unsigned long x = 0; 
unsigned long n = 0x01;

const unsigned char text[] = {
  0x00, 0xFE, 0x40, 0x20, 0x40, 0xFE, 0x00, 0x00,   // M
  0x00, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x00, 0x00,   // I
  0x00, 0x7C, 0x82, 0x82, 0x82, 0x44, 0x00, 0x00,   // C
  0x00, 0xFE, 0x90, 0x90, 0x90, 0x6E, 0x00, 0x00,   // R
  0x00, 0x7C, 0x82, 0x82, 0x82, 0x7C, 0x00, 0x00,   // O
  0x00, 0x7E, 0x88, 0x88, 0x88, 0x7E, 0x00, 0x00,   // A
  0x00, 0xFE, 0x90, 0x90, 0x90, 0x6E, 0x00, 0x00,   // R
  0x00, 0xFE, 0x92, 0x92, 0x92, 0x82, 0x00, 0x00,   // E
  0x00, 0xFE, 0x20, 0x10, 0x08, 0xFE, 0x00, 0x00,   // N
  0x00, 0x7E, 0x88, 0x88, 0x88, 0x7E, 0x00, 0x00    // A
}; 


void writeData(unsigned long value)
{                      
    unsigned char clock = 24; 
    
    digitalWrite(strobe, LOW);                                 
    while(clock > 0) 
    {                                                   
        if(0x800000 & value)                     
        {
            digitalWrite(data, HIGH);     
        }
        else
        {
            digitalWrite(data, LOW);     
        }
        digitalWrite(clk, HIGH);                                                              
        value <<= 1;   
        digitalWrite(clk, LOW);  
        clock -= 1;
    }                                     
    digitalWrite(strobe, HIGH);        
}  


void writePort(unsigned char value)
{
  unsigned char c = 0;
  unsigned char mask = 0x80;
  
  for(c = 0; c < 8; c += 1)
  {
        if((mask & value) != 0)
        {
          digitalWrite((2 + c), HIGH);
        }
        else
        {
          digitalWrite((c + 2), LOW);
        }
        mask >>= 1;
  }
  
}


void setup()
{
    for(s = 2; s <= 13; s += 1)
    {
        pinMode(s, OUTPUT);
        digitalWrite(s, LOW);
    }
}


void loop()
{
  switch(colour)
  {
      case R:
      {
          n = 0x01;
          break;
      }
      case G:
      {
          n = 0x0100;
          break;
      }
      case B:
      {
          n = 0x010000;
          break;
      }
      default:
      {
         colour = R;
      }
  }
  
  for(i = 0; i < 72; i += 1)
  {
          for(Loops = 0; Loops < 20; Loops += 1)
          {
              for(s = i; s < (8 + i); s += 1)
              {
                  writePort(text[s]);
                  writeData(n);
                  
                  n <<= 1;
                  
                  switch(colour)
                  {
                      case R:
                      {
                          if(n > 0x80)
                          {
                            n = 0x01;
                          }
                          break;
                      }
                      case G:
                      {
                          if(n > 0x8000)
                          {
                            n = 0x0100;
                          }
                          break;
                      }
                      case B:
                      {
                          if(n > 0x800000)
                          {
                            n = 0x010000;
                          }
                          break;
                      }
                  }
              }
          }
  }
  
  colour += 1;
}
