#define R        6
#define G        9
#define B       10
#define W       11

#define Up       2
#define Down     3
#define Left     4
#define Right    5
#define BTN      7


boolean rState = 0;
boolean gState = 0;
boolean bState = 0;
boolean wState = 0;
boolean polarity = 0;

signed char pol = 1;

unsigned char rDuty = 120;
unsigned char gDuty = 120;
unsigned char bDuty = 120;
unsigned char wDuty = 120;


void setup()
{
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(W, OUTPUT);
    pinMode(Up, INPUT);
    pinMode(Down, INPUT);
    pinMode(Left, INPUT);
    pinMode(Right, INPUT);
    pinMode(BTN, INPUT);
    digitalWrite(R, LOW);
    digitalWrite(G, LOW);
    digitalWrite(B, LOW);
    digitalWrite(W, LOW);
    delay(100);
}

void loop()
{
    if(!(digitalRead(BTN)))
    {
        delay(40);
        polarity = ~polarity;
    }
    
    if(polarity)
    {
         pol = -5;
    }
    else
    {
         pol = 5;
    }
  
    if(rState != (digitalRead(Up)))
    {
        delay(40);
        rState = (digitalRead(Up));
        rDuty += pol;     
    } 
    
    if(gState != (digitalRead(Down)))
    {
        delay(40);
        gState = (digitalRead(Down));
        gDuty += pol;     
    }
 
    if(bState != (digitalRead(Left)))
    {
        delay(40);
        bState = (digitalRead(Left));
        bDuty += pol;     
    } 
    
    if(wState != (digitalRead(Right)))
    {
        delay(40);
        wState = (digitalRead(Right));
        wDuty += pol;     
    }  
   
   analogWrite(R, rDuty); 
   analogWrite(G, gDuty);
   analogWrite(B, bDuty);
   analogWrite(W, wDuty);
   delay(20);   
}
