#include <LiquidCrystal.h>


#define DHT11_pin    P1_7


LiquidCrystal lcd(P1_4, P1_5, P2_0, P2_1, P2_2, P2_3);


boolean state = 1;

byte symbol[8]  = 
{
      0x07, 0x05, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00
};

unsigned char values[5]; 


void DHT11_init();
unsigned char get_byte();
unsigned char get_data();     


void setup()
{
     pinMode(RED_LED, OUTPUT);
     pinMode(GREEN_LED, OUTPUT);
     digitalWrite(RED_LED, LOW);
     digitalWrite(GREEN_LED, LOW);
     Serial.begin(9600);
     Serial.flush();
     lcd.begin(16, 2);
     lcd.clear(); 
     lcd.createChar(0, symbol);
     lcd.noBlink(); 
}

void loop()
{
      state = get_data();
      
      switch(state)
      {
          case 1:
          {
              lcd.clear();
              lcd.setCursor(5, 0);
              lcd.print("Error!");
              Serial.println("E");
          }
          default:
          {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("RH/%:");
              lcd.setCursor(0, 1);
              lcd.print("T/ C:");
              lcd.setCursor(2, 1);
              lcd.write(byte(0));
              
              lcd.setCursor(6, 0);
              lcd.print(values[0]);
              lcd.setCursor(8, 0);
              lcd.print(".");
              lcd.setCursor(9, 0);
              lcd.print(values[1]);
              
              lcd.setCursor(6, 1);
              lcd.print(values[2]);
              lcd.print(".");
              lcd.setCursor(9, 1);
              lcd.print(values[3]);
              
              Serial.println("S");
              Serial.print(values[0]);
              Serial.print(".");
               Serial.println(values[1]);
              Serial.print(values[2]);
              Serial.print(".");
               Serial.println(values[3]);
          }
      }
      
      digitalWrite(GREEN_LED, HIGH);
      delay(90);
      digitalWrite(GREEN_LED, LOW);
      delay(900);
}


void DHT11_init()
{
    pinMode(DHT11_pin, INPUT);
   delay(1000);
}


unsigned char get_byte()
{
   unsigned char s = 0;                                                                      
   unsigned char value = 0;
   
   pinMode(DHT11_pin, INPUT);
   digitalWrite(RED_LED, HIGH);

   for(s = 0; s < 8; s += 1) 
   {
      value <<= 1;
      while(digitalRead(DHT11_pin) == LOW);
      delayMicroseconds(30);
      if(digitalRead(DHT11_pin) == HIGH)
      {
          value |= 1;
      }
      while(digitalRead(DHT11_pin) == HIGH);
   }
   
   digitalWrite(RED_LED, LOW);
   return value;
}


unsigned char get_data()
{
   boolean chk = 0;
   unsigned char s = 0;
   unsigned char check_sum = 0;

   pinMode(DHT11_pin, OUTPUT);
   digitalWrite(DHT11_pin, HIGH);
   digitalWrite(DHT11_pin, LOW);
   delay(18);
   digitalWrite(DHT11_pin, HIGH);
   delayMicroseconds(26);
   
   pinMode(DHT11_pin, INPUT);
   chk = digitalRead(DHT11_pin);
   if(chk == true)
   {
      return 1;
   }
   delayMicroseconds(80);
   
   chk = digitalRead(DHT11_pin);
   if(chk == false)
   {
      return 1;
   }
   delayMicroseconds(80);

   for(s = 0; s <= 4; s += 1)
   {
       values[s] = get_byte();
   }
   
   pinMode(DHT11_pin, OUTPUT);
   digitalWrite(DHT11_pin, HIGH);
   
   for(s = 0; s < 4; s++)
   {
       check_sum += values[s];
   }
   
   if(check_sum != values[4])
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

