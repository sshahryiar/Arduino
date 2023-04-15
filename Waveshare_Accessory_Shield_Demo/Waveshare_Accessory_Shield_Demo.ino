#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ChainableLED.h>
#include <ADXL345.h>
#include "ds3231.h"
#include <inttypes.h>
#include <lm75.h>


//Definitions for SSD1306 128x64 OLED Display

#define OLED_RESET                            7
#define OLED_SA0                              8

//Definitions for P9813 RGB LED Driver

#define no_of_LEDs                            1

#define data_pin                              5
#define clk_pin                               6

#define rgb_pwr                               1

//Definitions for Joystick

#define KEY_UP                                A1
#define KEY_DOWN                              A5
#define KEY_LEFT                              A3
#define KEY_RIGHT                             A2
#define KEY_ENTER                             A4

//Clock Stuffs
  
#define CLOCK_CENTER_X                       24  
#define CLOCK_CENTER_Y                       40 
#define H_LENGTH                             10  
#define M_LENGTH                             14  
#define S_LENGTH                             18 

#define buzzer                               11 

#define rad_per_degree                0.0174556


Adafruit_SSD1306 oled(OLED_RESET, OLED_SA0);
ChainableLED RGB_LED(clk_pin, data_pin, rgb_pwr, no_of_LEDs);
TempI2C_LM75 thermo = TempI2C_LM75(0x48, TempI2C_LM75::nine_bits);
ADXL345 accelerometer;


boolean tgl = false;
boolean settings = false;

signed char parameter = 0;
unsigned char max_b = 127;

float t_p = 0.0;
float t_o = -255.0;


struct ts T;


void draw_background()
{
    unsigned char i = 0;
  
    oled.drawRect(2, 18, 44, 44, WHITE);
    oled.drawLine(24, 19, 24, 20, WHITE);
    oled.drawLine(24, 60, 24, 61, WHITE);
    oled.drawLine(3, 40, 4, 40, WHITE);
    oled.drawLine(44, 40, 45, 40, WHITE);
   
    oled.fillCircle(106, 58, 4, WHITE);
    oled.drawFastVLine(104, 18, 37, WHITE);
    oled.drawFastVLine(108, 18, 37, WHITE);
    oled.drawFastHLine(105, 17, 3, WHITE);
    
    for(i = 18; i <= 53; i += 5)
    {
        oled.drawFastHLine(111, i, 3, WHITE);
    }
    
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    for(i = 0; i < 40; i += 10)
    {
        oled.setCursor(116, (48 - i));
        oled.println(i + 10);
    }
    
    oled.setCursor(12, 0);
    oled.println("Time");
    oled.setCursor(59, 0);
    oled.println("Date");
    oled.setCursor(96, 0);
    oled.println("Temp.");
    
    oled.display();
}


float get_T_avg()
{
    unsigned char samples = 20;  
    float avg = 0.0;
    
    while(samples > 0)
    {
        avg += thermo.getTemp();
        delayMicroseconds(100);
        samples--;
    }
    
    avg /= 20.0;
    return avg;
}


void show_temperature()
{
    unsigned char bar_length = 0;
  
    t_p = get_T_avg();
    
    if(t_p != t_o)
    {
        oled.fillRect(98, 8, 29, 8, BLACK);
        oled.setTextColor(WHITE);
        oled.setCursor(98, 9);
        oled.println(t_p);
    
        t_p = constrain(t_p, 0, 40.0);
        bar_length = map(t_p, 0.0, 40.0, 0, 36);
        oled.drawLine(106, 53, 106, 18, BLACK);  
        oled.drawLine(106, 53, 106, (53 - bar_length), WHITE);  
        t_o = t_p;
    }
}


void show_time()
{
    static unsigned long previous_time;
    static unsigned long present_time;
  
    oled.fillRect(0, 8, 50, 8, BLACK);
    
    if(T.hour < 10)
    {
        oled.setCursor(0, 9);
        oled.println("0");
        oled.setCursor(6, 9);
        oled.println(T.hour);
    }
    else
    {
        oled.setCursor(0, 9);
        oled.println(T.hour);
    }
    
    if(T.min < 10)
    {
        oled.setCursor(18, 9);
        oled.println("0");
        oled.setCursor(24, 9);
        oled.println(T.min);
    }
    else
    {
        oled.setCursor(18, 9);
        oled.println(T.min);
    }
    
    if(T.sec < 10)
    {
        oled.setCursor(36, 9);
        oled.println("0");
        oled.setCursor(42, 9);
        oled.println(T.sec);
    }
    else
    {
        oled.setCursor(36, 9);
        oled.println(T.sec);
    }
    
    present_time = millis();
    if((present_time - previous_time) > 999)
    {
        tgl ^= 1;
        previous_time = present_time;
    }
    
    if(tgl)
    {
        oled.setCursor(12, 9);
        oled.println(":");
        oled.setCursor(30, 9);
        oled.println(":");
    }
    else
    {
        oled.setCursor(12, 9);
        oled.println(" ");
        oled.setCursor(30, 9);
        oled.println(" ");
    }
    
    display_analog_clock(T.hour, T.min, T.sec);
}


void show_date()
{
    oled.fillRect(48, 16, 54, 37, BLACK);
    
    if(T.mday < 10)
    {
        oled.setCursor(60, 18);
        oled.println("0");
        oled.setCursor(66, 18);
        oled.println(T.mday);
    }
    else
    {
        oled.setCursor(60, 18);
        oled.println(T.mday);
    }
    
    oled.setCursor(72, 18);
    oled.println("/");
    
    if(T.mon < 10)
    {
        oled.setCursor(78, 18);
        oled.println("0");
        oled.setCursor(84, 18);
        oled.println(T.mon);
    }
    else
    {
        oled.setCursor(78, 18);
        oled.println(T.mon);
    }
    
    oled.setCursor(62, 32);
    oled.println(T.year);
    
    oled.setCursor(49, 46);
    switch(T.wday)
    {
        case 1:
        {
            oled.println(" Monday  ");
            break;
        }
        case 2:
        {
            oled.println(" Tuesday ");
            break;
        }
        case 3:
        {
            oled.println("Wednesday");
            break;
        }
        case 4:
        {
            oled.println("Thursday ");
            break;
        }
        case 5:
        {
            oled.println(" Friday  ");
            break;
        }
        case 6:
        {
            oled.println("Saturday ");
            break;
        }
        default:
        {
            oled.println(" Sunday  ");
            break;
        }
    }
}


void set_time_and_date()
{
    if(digitalRead(KEY_ENTER) == LOW)
    {
        digitalWrite(buzzer, HIGH);
        delay(20);
        digitalWrite(buzzer, LOW);
        while(digitalRead(KEY_ENTER) == LOW);
        settings = true;
        parameter = 1;
    }
    
    if(settings == true)
    {
        switch(parameter)
        {
            case 1:
            {
                RGB_LED.setColorRGB(0, 255, 0, 0);
                T.hour = inc_dec(T.hour, 23, 0);
                break;
            }
            case 2:
            {
                RGB_LED.setColorRGB(0, 0, 255, 0);
                T.min = inc_dec(T.min, 59, 0);
                break;
            }
            case 3:
            {
                RGB_LED.setColorRGB(0, 0, 0, 255);
                T.sec = inc_dec(T.sec, 59, 0);
                break;
            }
            case 4:
            {
                RGB_LED.setColorRGB(0, 127, 127, 0);
                T.mday = inc_dec(T.mday, 31, 1);
                break;
            }
            case 5:
            {
                RGB_LED.setColorRGB(0, 0, 127, 127);
                T.mon = inc_dec(T.mon, 12, 1);
                break;
            }
            case 6:
            {
                RGB_LED.setColorRGB(0, 127, 0, 127);
                T.year = inc_dec(T.year, 2100, 1980);
                break;
            }
            case 7:
            {
                RGB_LED.setColorRGB(0, 127, 127, 127);
                T.wday = inc_dec(T.wday, 6, 0);
                break;
            }
            default:
            {
                DS3231_set(T);
                settings = false;
                break;
            }
        }
    }
}


void clock_stuffs()
{
    if(settings == false)
    {
        DS3231_get(&T);
    }
    show_time();
    show_date();
    set_time_and_date();
}


void display_analog_clock(signed int h, signed int m, signed int s)
{
    float midHours = 0;
    static signed char hx;
    static signed char hy;
    static signed char mx;
    static signed char my;
    static signed char sx;
    static signed char sy;
  
    h -= 3;
    m -= 15;
    s -= 15;
    
    if(h <= 0)
    {
        h += 12;
    }
    if(m < 0)
    {
        m += 60;
    }
    if(s < 0)
    {
        s += 60;
    }
    
    oled.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, (CLOCK_CENTER_X + sx), (CLOCK_CENTER_Y + sy), BLACK); 
    oled.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, (CLOCK_CENTER_X + mx), (CLOCK_CENTER_Y + my), BLACK);
    oled.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, (CLOCK_CENTER_X + hx), (CLOCK_CENTER_Y + hy), BLACK); 
  
    s *= 6;  
    sx = (S_LENGTH * cos(s * rad_per_degree));  
    sy = (S_LENGTH * sin(s * rad_per_degree)); 
    oled.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, (CLOCK_CENTER_X + sx), (CLOCK_CENTER_Y + sy), WHITE);  
    
    m *= 6; 
    mx = (M_LENGTH * cos(m * rad_per_degree));  
    my = (M_LENGTH * sin(m * rad_per_degree)); 
    oled.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, (CLOCK_CENTER_X + mx), (CLOCK_CENTER_Y + my), WHITE);  
    
    midHours = (T.min / 12);  
    h *= 5;  
    h += midHours;  
    h *= 6; 
    hx = (H_LENGTH * cos(h * rad_per_degree));  
    hy = (H_LENGTH * sin(h * rad_per_degree)); 
    oled.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, (CLOCK_CENTER_X + hx), (CLOCK_CENTER_Y + hy), WHITE);  
}


void accelerometer_and_RGB_LED()
{
    unsigned char r = 0x00;
    unsigned char g = 0x00;
    unsigned char b = 0x00;
  
    Vector norm = accelerometer.readNormalize();
    r = map(norm.XAxis, -11, 11, 0, max_b);
    g = map(norm.YAxis, -11, 11, 0, max_b);
    b = map(norm.ZAxis, -11, 11, 0, max_b);
    
    if(settings == false)
    {
      RGB_LED.setColorRGB(0, r, g, b);
    }
}


signed int inc_dec(signed int value, signed int max_value, signed int min_value)
{
    if(digitalRead(KEY_UP) == LOW)
    {
        digitalWrite(buzzer, HIGH);
        delay(20);
        digitalWrite(buzzer, LOW);
        value++;
    }
    if(value > max_value)
    {
        value = min_value;
    }
    
    if(digitalRead(KEY_DOWN) == LOW)
    {
        digitalWrite(buzzer, HIGH);
        delay(20);
        digitalWrite(buzzer, LOW);
        value--;
    }
    if(value < min_value)
    {
        value = max_value;
    }
    
    if(digitalRead(KEY_RIGHT) == LOW)
    {
        digitalWrite(buzzer, HIGH);
        delay(20);
        digitalWrite(buzzer, LOW);
        while(digitalRead(KEY_RIGHT) == LOW);
        parameter++;
    }
    
    if(digitalRead(KEY_LEFT) == LOW)
    {
        digitalWrite(buzzer, HIGH);
        delay(20);
        digitalWrite(buzzer, LOW);
        while(digitalRead(KEY_LEFT) == LOW);
        parameter--;
    }
    
    if((parameter > 7) || (parameter < 1))
    {
        parameter = 0;
    }
    
    return value;
}


void set_RGB_LED_max_brightness()
{
    unsigned int avg = 0;
    unsigned char samples = 16;

    while(samples > 0)
    {
        avg += analogRead(A0);
        delayMicroseconds(10);
        samples--;
    };
    max_b = (avg >> 6);
}


void transmit_data()
{
    if(tgl)
    {
        Serial.print(T.mday);
        Serial.print("/");
        Serial.print(T.mon);
        Serial.print(".");
        Serial.print(T.year);
        Serial.print("    ");
        Serial.print(T.hour);
        Serial.print(".");
        Serial.print(T.min);
        Serial.print(".");
        Serial.print(T.sec);
        Serial.print("    ");
        Serial.print("T/'C: ");
        Serial.println(t_p);
    }
}


void setup() 
{
    unsigned char i = 0;

    Serial.begin(9600);
    Serial.flush();
  
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.clearDisplay();
    
    RGB_LED.pwr_set(PWR_ENABLE); 
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, LOW);
    
    DS3231_init(DS3231_INTCN);

    for(i = A1; i <= A5; i++)
    {  
        pinMode(i, INPUT_PULLUP);
    }

    draw_background();
}


void loop() 
{
    set_RGB_LED_max_brightness();
    show_temperature();
    clock_stuffs();
    accelerometer_and_RGB_LED();
    transmit_data();
    oled.display();
}
