#include <Servo.h> 


#define number_of_servos    12
#define first_servo_pin_no   2


Servo servos[number_of_servos];


unsigned char angle[number_of_servos];

unsigned char s = 0;
unsigned char n = 0;


void setup()
{
   Serial.begin(19200);
   Serial.flush();
   for(s = 0; s < number_of_servos; s++)
   {
     servos[s].attach((s + first_servo_pin_no), 800, 2200);
     servos[s].write(90);
   }
   memset(angle, 0x00, sizeof(angle));
   s = 0;
}


void loop()
{
  if(Serial.available() > 0)
  {
    angle[s] = Serial.read();
    s++;
    
    if(s == number_of_servos)
    {
        for(n = 0; n < number_of_servos;n++)
        {
          servos[n].write(angle[n]); 
        }
        s = 0;
    }
  }
}
