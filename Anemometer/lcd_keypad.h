#pragma once

#include <stdint.h>
#include <Arduino.h>


#define keypad_pin				       A0

#define LCD_RS_pin				       8
#define LCD_EN_pin				       9

#define LCD_D4_pin				       4
#define LCD_D5_pin				       5
#define LCD_D6_pin				       6
#define LCD_D7_pin				       7

#define LCD_RS_HIGH              digitalWrite(LCD_RS_pin, HIGH)
#define LCD_RS_LOW               digitalWrite(LCD_RS_pin, LOW)

#define LCD_EN_HIGH              digitalWrite(LCD_EN_pin, HIGH)
#define LCD_EN_LOW               digitalWrite(LCD_EN_pin, LOW)

#define LCD_DB4_HIGH             digitalWrite(LCD_D4_pin, HIGH)
#define LCD_DB4_LOW              digitalWrite(LCD_D4_pin, LOW)

#define LCD_DB5_HIGH             digitalWrite(LCD_D5_pin, HIGH)
#define LCD_DB5_LOW              digitalWrite(LCD_D5_pin, LOW)

#define LCD_DB6_HIGH             digitalWrite(LCD_D6_pin, HIGH)
#define LCD_DB6_LOW              digitalWrite(LCD_D6_pin, LOW)

#define LCD_DB7_HIGH             digitalWrite(LCD_D7_pin, HIGH)
#define LCD_DB7_LOW              digitalWrite(LCD_D7_pin, LOW)

#define clear_display            0x01
#define goto_home                0x02

#define cursor_direction_inc     0x06
#define cursor_direction_dec     0x04
#define display_shift            0x05
#define display_no_shift         0x04

#define display_on               0x0C
#define display_off              0x0A
#define cursor_on                0x0A
#define cursor_off               0x08
#define blink_on                 0x09
#define blink_off                0x08

#define _8_pin_interface         0x30
#define _4_pin_interface         0x20
#define _2_row_display           0x28
#define _1_row_display           0x20
#define _5x10_dots               0x60
#define _5x7_dots                0x20

#define line_1_y_pos             0x00
#define line_2_y_pos             0x40
#define line_3_y_pos             0x14
#define line_4_y_pos             0x54

#define DAT                      true
#define CMD                      false

// VCC = 1024 //

#define kbd_select_max           750  //(0.75 * VCC)
#define kbd_select_min           500  //(0.50 * VCC)

#define kbd_left_max             500  //(0.50 * VCC)
#define kbd_left_min             350  //(0.30 * VCC)

#define kbd_down_max             350  //(0.35 * VCC)
#define kbd_down_min             150  //(0.15 * VCC)

#define kbd_up_max               150  //(0.15 * VCC)
#define kbd_up_min               50   //(0.05 * VCC)

#define kbd_right_max            50   //(0.05 * VCC)
#define kbd_right_min            0    //(0.00 * VCC)

#define up_key                   1
#define down_key                 2
#define left_key                 3
#define right_key                4
#define select_key               5


class lcd_kbd
{
	public:
		lcd_kbd();
		void begin(void);
		void init(void);
		void goto_xy(uint8_t x_pos, uint8_t y_pos);
		void chr(uint8_t x_pos, uint8_t y_pos, char ch);
		void text(uint8_t x_pos, uint8_t y_pos, const char *ch);
    uint8_t get_key(void);

	private:
		void send(uint8_t lcd_data, uint8_t mode);
    void send_4_bits(uint8_t value);
		void clear(void);
		void toggle_EN_pin(void);
    uint16_t adc_avg(void);
};