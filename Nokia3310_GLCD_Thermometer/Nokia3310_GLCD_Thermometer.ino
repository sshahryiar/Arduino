#define nok_sclk                           2
#define nok_sda                            3
#define nok_dc                             4
#define nok_cs                             5
#define nok_res                            6
#define nok_bkl                            7
#define LED                               13

#define ON                                 1
#define OFF                                0

#define AM                                 1
#define PM                                 0

#define PIXEL_OFF                          0
#define PIXEL_ON                           1
#define PIXEL_XOR                          2

#define LCDROWMAX                          6
#define LCDCOLMAX                         84
#define LCDPIXELROWMAX                    48   

#define fixed_R                      10000.0
#define Beta                          3975.0
#define Ro                          0.007569
#define alarmPin                          16


char tmp[16];

signed int t_present = 0;
signed int t_previous = 0;
signed int max_t = 0;
signed int min_t = 70;

unsigned char time = 0;
unsigned char char_row = 0;
unsigned long plot_value32 = 0; 
unsigned long plot_umsb = 0;
unsigned long plot_lmsb = 0;
unsigned long plot_ulsb = 0;
unsigned long plot_llsb = 0;

static unsigned char lcd_buffer[LCDROWMAX][LCDCOLMAX];

static const byte ASCII[96][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};


void nokia_init(); 
void nokia_write_command(unsigned char bytefornokia_command); 
void nokia_write_data(unsigned char bytefornokia_data); 
void nokia_write_dorc(unsigned char bytefornokia); 
void nokia_gotoxy(unsigned char xnokia, unsigned char ynokia); 
void nokia_erase_y(unsigned char ynokia); 
void nokia_erase_x(unsigned char xnokia); 
void nokia_printchar(unsigned char cvar); 
void nokia_clean_ddram(); 
void nokia_plot(unsigned char xnokia,unsigned char plot_value8); 
void nokia_write_data_inv(unsigned char bytefornokia_data_inv); 
void nokia_clear_screen(); 
void nokia_clear_xy(unsigned char xnokia, unsigned char ynokia); 
void lcd_pixel (signed char x, signed char y, signed char c);
void lcd_update();
void draw_line(signed char x1, signed char y1, signed char x2, signed char y2, unsigned char c);
void draw_circle(unsigned char xc, unsigned char yc, unsigned char r, unsigned char c); 
void draw_filled_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char c); 
void draw_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char c);
void backlight_state(boolean dat);
void lcd_character(char character);
void lcd_string(char *characters);
void draw_background();
unsigned int adc_avg();


void nokia_init() 
{ 
  unsigned char s = 0;
  
  for(s = 2; s < 8; s++)	
  {
    pinMode(s, OUTPUT);
    digitalWrite(s, LOW);
  } 
  
  digitalWrite(nok_dc, HIGH);
  digitalWrite(nok_cs, HIGH);
  digitalWrite(nok_res, LOW);
  delay(10);
  digitalWrite(nok_res, HIGH);
  nokia_write_command(0x21);  
  nokia_write_command(0xc2);    
  nokia_write_command(0x13); 
  nokia_write_command(0x20); 
  nokia_write_command(0x09);   
  nokia_clean_ddram();      
  nokia_write_command(0x08);  
  nokia_write_command(0x0c);  
} 


void nokia_clean_ddram() 
{ 
  unsigned int ddram = 0; 
  
  nokia_gotoxy(0, 0);        
  for(ddram = 504; ddram > 0; ddram--)
  {
    nokia_write_data(0x00);
  } 
} 


void nokia_write_command(unsigned char bytefornokia_command) 
{ 
  digitalWrite(nok_dc, LOW);
  digitalWrite(nok_cs, LOW);
  nokia_write_dorc(bytefornokia_command); 
  digitalWrite(nok_cs, HIGH);
} 


void nokia_write_data(unsigned char bytefornokia_data) 
{ 
  digitalWrite(nok_dc, HIGH);
  digitalWrite(nok_cs, LOW);
  nokia_write_dorc(bytefornokia_data); 
  digitalWrite(nok_cs, HIGH);
} 


void nokia_write_dorc(unsigned char bytefornokia)      
{ 
  unsigned char caa = 0;
  
  for (caa = 8; caa > 0; caa--) 
  { 
    digitalWrite(nok_sclk, LOW);
    if ((bytefornokia & 0x80) == 0)
    {
      digitalWrite(nok_sda, LOW);
    } 
    else 
    {
      digitalWrite(nok_sda, HIGH);
    } 
    digitalWrite(nok_sclk, HIGH); 
    bytefornokia = (bytefornokia << 1); 
  } 
} 


void nokia_gotoxy(unsigned char xnokia, unsigned char ynokia)      
{ 
  nokia_write_command(0x40 | (ynokia & 0x07));    
  nokia_write_command(0x80 | (xnokia & 0x7f));  
} 


void nokia_erase_y(unsigned char ynokia) 
{ 
  unsigned char s = 0;
  for(s = 0; s < 47; s++)
  {
    nokia_gotoxy(s, ynokia); 
    lcd_string(" "); 
  }
}


void nokia_erase_x(unsigned char xnokia) 
{ 
  unsigned char column = 0; 

  for(column = 0; column != 6; column++)
  { 
    nokia_gotoxy(xnokia, column); 
    nokia_write_data(0x00); 
    nokia_write_data(0x00); 
    nokia_write_data(0x00); 
    nokia_write_data(0x00); 
    nokia_write_data(0x00); 
    nokia_write_data(0x00); 
  } 
} 


void nokia_plot(unsigned char xnokia, unsigned char plot_value8) 
{ 
  unsigned char i = 0; 

  plot_value32 = 0; 
  plot_value32 |= 1;         // unremark this if you want dotgraph instead of bargraph 

  for(i = 0; i != plot_value8; i++)
  { 

    //plot_value32 |= 1;         // remark this if you want dotgraph instead of bargraph 
    plot_value32 <<= 1; 
  } 

  plot_value32 |= 2;            // bottom line is always filled            

  plot_llsb = (plot_value32 & 0xff); 
  plot_ulsb = ((plot_value32 >> 8) & 0xff); 
  plot_lmsb = ((plot_value32 >> 16) & 0xff); 
  plot_umsb = ((plot_value32 >> 24) & 0xff); 

  nokia_gotoxy(xnokia, 1); 
  nokia_write_data_inv(plot_umsb); 
  nokia_gotoxy(xnokia, 2); 
  nokia_write_data_inv(plot_lmsb); 
  nokia_gotoxy(xnokia, 3); 
  nokia_write_data_inv(plot_ulsb); 
  nokia_gotoxy(xnokia, 4); 
  nokia_write_data_inv(plot_llsb); 
} 


void nokia_write_data_inv(unsigned char bytefornokia_data_inv) 
{ 
  unsigned char caa = 0; 
  
  digitalWrite(nok_dc, HIGH);
  digitalWrite(nok_cs, LOW);

  for(caa = 8; caa > 0; caa--) 
  { 
    digitalWrite(nok_sclk, LOW);  
    delayMicroseconds(2); 
    if((bytefornokia_data_inv & 0x01) == 0)
    {
      digitalWrite(nok_sda, LOW);
    } 
    else 
    {
      digitalWrite(nok_sda, HIGH);
    }
    
    digitalWrite(nok_sclk, HIGH); 
    bytefornokia_data_inv = (bytefornokia_data_inv >> 1); 
  } 
  digitalWrite(nok_cs, HIGH);  
} 


void nokia_clear_screen() 
{ 
  nokia_erase_y(0); 
  nokia_erase_y(1); 
  nokia_erase_y(2); 
  nokia_erase_y(3); 
  nokia_erase_y(4); 
  nokia_erase_y(5); 
} 


void nokia_clear_xy(unsigned char xnokia, unsigned char ynokia) 
{ 
  nokia_gotoxy(xnokia, ynokia); 
  lcd_string(" "); 
} 


void lcd_pixel(signed char x, signed char y, signed char c) 
{    
    unsigned char row = 0;
    unsigned char value = 0;
    
    if((x < 0) || (x >= LCDCOLMAX) || (y < 0) || (y >= LCDPIXELROWMAX)) 
    {
      return;
    }
    row = (y >> 3);

    value = lcd_buffer[row][x];
    if(c == PIXEL_ON) 
    {
        value |= (1 << (y % 8));
    } 
    else if(c == PIXEL_XOR) 
    {
        value ^= (1 << (y % 8));
    } 
    else 
    {
        value &= (~(1 << (y % 8)));
    }

    lcd_buffer[row][x] = value;
    
    nokia_gotoxy(x, row);
    nokia_write_data(value);
}


void lcd_update() 
{
    unsigned char i = 0;
    unsigned char j = 0;

    for(i = 0; i < LCDROWMAX; i++) 
    {
        nokia_gotoxy(0, i);
        for(j = 0; j < LCDCOLMAX; j++) 
        {
            nokia_write_data(lcd_buffer[i][j]);
        }
    }
}


void draw_line(signed char x1, signed char y1, signed char x2, signed char y2, unsigned char c) 
{    
    signed char dx = 0;
    signed char dy = 0;
    signed char stepx = 0;
    signed char stepy = 0;
    signed char fraction = 0;

    dy = (y2 - y1);
    dx = (x2 - x1);
    
    if (dy < 0) 
    {
        dy = -dy;
        stepy--;
    } 
    else 
    {
        stepy = 1;
    }

    if (dx < 0) 
    {
        dx = -dx;
        stepx--;
    } 
    else 
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;
    
    lcd_pixel(x1, y1, c);
    
    if (dx > dy) 
    {
        fraction = (dy - (dx >> 1));
        while (x1 != x2) 
        {
            if (fraction >= 0) 
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;
            lcd_pixel(x1, y1, c);
        }
    } 
    else 
    {
        fraction = (dx - (dy >> 1));
        while (y1 != y2) 
        {
            if(fraction >= 0) 
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            lcd_pixel(x1, y1, c);
         }
    }
}


void draw_circle(unsigned char xc, unsigned char yc, unsigned char r, unsigned char c) 
{
    unsigned char x = 0;
    unsigned char y = r;
    signed char p = 0; 
    
    p = (3 - (2 * r));
    lcd_pixel((xc + x), (yc - y), c);

    for(x = 0; x <= y; x++) 
    {
        if(p < 0) 
        {
            y = y;
            p = (p + (4 * x) + 6);
        } 
        else 
        {
            y--;
            p = (p + ((4 * (x - y) + 10)));
        }

        lcd_pixel((byte)(xc + x), (byte)(yc - y), c);
        lcd_pixel((byte)(xc - x), (byte)(yc - y), c);
        lcd_pixel((byte)(xc + x), (byte)(yc + y), c);
        lcd_pixel((byte)(xc - x), (byte)(yc + y), c);
        lcd_pixel((byte)(xc + y), (byte)(yc - x), c);
        lcd_pixel((byte)(xc - y), (byte)(yc - x), c);
        lcd_pixel((byte)(xc + y), (byte)(yc + x), c);
        lcd_pixel((byte)(xc - y), (byte)(yc + x), c);
    }
}


void draw_filled_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char c) 
{
    unsigned char i = 0;
    for(i = y1; i <= y2; i++) 
    {
        draw_line(x1, i, x2, i, c);
    }
}


void draw_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char c)
{
    draw_line(x1, y1, x2, y1, c);
    draw_line(x1, y1, x1, y2, c);
    draw_line(x1, y2, x2, y2, c);
    draw_line(x2, y1, x2, y2, c);
} 


void backlight_state(boolean dat)
{
    if(dat)
    {	
      digitalWrite(nok_bkl, HIGH);
    }
    else	
    {
      digitalWrite(nok_bkl, LOW); 
    }  
} 


void lcd_character(char character)
{
  unsigned char index = 0;
  for(index = 0; index < 5; index++)
  {
    nokia_write_data(ASCII[character - 0x20][index]);
  }
}


void lcd_string(char *characters)
{
  while(*characters)
  {
    lcd_character(*characters++);
  }
}


void draw_background()
{
    draw_line(8, 1, 10, 1, true);  
    draw_line(7, 2, 7, 38, true); 
    draw_line(11, 2, 11, 38, true);
    draw_line(7, 39, 11, 39, true);
    draw_line(6, 40, 12, 40, true);
    draw_line(5, 41, 13, 41, true);
    draw_line(5, 42, 13, 42, true);
    draw_line(5, 43, 13, 43, true);
    draw_line(5, 44, 13, 44, true);
    draw_line(6, 45, 12, 45, true);
    draw_line(7, 46, 11, 46, true);
    
    nokia_gotoxy(22, 0);
    lcd_string("T avg/'C:"); 
    nokia_gotoxy(22, 1);
    lcd_string("T max/'C:");
    nokia_gotoxy(22, 2);
    lcd_string("T min/'C:");
    nokia_gotoxy(22, 3);
    lcd_string("R/ohms:");
}


unsigned int adc_avg()
{
    unsigned char samples = 16;
    register unsigned int avg = 0;
    
    while(samples > 0)
    {
        avg += analogRead(A1);
        delayMicroseconds(9);
        samples--;
    }
    
    avg >>= 4;
    
    return avg;
}


void setup()
{
    backlight_state(true);
    nokia_init();
    nokia_clear_screen();
    draw_background();
    
    pinMode(alarmPin, INPUT);
}


void loop()
{ 
    float RT = 0;
    float r = 0;
    unsigned char l = 0;
    
    if(time == 0)
    {
         max_t = 0;
         min_t = 70;
    }
    time++;
    
    
    t_present = adc_avg();
    
    RT = ((1023.0 / t_present) - 1);
    r = (fixed_R / RT); 
    
    RT = (Beta / (log (r / Ro)));
    t_present = (RT - 273.0);
    
    if(t_present >= 70)
    {
       t_present = 70;
    }
    
    if(t_present <= 0)
    {
       t_present = 0;
    }
    
    if(digitalRead(alarmPin) == true)
    {
       backlight_state(false);
       nokia_gotoxy(22, 5);
       lcd_string("No Alarm.");
    }
    else
    {
       backlight_state(true);
       nokia_gotoxy(22, 5);
       lcd_string("  Alarm! ");
    }
    
    if(t_present != t_previous)
    {
        if(t_present > max_t)
        {
          max_t = t_present;
        }
        if(t_present < min_t)
        {
          min_t = t_present;
        }
        
        itoa(t_present, tmp, 10);
        nokia_gotoxy(68, 0);
        lcd_string(tmp); 
        
        itoa(max_t, tmp, 10);
        nokia_gotoxy(68, 1);
        lcd_string(tmp); 
        
        itoa(min_t, tmp, 10);
        nokia_gotoxy(68, 2);
        lcd_string(tmp); 
        
        ltoa(r, tmp, 10);
        nokia_gotoxy(60, 3);
        lcd_string(tmp); 
        
        l = map(t_previous, 0, 70, 38, 3);
        draw_line(9, 38, 9, l, false);
        
        l = map(t_present, 0, 70, 38, 3);
        draw_line(9, 38, 9, l, true);
        
    }
    
    t_previous = t_present;
    delay(1000);
}
