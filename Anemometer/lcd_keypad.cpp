#include "lcd_keypad.h"


lcd_kbd::lcd_kbd()
{
}


void lcd_kbd::begin(void)
{
	init();
}


void lcd_kbd::init(void)
{
	for(uint16_t pin = LCD_D4_pin; pin <= LCD_EN_pin; pin++)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	};

  send(0x33, CMD);
  send(0x32, CMD);
  delay(10);
  send(0x33, CMD);
  send(0x32, CMD);
  delay(10);
  send(0x33, CMD);
  send(0x32, CMD);
  delay(10);


  send((display_on | cursor_off | blink_off), CMD);
  clear();
  send((cursor_direction_inc | display_no_shift), CMD);
}


void lcd_kbd::send(uint8_t lcd_data, uint8_t mode)
{
  digitalWrite(LCD_RS_pin, mode);

  send_4_bits(((lcd_data & 0xF0) >> 4));
  send_4_bits((lcd_data & 0x0F));
}


void lcd_kbd::send_4_bits(uint8_t value)
{
  digitalWrite(LCD_D7_pin, ((value & 0x08) >> 3));
  digitalWrite(LCD_D6_pin, ((value & 0x04) >> 2));
  digitalWrite(LCD_D5_pin, ((value & 0x02) >> 1));
  digitalWrite(LCD_D4_pin, (value & 0x01));
	toggle_EN_pin();
}


void lcd_kbd::goto_xy(uint8_t x_pos, uint8_t y_pos)
{
	switch(y_pos)
	{
    case 1:
    {
      send((0x80 | (line_2_y_pos + x_pos)), CMD);
      break;
    }

    case 2:
    {
      send((0x80 | (line_3_y_pos + x_pos)), CMD);
      break;
    }

    case 3:
    {
      send((0x80 | (line_4_y_pos + x_pos)), CMD);
      break;
    }

    default:
    {
      send((0x80 | (line_1_y_pos + x_pos)), CMD);
      break;
    }
	}
}


void lcd_kbd::clear(void)
{
	send(clear_display, CMD);
	send(goto_home, CMD);
}


void lcd_kbd::toggle_EN_pin(void)
{
	LCD_EN_HIGH;
	delay(1);
	LCD_EN_LOW;
	delay(1);
}


void lcd_kbd::chr(uint8_t x_pos, uint8_t y_pos, char ch)
{
	goto_xy(x_pos, y_pos);
	send(ch, DAT);
}


void lcd_kbd::text(uint8_t x_pos, uint8_t y_pos, const char *ch)
{
   do
   {
     chr(x_pos++, y_pos, *ch++);
   }while(*ch != '\0');
}


uint16_t lcd_kbd::adc_avg(void)
{
  int8_t sample = 4;
  uint16_t average = 0;

  while(sample > 0)
  {
    average += analogRead(keypad_pin);
    sample--;
    delayMicroseconds(10);
  };

  average >>= 2;

  return average;
}


uint8_t lcd_kbd::get_key(void)
{
  int16_t adc_value = adc_avg();

  if((adc_value >= kbd_select_min) && (adc_value < kbd_select_max))
  {
    return select_key;
  }

  else if((adc_value >= kbd_left_min) && (adc_value < kbd_left_max))
  {
    return left_key;
  }

  else if((adc_value >= kbd_down_min) && (adc_value < kbd_down_max))
  {
    return down_key;
  }

  else if((adc_value >= kbd_up_min) && (adc_value < kbd_up_max))
  {
    return up_key;
  }

  else if((adc_value >= kbd_right_min) && (adc_value < kbd_right_max))
  {
    return right_key;
  }
  
  else 
  {
    return 0;
  }
}
