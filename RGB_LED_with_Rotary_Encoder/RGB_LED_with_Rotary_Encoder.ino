#define encA        2
#define encB        3
#define btn         4

#define B           9
#define G          10
#define R          11
#define Step        1


boolean previous_A = 0;
boolean present_A = 0; 

unsigned char n = 0;
unsigned char s = 0;
unsigned char i = 0;
unsigned char d = 0;

unsigned char inputPins[3] = {encA, encB, btn};
unsigned char outputPins[3] = {R, G, B};


unsigned char getEncoder()
{
     present_A = digitalRead(encA);
     if((present_A) && (!previous_A))
     {
       if(digitalRead(encB))
       {
           i += Step;
       }
       else
       {
           i -= Step;
       }
     }
     previous_A = present_A;
     return i;
}


void glowSelected(unsigned char channel)
{
    unsigned char p = 0;
    unsigned char x = 0;
    unsigned char ch = 0;
    unsigned char glow[6] = {60, 120, 240, 120, 60, 0};
    ch = (channel - 1);
    for(p = 0; p < 4; p += 1)
    {
        for(x = 0; x < 6; x++)
        {
            analogWrite(outputPins[ch], glow[x]);
            delay(60);
        }
    }
}


void selectLEDandSet()
{
   if(digitalRead(btn) == LOW)
   {
       while(digitalRead(btn) == LOW); 
       s += 1;
       glowSelected(s);  
   } 
   if(s >= 4)
   {
       s = 0;
   }
   
   if(s != 0)
   {
       n = (s - 1);
       d = getEncoder();
       analogWrite(outputPins[n], d);
   }
}


void setup()
{
    for(s = 0; s <= 2; s += 1)
    {
        pinMode(inputPins[s], INPUT);
        digitalWrite(inputPins[s], HIGH);
        pinMode(outputPins[s], OUTPUT);
        digitalWrite(outputPins[s], HIGH);
        delay(400);
        digitalWrite(outputPins[s], LOW);    
    } 
    delay(10);
    s = 0;
}

void loop()
{
   selectLEDandSet();
}
