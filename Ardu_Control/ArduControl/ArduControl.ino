/*
Coder: Shawon Shahryiar
https://www.facebook.com/groups/ArduinoBangla/
https://www.facebook.com/groups/microarena/
https://www.facebook.com/MicroArena
*/


unsigned char s = 0;
unsigned char temp[3];
unsigned int adc[6];


void resetFunc()
{
  asm volatile ("jmp 0");
}


void setup()
{
    for(s = 2; s <= 13; s++)
    {
        pinMode(s, OUTPUT);
        digitalWrite(s, LOW);
    }
    Serial.begin(115200);
    Serial.flush();
}


void loop()
{
      if(Serial.available() > 0)
      {
        temp[0] = Serial.read();
        temp[1] = Serial.read();
        temp[2] = Serial.read();
      }
      
      if((temp[0] == 'U') && (temp[1] == 'u'))
      {
          analogWrite(3, (temp[2]));
      }
      if((temp[0] == 'V') && (temp[1] == 'v'))
      {
          analogWrite(5, (temp[2]));
      }
      if((temp[0] == 'W') && (temp[1] == 'w'))
      {
          analogWrite(6, (temp[2]));
      }
      if((temp[0] == 'X') && (temp[1] == 'x'))
      {
          analogWrite(9, (temp[2]));
      }
      if((temp[0] == 'Y') && (temp[1] == 'y'))
      {
          analogWrite(10, (temp[2]));
      }
      if((temp[0] == 'Z') && (temp[1] == 'z'))
      {
        analogWrite(11, (temp[2]));
      }
      
      if((temp[0] == 'A') && (temp[1] == 'A') && (temp[2] == 'A'))
      {
        digitalWrite(2, HIGH);
      }
      if((temp[0] == 'a') && (temp[1] == 'a') && (temp[2] == 'a'))
      {
        digitalWrite(2, LOW);
      }
      if((temp[0] == 'B') && (temp[1] == 'B') && (temp[2] == 'B'))
      {
        digitalWrite(3, HIGH);
      }
      if((temp[0] == 'b') && (temp[1] == 'b') && (temp[2] == 'b'))
      {
        digitalWrite(3, LOW);
      }
      if((temp[0] == 'C') && (temp[1] == 'C') && (temp[2] == 'C'))
      {
        digitalWrite(4, HIGH);
      }
      if((temp[0] == 'c') && (temp[1] == 'c') && (temp[2] == 'c'))
      {
        digitalWrite(4, LOW);
      }
      if((temp[0] == 'D') && (temp[1] == 'D') && (temp[2] == 'D'))
      {
        digitalWrite(5, HIGH);
      }
      if((temp[0] == 'd') && (temp[1] == 'd') && (temp[2] == 'd'))
      {
        digitalWrite(5, LOW);
      }
      if((temp[0] == 'E') && (temp[1] == 'E') && (temp[2] == 'E'))
      {
        digitalWrite(6, HIGH);
      }
      if((temp[0] == 'e') && (temp[1] == 'e') && (temp[2] == 'e'))
      {
        digitalWrite(6, LOW);
      }
      if((temp[0] == 'F') && (temp[1] == 'F') && (temp[2] == 'F'))
      {
        digitalWrite(7, HIGH);
      }
      if((temp[0] == 'f') && (temp[1] == 'f') && (temp[2] == 'f'))
      {
        digitalWrite(7, LOW);
      }
      if((temp[0] == 'G') && (temp[1] == 'G') && (temp[2] == 'G'))
      {
        digitalWrite(8, HIGH);
      }
      if((temp[0] == 'g') && (temp[1] == 'g') && (temp[2] == 'g'))
      {
        digitalWrite(8, LOW);
      }
      if((temp[0] == 'H') && (temp[1] == 'H') && (temp[2] == 'H'))
      {
        digitalWrite(9, HIGH);
      }
      if((temp[0] == 'h') && (temp[1] == 'h') && (temp[2] == 'h'))
      {
        digitalWrite(9, LOW);
      }
      if((temp[0] == 'I') && (temp[1] == 'I') && (temp[2] == 'I'))
      {
        digitalWrite(10, HIGH);
      }
      if((temp[0] == 'i') && (temp[1] == 'i') && (temp[2] == 'i'))
      {
        digitalWrite(10, LOW);
      }
      if((temp[0] == 'J') && (temp[1] == 'J') && (temp[2] == 'J'))
      {
        digitalWrite(11, HIGH);
      }
      if((temp[0] == 'j') && (temp[1] == 'j') && (temp[2] == 'j'))
      {
        digitalWrite(11, LOW);
      }
      if((temp[0] == 'K') && (temp[1] == 'K') && (temp[2] == 'K'))
      {
        digitalWrite(12, HIGH);
      }
      if((temp[0] == 'k') && (temp[1] == 'k') && (temp[2] == 'k'))
      {
        digitalWrite(12, LOW);
      }
      if((temp[0] == 'L') && (temp[1] == 'L') && (temp[2] == 'L'))
      {
        digitalWrite(13, HIGH);
      }
      if((temp[0] == 'l') && (temp[1] == 'l') && (temp[2] == 'l'))
      {
        digitalWrite(13, LOW);
      }
      
      if((temp[0] == 'R') && (temp[1] == 'R') && (temp[2] == 'R'))
      {
        Serial.flush();
        resetFunc(); 
      }
      
      Serial.println("S");
      for(s = 0; s < 6; s++)
      {
         adc[s] = analogRead(s);
         Serial.println(adc[s]);
      }
      
      delay(100);
}
