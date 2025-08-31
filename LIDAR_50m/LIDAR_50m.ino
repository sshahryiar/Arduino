#include <SoftwareSerial.h>
#include "lcd_keypad.h"


lcd_kbd io;
SoftwareSerial lidarSerial(11, 12); // RX, TX


const int BUFFER_SIZE = 32;
char lidarBuffer[BUFFER_SIZE];
uint8_t bufferIndex = 0;
char c;


void setup(void) 
{
  io.begin();
  io.clear();

  Serial.begin(9600);
  lidarSerial.begin(19200); 
}

void loop(void) 
{
  if(io.get_key() == up_key)
  {
    lidarSerial.print("D");
  }
  if(io.get_key() == down_key)
  {
    lidarSerial.print("S");
  }

  
  while(lidarSerial.available()) 
  {
    c = lidarSerial.read();

    if(bufferIndex < BUFFER_SIZE - 1) 
    {
      lidarBuffer[bufferIndex++] = c;
      
      if(c == '\n') 
      {
        lidarBuffer[bufferIndex] = '\0'; 
        parseLidarData(lidarBuffer);
        bufferIndex = 0; 
      }
    } 
    else 
    {
      bufferIndex = 0;
    }
  }

  delay(1000);
}


void parseLidarData(const char* data)
{
  const char *start;
  const char *chr;
  const char *schr;
  const char *comma;

  char buffer[40];
  char floatStr[20];
  char tmpStr_1[10] = {0};
  char tmpStr_2[10] = {0};

  int tmp1 = 0;
  float tmp2 = 0;
  float tmp3 = 0;

  if(strncmp(data, "D:", 2) == 0) 
  {
    start = strchr(data, ':');
    chr = strchr(data, 'm');
    comma = strchr(data, ',');

    if((start && chr && comma) && (chr > start) && (comma > chr)) 
    {
      strncpy(tmpStr_1, (comma + 1), 5); 
      strncpy(tmpStr_2, (start + 2), (chr - start - 2));  
                    
      tmp1 = atoi(tmpStr_1);
      tmp2 = atof(tmpStr_2);

      Serial.print("Distance (m): ");
      Serial.println(tmp2, 3);
      Serial.print("Signal Info: ");
      Serial.println(tmp1);
      strcat(tmpStr_2, " m ");

      io.text(0, 0, tmpStr_2);
      io.text(10, 0, "S:");
      io.text(12, 0, tmpStr_1);

    } 
    else 
    {
      Serial.println("Parsing failed (structure mismatch).");
    }
  }

  else if(strncmp(data, "S:", 2) == 0) 
  {
    start = strchr(data, ':');
    chr = strchr(data, '`'); 
    comma = strchr(data, ',');
    schr = strchr(data, 'V');    

    if(start && chr && comma && schr && 
      (chr > start) && (comma > chr) && (schr > comma)) 
    {
        chr = '\0'; 
        tmp2 = atof(start + 1);
        chr = '°'; 

        schr = '\0'; 
        tmp3 = atof(comma + 1);
        schr = 'V'; 

        Serial.print("Temp (°C): ");
        Serial.println(tmp2, 2);
        Serial.print("BV (V): ");
        Serial.println(tmp3, 2);

        dtostrf(tmp2, 2, 2, floatStr);
        sprintf(buffer, "%s", floatStr);
        strcat(floatStr, "'C ");
        io.text(0, 1, floatStr);

        dtostrf(tmp3, 1, 2, floatStr);
        sprintf(buffer, "%s", floatStr);
        strcat(floatStr, " V");
        io.text(10, 1, floatStr);
    }  
    else 
    {
        Serial.println("Parsing failed (structure mismatch).");
    }
  }

  else if(strncmp(data, "O,OK!", 5) == 0) 
  {
    Serial.print("Status: ");
    Serial.println(data + 2);
  }

  else if(strncmp(data, "C,OK!", 5) == 0) 
  {
    Serial.print("Status: ");
    Serial.println(data + 2);
  }

  else
  {
    Serial.println("Error! Unknown Code!");
  }
}
