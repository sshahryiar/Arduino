#define Red_LED    9
#define Green_LED  10
#define Blue_LED   11


unsigned int s1 = 0;
unsigned int s2 = 0;


void setup() 
{
    for(s1 = 9; s1 <= 11; s1++)
    {
        pinMode(s1, OUTPUT);
        digitalWrite(s1, LOW);
    }
    
    Serial.begin(9600);
    Serial.flush();
}

void loop()
{
    unsigned char colour = 0;
  
    if(Serial.available() >= 2)
    {
      s1 = Serial.read();
      s2 = Serial.read();
      
      s2 <<= 8;
      s1 |= s2;
    }
    
    if((s1 >= 1000) && (s1 <= 1255))
    {
        colour = (s1 - 1000);
        analogWrite(Red_LED, colour);
    }
    
    if((s1 >= 2000) && (s1 <= 2255))
    {
        colour = (s1 - 2000);
        analogWrite(Green_LED, colour);
    }
    
    if((s1 >= 3000) && (s1 <= 3255))
    {
        colour = (s1 - 3000);
        analogWrite(Blue_LED, colour);
    }
}
