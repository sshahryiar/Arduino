#include <IRremote.h>


boolean state0 = 0;
boolean state1 = 0;
boolean state2 = 0;
boolean state3 = 0;
boolean state4 = 0;
unsigned char s = 0;
unsigned char ledPin = 2;
unsigned char rcvPin = 10;
unsigned char relayPins[4] = {6, 7, 8, 9};


IRrecv irrecv(rcvPin);


decode_results rcv;


void allOff()
{
    for(s = 0; s < 4; s++)
    {
        digitalWrite(relayPins[s], LOW);
    }
}


void allOn()
{
    for(s = 0; s < 4; s++)
    {
        digitalWrite(relayPins[s], HIGH);
    }
}


void setup()
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    for(s = 0; s < 4; s++)
    {
        pinMode(relayPins[s], OUTPUT);
        digitalWrite(relayPins[s], LOW);
    }
    Serial.begin(9600);
    irrecv.enableIRIn(); 
}


void loop() 
{
    if (irrecv.decode(&rcv))
    {
          digitalWrite(ledPin, HIGH);
          Serial.println(rcv.value, DEC);
          irrecv.resume(); 
    }
    
    if(rcv.value == 16)
    {
         state1 = ~state1;
         digitalWrite(relayPins[0],  state1);    
    }
    if(rcv.value == 2064) 
    {  
         state2 = ~state2;
         digitalWrite(relayPins[1],  state2);   
    }
    if(rcv.value == 1040) 
    {  
         state3 = ~state3;
         digitalWrite(relayPins[2],  state3);    
    }
    if(rcv.value == 3088) 
    {  
         state4 = ~state4;
         digitalWrite(relayPins[3],  state4);    
    }
    if(rcv.value == 528) 
    {  
         allOff();
         digitalWrite(relayPins[0],  HIGH);
    }
    if(rcv.value == 2576) 
    {  
         allOff();
         digitalWrite(relayPins[1],  HIGH);
    }
    if(rcv.value == 1552) 
    {  
         allOff();
         digitalWrite(relayPins[2],  HIGH);
    }
    if(rcv.value == 3600) 
    {  
         allOff();
         digitalWrite(relayPins[3],  HIGH);
    }
    if(rcv.value == 272) 
    {  
         allOn();
    }
    if(rcv.value == 2320) 
    {  
         allOff();
    }
    if(rcv.value == 2704) 
    {  
         state0 ^= 0x1;
         switch(state0)
         {
             case 1:
             {
                 allOn();
                 break;
             }
             default:
             {
                 allOff();
                 break;
             }
         }
    }
    digitalWrite(ledPin, LOW);
}
