#include <EepromAT24C32.h>
#include <RtcDS1302.h>
#include <RtcDS1307.h>
#include <RtcDS3231.h>
#include <RtcDS3234.h>
#include <RtcDateTime.h>
#include <RtcTemperature.h>
#include <RtcUtility.h>
#include <ThreeWire.h>

#include <SPI.h>
#include <Wire.h>
#include "DHTesp.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ThingSpeak.h"


#define OLED_RESET -1


DHTesp dht; 

WiFiClient client;

//HTTPClient http;

#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);


Adafruit_BMP085 bmp;


unsigned long myChannelNumber = youchannelnumber;
const char *myWriteAPIKey = "yourkey";


float humidity = 0;
float pressure = 0;
float temperature = 0;


void setup() 
{
    Serial.begin(152000);

    WiFi.disconnect();
    WiFi.begin("ssid", "pass");
    while((!(WiFi.status() == WL_CONNECTED)))
    {
      delay(300);
    }

    bmp.begin();
    
    dht.setup(2, DHTesp::DHT11);
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48) 
    delay(100);
    display.clearDisplay();
    delay(100);
    
    ThingSpeak.begin(client);
}


void loop() 
{
    pressure = (bmp.readPressure() / 1000.0);

    delay(dht.getMinimumSamplingPeriod());

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();

    
    ThingSpeak.setField(1, humidity);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, pressure);

    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);


    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 20);
    display.print(temperature, 1);
    display.println("'C");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 30);
    display.print(pressure, 1);
    display.println("kPa");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 40);
    display.print(humidity, 1);
    display.println("%");

    display.display();

    delay(15000);
}
