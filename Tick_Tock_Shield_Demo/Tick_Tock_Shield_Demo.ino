#include <Wire.h>


//Shield I/O connection//

#define LED1                                 2
#define LED2                                 3
#define LED3                                 4
#define LED4                                 5

#define Buzzer                               6

#define CLK                                  7
#define SDIO                                 8

#define Button1                              9
#define Button2                             10
#define Button3                             11

#define Thermistor                          A0
#define LDR                                 A1
#define V_batt                              A2


//Miscellaneous//

#define kbd_dly                              6

#define Menu_BTN                             1
#define Inc_BTN                              2
#define Dec_BTN                              3

#define time_disp                         0x00
#define date_disp                         0x01
#define year_disp                         0x02
#define day_disp                          0x03
#define temp_disp                         0x04
#define lux_disp                          0x05
#define BV_disp                           0x06
#define no_disp                           0x07

//TM1636 Defintions//

#define Write_Display_Data_Cmd            0x40
#define Read_Key_Data_Cmd                 0x42
#define Auto_Address_Increment            0x40
#define Fixed_Addressing                  0x44
#define Normal_Mode                       0x40
#define Test_Mode                         0x48

#define Display_Address                   0xC0

#define Display_Off                       0x80
#define Display_On                        0x88

#define point                             0x80


//Thermistor Definitions//

#define B                               3975.0
#define _25C_K                          298.15
#define _0C_K                           273.15
#define VDD_max                           1023
#define R0                             10000.0
#define R_fixed                        10000.0                     


//LDR Definitions//

#define LDR_constant                  900000.0
#define R_fixed                        10000.0


//DS1307 RTC Definitions//

#define sec_reg                           0x00
#define min_reg                           0x01
#define hr_reg                            0x02
#define day_reg                           0x03
#define date_reg                          0x04
#define month_reg                         0x05
#define year_reg                          0x06
#define control_reg                       0x07

#define DS1307_addr                       0x68

#define _12_hr                            false
#define _24_hr                            true

#define pm                                false
#define am                                true


//Global variables//

boolean sec_point_state = false;
boolean set_time = false;
boolean disp_state = true;
boolean toggle = true;

signed char b_lvl = 4;
signed char menu_no = 0;

unsigned char z = 0;
unsigned char i = time_disp;
unsigned char m = 10;
unsigned char s = 30;
unsigned char h = 10;
unsigned char dy = 1;
unsigned char dt = 1;
unsigned char mt = 1;
unsigned char yr = 15;

float R_inf = 0.0;

const unsigned char chr[33] = 
{
  0x3F, 0x06, 0x5B, 
  0x4F, 0x66, 0x6D, 
  0x7D, 0x07, 0x7F, 
  0x6F, 0x77, 0x7C, 
  0x39, 0x5E, 0x79, 
  0x71, 0x40, 0x00, 
  0x77, 0x58, 0x5E, 
  0x79, 0x71, 0x74, 
  0x10, 0x37, 0x54, 
  0x5C, 0x73, 0x50, 
  0x6D, 0x78, 0x3E
};
//0~9,A,b,C,d,E,F,"-"," "//
//a(18) c(19) d(20) e(21) f(22) h(23) i(24) m(25) n(26) o(27) p(28) r(29) s(30) t(31) u(32)     


void setup() 
{
  calculate_R_inf();
  
  for(z = LED1; z <= SDIO; z++)
  {
      pinMode(z, OUTPUT);
      digitalWrite(z, LOW);
  }
  
  for(z = Button1; z <= Button3; z++)
  {
      pinMode(z, INPUT_PULLUP);
  }
  
  Wire.begin();
  DS1307_init();
  clear_display();
}

void loop() 
{
  get_time();
  display_values();
  change_and_set_display_brightness();
  change_display();
  adjust_time();
  delay(460);
}


unsigned char inc_dec(signed char value, signed char max_value, signed char min_value)
{
  if(get_keypad() == Inc_BTN)
  {
    value++;
  }
  if(value > max_value)
  {
    value = min_value;
  }
  
  if(get_keypad() == Dec_BTN)
  {
    value--;
  }
  if(value < min_value)
  {
    value = max_value;
  }
  
  return value;
}


void LED_states(unsigned char value)
{
   digitalWrite(LED1, (value & 0x01));
   digitalWrite(LED2, ((value & 0x02) >> 1));
   digitalWrite(LED3, ((value & 0x04) >> 2));
   digitalWrite(LED4, ((value & 0x08) >> 3));
}


void keyboard_click()
{
  digitalWrite(Buzzer, HIGH);
  delay(kbd_dly);
  digitalWrite(Buzzer, LOW);
}


unsigned char get_keypad()
{
  if(digitalRead(Button3) == false)
  {
    keyboard_click();
    return Menu_BTN;
  }
  else if(digitalRead(Button2) == false)
  {
    keyboard_click();
    return Inc_BTN;
  }
  else if(digitalRead(Button1) == false)
  {
    keyboard_click();
    return Dec_BTN;
  }
  else
  {
    return 0;
  }
}


void display_values()
{
  switch(i)
  {
    case time_disp:
    {
      LED_states(9);
      show_time();
      break;
    }    
    case date_disp:
    {
      LED_states(5);
      show_date();
      break; 
    }
    case year_disp:
    {
      LED_states(3);
      show_year();
      break; 
    }
    case day_disp:
    {
      LED_states(1);
      show_day();
      break; 
    }
    case temp_disp:
    {
      LED_states(2);
      show_temperature();
      break; 
    }
    case lux_disp:
    {
      LED_states(4);
      display_light_intensity();
      break; 
    }
    case BV_disp:
    {
      LED_states(8);
      show_battery_voltage();
      break; 
    }
    default:
    {
      LED_states(0);
      clear_display();
      break;
    }
  }
  if((toggle == true) && (set_time == true))
  {
    clear_display();
  }
}

void change_display()
{
  sec_point_state ^= 1;
  toggle ^= 1;   
  
  if((set_time == false) && (get_keypad() == Dec_BTN))
  {
    i++;
    clear_display();
    
    if(i > no_disp)
    {
      i = time_disp;
    }
  }
  if((i >= date_disp) && (i <= no_disp))
  {
    sec_point_state = false;
  }
}


void change_and_set_display_brightness()
{   
  if((set_time == false) && (get_keypad() == Inc_BTN))
  {
    b_lvl++;   
  
    if(b_lvl >= 0x08)
    {
      b_lvl = -1;
      disp_state = false;
    }
    else
    {
      disp_state = true;
    }
  }
  
  if(disp_state == true)
  {
    dislay_status(b_lvl, Display_On);
  }
  else
  {
    dislay_status(b_lvl, Display_Off);
  }
}


void TM1636_start()
{
  digitalWrite(CLK, HIGH);
  digitalWrite(SDIO, HIGH); 
  digitalWrite(SDIO, LOW); 
} 


void TM1636_stop()
{
  digitalWrite(CLK, LOW);
  digitalWrite(SDIO, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(SDIO, HIGH); 
}


void TM1636_ack()
{
  digitalWrite(CLK, LOW);
  pinMode(SDIO, INPUT_PULLUP);
  while(digitalRead(SDIO) == HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW); 
  pinMode(SDIO, OUTPUT);
}


void TM1636_write_byte(unsigned char value)
{
  for(z = 0; z < 8; z++)
  {
    digitalWrite(CLK, LOW);
    if((value & 0x01) != 0)
    {
      digitalWrite(SDIO, HIGH);
    }
    else
    {
      digitalWrite(SDIO, LOW);
    }
    value >>= 1;
    digitalWrite(CLK, HIGH); 
  }
}


unsigned char TM1636_scan_keys()
{
  unsigned char key = 0;

  TM1636_start();
  TM1636_write_byte(Read_Key_Data_Cmd); 
  TM1636_ack();
  digitalWrite(SDIO, HIGH);
  
  for(z = 0; z < 8; z++)
  {
    digitalWrite(CLK, LOW);
    key >>= 1;
    delayMicroseconds(30);
    digitalWrite(CLK, HIGH);
    pinMode(SDIO, INPUT_PULLUP);

    if(digitalRead(SDIO) == HIGH)
    {
      key |= 0x80;
    }
    delayMicroseconds(30);
  }
  
  TM1636_ack();
  TM1636_stop();

  return key;
}


void display_data(unsigned char value, unsigned char location)
{
  TM1636_start();
  TM1636_write_byte(Fixed_Addressing);
  TM1636_ack();
  TM1636_stop();
  
  TM1636_start();
  TM1636_write_byte(Display_Address | (location & 0x03));
  TM1636_ack();

  if((sec_point_state == true) && (i == time_disp))
  {
    TM1636_write_byte(chr[value] | point);
  }
  else
  {
    TM1636_write_byte(chr[value]);
  }
  
  TM1636_ack();
  TM1636_stop();  
}


void dislay_status(unsigned char brightness, unsigned char display_state)
{
  TM1636_start();
  TM1636_write_byte(display_state | (brightness & 0x07));
  TM1636_ack();
  TM1636_stop();
}


void clear_display()
{
  display_data(0x11, 0);
  display_data(0x11, 1);
  display_data(0x11, 2);
  display_data(0x11, 3);
}


unsigned int adc_avg(unsigned char channel)
{
  unsigned char samples = 64;
  unsigned int avg = 0;

  while(samples > 0)
  {
    avg += analogRead(channel);
    samples--;
    delayMicroseconds(10);
  }
  avg >>= 6;

  return avg;
}


void calculate_R_inf()
{
  R_inf = (-1.0 * (B / _25C_K));
  R_inf = exp(R_inf); 
  R_inf *= R0;
}


signed int measure_temperature()
{
  float T_therm = 0;

  T_therm = adc_avg(Thermistor);
  if(T_therm > 0)
  {
    T_therm = ((VDD_max / T_therm) - 1.0);
    T_therm *= R_fixed;

    T_therm = (B / (log (T_therm / R_inf)));
    T_therm = (T_therm - _0C_K);

    return T_therm;
  }
  else
  {
    return 126;
  }
}


void show_temperature()
{
  signed char T = 0;

  T = measure_temperature();

  if(T <= -40)
  {
    T = -40;
  }
  if(T > 125)
  {
    T = 126;
  }

  if((T >= -40) && (T <= 99))
  {
    if(T > 0)
    {
      display_data(0x11, 0);
    }
    else
    {
      display_data(0x10, 0);
      T *= -1;
    }
    display_data((T / 10), 1);
    display_data((T % 10), 2);
  }
  else if((T > 99) && (T < 126))
  {
    display_data((T / 100), 0);
    display_data(((T / 10) % 10), 1);
    display_data((T % 10), 2);
  }
  else
  {
    display_data(0x10, 0);
    display_data(0x10, 1);
    display_data(0x10, 2);
  }
  
  display_data(0x0C, 3);
}


signed int measure_light_intensity()
{
  float lux = 0;

  lux = adc_avg(LDR);
  if(lux > 0)
  {
    lux = ((VDD_max / lux) - 1.0);
    lux *= R_fixed;
    lux = (LDR_constant / lux);
  }

  if((lux >= 0) && (lux <= 9999))
  {
    return lux;
  }
  else
  {
    return -1;
  }
}


void display_light_intensity()
{
  signed int lx = 0;
  
  lx = measure_light_intensity();
  
  if(lx < 0)
  {
    display_data(0x10, 0);
    display_data(0x10, 1);
    display_data(0x10, 2);
    display_data(0x10, 3);
  }
  else
  {
    display_data((lx / 1000), 0);
    display_data(((lx / 100) % 10), 1);
    display_data(((lx / 10) % 10), 2);
    display_data((lx % 10), 3);
  }
}


void show_battery_voltage()
{
  unsigned int mV = 0;

  mV = adc_avg(V_batt);
  mV = ((mV * 5000.0) / VDD_max);
  display_data((mV / 1000), 0);
  display_data(((mV / 100) % 10), 1);
  display_data(((mV / 10) % 10), 2);
  display_data((mV % 10), 3);
}


unsigned char bcd_to_decimal(unsigned char value)                
{                                                                                          
  return ((value & 0x0F) + (((value & 0xF0) >> 0x04) * 0x0A));
}                                
                                                             

unsigned char decimal_to_bcd(unsigned char value)
{
  return (((value / 0x0A) << 0x04) & 0xF0) | ((value % 0x0A) & 0x0F);
} 


void DS1307_init()
{
  DS1307_write(sec_reg, 0x00);
}


void DS1307_read()
{                                     
  unsigned char value = 0; 
  
  Wire.beginTransmission(DS1307_addr);   
  Wire.write(sec_reg);               
  Wire.requestFrom(DS1307_addr, 7);  
  s = Wire.read();
  m = Wire.read();
  h = Wire.read();
  dy = Wire.read();
  dt = Wire.read();
  mt = Wire.read();
  yr = Wire.read();
  
  Wire.endTransmission(); 
}                     


void DS1307_write(unsigned char address, unsigned char value)    
{  
  Wire.beginTransmission(DS1307_addr);   
  Wire.write(address);
  Wire.write(value);                   
  Wire.endTransmission();
}  


void get_time()
{
  if(set_time == false)
  {    
    DS1307_read();
  
    s &= 0x7F;
    s = bcd_to_decimal(s);
  
    m &= 0x7F;
    m = bcd_to_decimal(m);
  
    h &= 0x3F;
    h = bcd_to_decimal(h);
  
    dt &= 0x3F;
    dt = bcd_to_decimal(dt);
  
    mt &= 0x1F; 
    mt = bcd_to_decimal(mt);
    
    yr = bcd_to_decimal(yr);
  
    dy &= 0x07;
    dy = bcd_to_decimal(dy);
  }
}


void show_time()
{
  display_data((h / 10), 0);
  display_data((h % 10), 1);
  display_data((m / 10), 2);
  display_data((m % 10), 3);
}


void show_date()
{
  display_data((dt / 10), 0);
  display_data((dt % 10), 1);
  display_data((mt / 10), 2);
  display_data((mt % 10), 3);
}


void show_year()
{
  display_data(2, 0);
  display_data(0, 1);
  display_data((yr / 10), 2);
  display_data((yr % 10), 3);
}


void show_day()
{
  unsigned char n = 0;
  unsigned char tmp[4];
  
  switch(dy)
  {
    case 1:
    {
      tmp[0] = 30;
      tmp[1] = 18;
      tmp[2] = 31;
      tmp[3] = 17;
      break;
    }
    case 2:
    {
      tmp[0] = 30;
      tmp[1] = 32;
      tmp[2] = 26;
      tmp[3] = 17;
      break;
    }
    case 3:
    {
      tmp[0] = 25;
      tmp[1] = 25;
      tmp[2] = 27;
      tmp[3] = 26;
      break;
    }
    case 4:
    {
      tmp[0] = 31;
      tmp[1] = 32;
      tmp[2] = 21;
      tmp[3] = 17;
      break;
    }
    case 5:
    {
      tmp[0] = 32;
      tmp[1] = 32;
      tmp[2] = 21;
      tmp[3] = 20;
      break;
    }
    case 6:
    {
      tmp[0] = 31;
      tmp[1] = 23;
      tmp[2] = 29;
      tmp[3] = 17;
      break;
    }
    case 7:
    {
      tmp[0] = 22;
      tmp[1] = 29;
      tmp[2] = 24;
      tmp[3] = 17;
      break;
    }
  }

  for(n = 0; n < 4; n++)
  {
    display_data(tmp[n], n);
  }
}


void setup_time()
{
  DS1307_write(year_reg, decimal_to_bcd(yr));
  DS1307_write(month_reg, decimal_to_bcd(mt));
  DS1307_write(date_reg, decimal_to_bcd(dt));
  DS1307_write(day_reg, decimal_to_bcd(dy));
  DS1307_write(hr_reg, decimal_to_bcd(h));
  DS1307_write(min_reg, decimal_to_bcd(m));
  DS1307_write(sec_reg, 0x00);
}


void adjust_time()
{ 
  if((get_keypad() == Menu_BTN) && (set_time == false))
  {
      while(get_keypad() == Menu_BTN);      
      set_time = true;
      DS1307_write(sec_reg, 0x80);
      i = time_disp;
      menu_no = 0;
      delay(900);
  }

  if(set_time == true)
  {
    if(get_keypad() == Menu_BTN)
    {
      while(get_keypad() == Menu_BTN); 
      menu_no++;

      if(menu_no == 2)
      {
        i = date_disp;
      }

      if(menu_no == 4)
      {
        i = year_disp;
      }
      
      if(menu_no == 5)
      {
        i = day_disp;
      }
      
      if(menu_no == 6)
      {
          menu_no = 0;
          i = time_disp;
          set_time = false;
          setup_time();
      }
    }
    
    if(menu_no == 0)
    { 
      h = inc_dec(h, 23, 0);
    }
    if(menu_no == 1)
    { 
      m = inc_dec(m, 59, 0);
    }

    if(menu_no == 2)
    { 
      dt = inc_dec(dt, 31, 1);
    }
    if(menu_no == 3)
    { 
      mt = inc_dec(mt, 12, 1);
    }

    if(menu_no == 4)
    { 
      yr = inc_dec(yr, 99, 0);
    }
    if(menu_no == 5)
    { 
      dy = inc_dec(dy, 7, 1);
    }
  }
}

