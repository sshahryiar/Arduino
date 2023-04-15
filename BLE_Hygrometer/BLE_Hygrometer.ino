#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


#define DHT22_pin                 2


boolean state = 1;


float tmp = 0.0;
float rh = 0.0;
float dp = 0.0;

byte symbol[8]  = 
{
    0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
};

unsigned char values[5]; 


void setup() 
{
    Serial.begin(115200);
    Serial.flush();
    
    lcd.begin(16, 2);
    lcd.clear(); 
    lcd.createChar(0, symbol);
    lcd.noBlink();

    DHT22_init();
}


void loop() 
{
    state = get_DHT22();
      
      switch(state)
      {
          case 0:
          {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("RH/%");
              lcd.setCursor(6, 0);
              lcd.print("T/ C");
              lcd.setCursor(8, 0);
              lcd.write(byte(0));
              lcd.setCursor(13, 0);
              lcd.print("DP");
              
              rh =  ((values[0] * 256.0 + values[1]) * 0.1); 
              lcd.setCursor(0, 1);
              lcd.print(rh, 1);
              
              tmp =  ((values[2] * 256.0 + values[3]) * 0.1); 
              lcd.setCursor(6, 1);
              lcd.print(tmp, 1);

              dp = ((log(rh/100)) + ((17.62 * tmp) / (243.12 + tmp)));
              dp = ((243.12 * dp) / (17.62 - dp));
              lcd.setCursor(12, 1);
              lcd.print(dp, 1); 
              
              Serial.println("S");
              Serial.println(rh, 1);
              Serial.println(tmp, 1);
              Serial.println(dp, 1);
              break;
          }
          
          default:
          {
              lcd.clear();
              lcd.setCursor(5, 0);
              lcd.print("Error!");
              Serial.println("E");
              break;
          }
      }

      delay(990);
}



void DHT22_init()
{
    pinMode(DHT22_pin, INPUT);
    delay(600);
}


unsigned char get_DHT22()
{
    unsigned char s = 0;
    unsigned char checksum = 0;

    pinMode(DHT22_pin, OUTPUT);
    digitalWrite(DHT22_pin, HIGH);
    digitalWrite(DHT22_pin, LOW);
    delay(1);
    
    digitalWrite(DHT22_pin, HIGH);
    delayMicroseconds(32);
    
    digitalWrite(DHT22_pin, LOW);
    pinMode(DHT22_pin, INPUT);  
    delayMicroseconds(2);
    
    if(digitalRead(DHT22_pin) == HIGH)
    {
        return 1;
    }
    delayMicroseconds(80);
    
    if(digitalRead(DHT22_pin) == LOW)
    {
        return 1;
    }
    delayMicroseconds(80);
    
    for(s = 0; s <= 4; s++)
    {
        values[s] = read_DHT22();
    }
    
    pinMode(DHT22_pin, OUTPUT);
    digitalWrite(DHT22_pin, HIGH);
    
    for(s = 0; s < 4; s++)
    {
        checksum += values[s];
    }
    
    if(checksum != values[4])
    {
       return 2;
    }
    else
    {
       return 0;
    }
}


unsigned char read_DHT22()
{
   unsigned char s = 0;
   unsigned char val = 0;

   unsigned int timeout = 0;
   
   pinMode(DHT22_pin, INPUT);  
   
   for(s = 0; s < 8; s++)
   {
      val <<= 1;
      
      timeout = 0;
      while((digitalRead(DHT22_pin) == LOW) && (timeout <= 60000))
      {
          timeout++;
      }
      delayMicroseconds(32);
      
      if(digitalRead(DHT22_pin) == HIGH)
      {
          val |= 1;
      }
      
      timeout = 0;
      while((digitalRead(DHT22_pin) == HIGH) && (timeout <= 60000))
      {
          timeout++;
      }
   }
   return val;
}
