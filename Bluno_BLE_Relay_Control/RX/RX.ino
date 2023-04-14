#define RL1        2
#define RL2        3
#define RL3        4
#define RL4        5
#define RL5        6
#define RL6        7
#define RL7        8
#define RL8        9


unsigned char s = 0;


void setup()
{
    Serial.begin(9600);
    Serial.flush();
    
    for(s = RL1; s <= RL8; s++)
    {
        pinMode(s, OUTPUT);
        digitalWrite(s, HIGH);
    }
}


void loop()
{
    if(Serial.available() > 0)
    {
        s = Serial.parseInt();
    }
    
    switch(s)
    {
       case 11:
       {
           digitalWrite(RL1, HIGH);
           break;
       } 
       case 22:
       {
           digitalWrite(RL2, HIGH);
           break;
       } 
       case 33:
       {
           digitalWrite(RL3, HIGH);
           break;
       } 
       case 44:
       {
           digitalWrite(RL4, HIGH);
           break;
       } 
       case 55:
       {
           digitalWrite(RL5, HIGH);
           break;
       } 
       case 66:
       {
           digitalWrite(RL6, HIGH);
           break;
       } 
       case 77:
       {
           digitalWrite(RL7, HIGH);
           break;
       } 
       case 88:
       {
           digitalWrite(RL8, HIGH);
           break;
       }
       case 1:
       {
           digitalWrite(RL1, LOW);
           break;
       } 
       case 2:
       {
           digitalWrite(RL2, LOW);
           break;
       } 
       case 3:
       {
           digitalWrite(RL3, LOW);
           break;
       } 
       case 4:
       {
           digitalWrite(RL4, LOW);
           break;
       } 
       case 5:
       {
           digitalWrite(RL5, LOW);
           break;
       } 
       case 6:
       {
           digitalWrite(RL6, LOW);
           break;
       } 
       case 7:
       {
           digitalWrite(RL7, LOW);
           break;
       } 
       case 8:
       {
           digitalWrite(RL8, LOW);
           break;
       } 
    }
}
