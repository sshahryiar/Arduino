#include <SoftwareSerial.h>


#define redPin          9
#define bluePin         10
#define greenPin        11

#define BUFFER_SIZE     32


SoftwareSerial nextionSerial(7, 8); // RX, TX


char serialBuffer[BUFFER_SIZE];
byte bufferIndex = 0;


void setup(void) 
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(9600);
  Serial.flush();
  nextionSerial.begin(9600);
  nextionSerial.flush();

  Serial.println("RGB LED Controller");
  delay(100);
}


void loop(void) 
{
  if(nextionSerial.available()) 
  {
    char c = nextionSerial.read();
    
    if(c == 'S') 
    {
      bufferIndex = 0;
      serialBuffer[bufferIndex++] = c;
    } 
    else if(c == 'E') 
    {
      // End of message
      if(bufferIndex < BUFFER_SIZE - 1) 
      {
        serialBuffer[bufferIndex++] = c;
        serialBuffer[bufferIndex] = '\0'; // Null terminate
        
        // Parse the complete message
        parseMessage(serialBuffer);
      }
      bufferIndex = 0; // Reset buffer
    } 
    else if(bufferIndex > 0 && bufferIndex < BUFFER_SIZE - 1) 
    {
      // Continue building message
      serialBuffer[bufferIndex++] = c;
    }
  }
}


void parseMessage(char* message) 
{
  char* ptr;
  char* redStart; 
  char* blueStart;
  char* greenStart; 
  uint8_t red = 0;
  uint8_t blue = 0;
  uint8_t green = 0;

  // Expected format: "S124 249 356E"
  // Skip the 'S' at beginning
  ptr = (message + 1);
  
  // Extract red value (after '1')
  if (*ptr == '1') 
  {
    ptr++; // Skip channel indicator
  }

  redStart = ptr;

  while((*ptr != ' ') && (*ptr != '\0')) 
  {
    ptr++;
  }

  if(*ptr == ' ') 
  {
    *ptr = '\0'; // Temporarily terminate
    red = atoi(redStart);
    ptr++; // Move past space
    
    // Extract green value (after '2')
    if(*ptr == '2') 
    {
      ptr++; // Skip channel indicator
    }

    greenStart = ptr;

    while((*ptr != ' ') && (*ptr != '\0')) 
    {
      ptr++;
    }

    if(*ptr == ' ') 
    {
      *ptr = '\0';
      green = atoi(greenStart);
      ptr++; // Move past space
      
      // Extract blue value (after '3')
      if(*ptr == '3') 
      {
        ptr++; // Skip channel indicator
      }

      blueStart = ptr;
      
      while((*ptr != 'E') && (*ptr != '\0')) 
      {
        ptr++;
      }

      if(*ptr == 'E') 
      {
        *ptr = '\0';
        blue = atoi(blueStart);
        
        // Update LEDs
        updateLEDs(red, green, blue);
        
        Serial.print(" R:"); Serial.print(red);
        Serial.print(" G:"); Serial.print(green);
        Serial.print(" B:"); Serial.println(blue);
      }
    }
  }
}


void updateLEDs(uint8_t red, uint8_t green, uint8_t blue) 
{
  // Map values to PWM range
  int16_t redPWM = map(red, 0, 100, 0, 255);
  int16_t greenPWM = map(green, 0, 100, 0, 255);
  int16_t bluePWM = map(blue, 0, 100, 0, 255);

  redPWM = constrain(redPWM, 0, 255);
  greenPWM = constrain(greenPWM, 0, 255);
  bluePWM = constrain(bluePWM, 0, 255);


  analogWrite(redPin, redPWM);
  analogWrite(greenPin, greenPWM);
  analogWrite(bluePin, bluePWM);
}