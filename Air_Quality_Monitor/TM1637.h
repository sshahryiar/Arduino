#pragma once

#include <Arduino.h>


#define	TM1637_DELAY_US		          4

#define TM1637_BRIGHTNESS_MIN       0
#define TM1637_BRIGHTNESS_1         1
#define TM1637_BRIGHTNESS_2         2
#define TM1637_BRIGHTNESS_3         3
#define TM1637_BRIGHTNESS_4         4
#define TM1637_BRIGHTNESS_5         5
#define TM1637_BRIGHTNESS_6         6
#define	TM1637_BRIGHTNESS_MAX	      7

#define	TM1637_POSITION_MAX	        4

#define	TM1637_CMD_SET_DATA         0x40
#define	TM1637_CMD_SET_ADDR	        0xC0
#define	TM1637_CMD_SET_DSIPLAY	    0x80

#define	TM1637_SET_DATA_WRITE	      0x00
#define	TM1637_SET_DATA_READ	      0x02
#define	TM1637_SET_DATA_A_ADDR	    0x00
#define	TM1637_SET_DATA_F_ADDR	    0x04
#define	TM1637_SET_DATA_M_NORM	    0x00
#define	TM1637_SET_DATA_M_TEST	    0x10
#define	TM1637_SET_DISPLAY_OFF	    0x00 
#define	TM1637_SET_DISPLAY_ON	      0x08 


class TM1637
{
  public:
    TM1637(int dat_pin, int clk_pin);
    void begin(void);
    void clear(void);
    void write_segments(uint8_t position, uint8_t segment_value, uint8_t dot_state);
    void write_number(uint8_t position, uint8_t value, uint8_t dot_state);
    void put_data(uint8_t position, uint8_t value, uint8_t dot_state);
    void write_character(uint8_t position, uint8_t value);
    void text(int8_t x_pos, const char *ch);
    void print_i(int16_t value);
    void print_f(float value);
      
  private:
    int _dat_pin; 
    int _clk_pin;

    void start(void);
    void stop(void);
    uint8_t write(uint8_t value);
    void send_command(uint8_t value);
    void print_number(int16_t value, uint8_t dot_state);
};