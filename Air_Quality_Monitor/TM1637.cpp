 #include "TM1637.h"


static const uint8_t seg_data[95] =
{
  0x00, // (32) 0 	<space>
  0x86, // (33)	1	  !
  0x22, // (34)	2	  "      
  0x7E, // (35)	3	  #  
  0x6D, // (36)	4	  $
  0x00, // (37)	5	  %
  0x00, // (38)	6	  &   
  0x02, // (39)	7	  '
  0x30, // (40)	8	  (
  0x06, // (41)	9	  )                         
  0x63, // (42)	10	*
  0x00, // (43)	11 	+
  0x04, // (44)	12	,                                                   
  0x40, // (45)	13	-
  0x80, // (46)	14	.                                   
  0x52, // (47)	15	/
  0x3F, // (48)	16	0
  0x06, // (49)	17	1
  0x5B, // (50)	18	2
  0x4F, // (51)	19	3
  0x66, // (52)	20	4  
  0x6D, // (53)	21	5  
  0x7D, // (54)	22	6  
  0x27, // (55)	23	7
  0x7F, // (56)	24	8  
  0x6F, // (57)	25	9  
  0x00, // (58)	26	:
  0x00, // (59)	27	;  
  0x00, // (60)	28	<  
  0x48, // (61)	29	=
  0x00, // (62)	30	>  
  0x53, // (63)	31	?
  0x5E, // (64)	32	@  
  0x77, // (65)	33	A  
  0x7E, // (66)	34	B  
  0x39, // (67)	35	C  
  0x3E, // (68)	36	D  
  0x79, // (69)	37	E  
  0x71, // (70)	38	F  
  0x3D, // (71)	39	G  
  0x76, // (72)	40	H  
  0x06, // (73)	41	I  
  0x1F, // (74)	42	J
  0x69, // (75)	43	K
  0x38, // (76)	44	L
  0x15, // (77)	45	M
  0x37, // (78)	46	N
  0x3F, // (79)	47	O
  0x73, // (80)	48	P
  0x67, // (81)	49	Q
  0x31, // (82)	50	R  
  0x6D, // (83)	51	S
  0x78, // (84)	52	T
  0x3E, // (85)	53	U
  0x2A, // (86)	54  V
  0x1D, // (87) 55	W
  0x76, // (88)	56	X
  0x6E, // (89)	57	Y
  0x5B, // (90)	58	Z
  0x39, // (91)	59	[
  0x64, // (92)	60	\ 
  0x0F, // (93)	61	]
  0x00, // (94)	62	^
  0x08, // (95)	63	_
  0x20, // (96)	64	`
  0x5F, // (97)	65	a                                                      
  0x7C, // (98)	66	b
  0x58, // (99)	67	c
  0x5E, // (100) 68	d
  0x7B, // (101) 69	e
  0x31, // (102) 70	f
  0x6F, // (103) 71	g
  0x74, // (104) 72	h
  0x04, // (105) 73	i
  0x0E, // (106) 74	j
  0x75, // (107) 75	k
  0x30, // (108) 76	l
  0x55, // (109) 77	m
  0x54, // (110) 78	n
  0x5C, // (111) 79	o
  0x73, // (112) 80	p
  0x67, // (113) 81	q
  0x50, // (114) 82	r
  0x6D, // (115) 83	s
  0x78, // (116) 84	t
  0x1C, // (117) 85	u
  0x2A, // (118) 86	v
  0x1D, // (119) 87	w
  0x76, // (120) 88	x
  0x6E, // (121) 89	y
  0x47, // (122) 90	z        
  0x46, // (123) 91	{
  0x06, // (124) 92	|
  0x70, // (125) 93	}
  0x01  // (126) 94	~
};


TM1637::TM1637(int dat_pin, int clk_pin)
{
  _dat_pin = dat_pin;
  _clk_pin = clk_pin;
}


void TM1637::begin(void)
{
  pinMode(_clk_pin, OUTPUT);
  pinMode(_dat_pin, OUTPUT);

  send_command(TM1637_CMD_SET_DSIPLAY | TM1637_BRIGHTNESS_4 | TM1637_SET_DISPLAY_ON);
  clear();
}


void TM1637::clear(void)
{
  int8_t i = (TM1637_POSITION_MAX - 1);

  while(i > -1)
  {
    write_segments(i, 0, 0);
    i--;
  };
}


void TM1637::write_segments(uint8_t position, uint8_t segment_value, uint8_t dot_state)
{
  switch(dot_state)
  {
    case true:
    {
      segment_value |= 0x80;
      break;
    }

    default:
    {
      segment_value &= 0x7F;
      break;
    }
  }

  send_command(TM1637_CMD_SET_DATA | TM1637_SET_DATA_F_ADDR);
  start();
  write(TM1637_CMD_SET_ADDR | (position & (TM1637_POSITION_MAX - 1)));
  write(segment_value);
  stop();
}


void TM1637::write_number(uint8_t position, uint8_t value, uint8_t dot_state)
{
  put_data(position, (0x10 + value), dot_state);
}


void TM1637::put_data(uint8_t position, uint8_t value, uint8_t dot_state)
{
  write_segments(position, seg_data[value], dot_state);
}


void TM1637::write_character(uint8_t position, uint8_t value)
{
  char chr = (value - 0x20);

  if(chr >= 95) 
  {
    chr = 0; 
  }

  put_data(position, chr, 0);
}


void TM1637::text(int8_t x_pos, const char *ch)
{
  while(*ch != '\0')
  {
    write_character(x_pos, *ch++);
    x_pos++;
  }
}



void TM1637::print_i(int16_t value)
{
  print_number(value, false);
}


void TM1637::print_f(float value)
{
  value *= 10;
  print_number((int16_t)value, true);
}


void TM1637::start(void)
{
  digitalWrite(_clk_pin, HIGH);
  digitalWrite(_dat_pin, HIGH);
  delayMicroseconds(TM1637_DELAY_US);
  digitalWrite(_dat_pin, LOW);
}


void TM1637::stop(void)
{
  digitalWrite(_clk_pin, LOW);
  delayMicroseconds(TM1637_DELAY_US);
  digitalWrite(_dat_pin, LOW);
  delayMicroseconds(TM1637_DELAY_US);
  digitalWrite(_clk_pin, HIGH);
  delayMicroseconds(TM1637_DELAY_US);
  digitalWrite(_dat_pin, HIGH);
}


uint8_t TM1637::write(uint8_t value)
{
  int8_t i = 8;
  uint8_t ack = 0;

  while(i)
  {
    digitalWrite(_clk_pin, LOW);
    delayMicroseconds(TM1637_DELAY_US);
    digitalWrite(_dat_pin, (value & 0x01));
    digitalWrite(_clk_pin, HIGH);
    delayMicroseconds(TM1637_DELAY_US);

    value >>= 1;
    i--;
  };

  digitalWrite(_clk_pin, LOW);
  delayMicroseconds(TM1637_DELAY_US);

  pinMode(_dat_pin, INPUT);

  ack = digitalRead(_dat_pin);

  pinMode(_dat_pin, OUTPUT);

  if(ack)
  {
    digitalWrite(_dat_pin, LOW);
  }

  delayMicroseconds(TM1637_DELAY_US);

  digitalWrite(_clk_pin, HIGH);
  delayMicroseconds(TM1637_DELAY_US);

  digitalWrite(_clk_pin, LOW);
  delayMicroseconds(TM1637_DELAY_US);

  return ack;
}


void TM1637::send_command(uint8_t value)
{
  start();
  write(value);
  stop();
}


void TM1637::print_number(int16_t value, uint8_t dot_state)
{
  uint8_t neg = false;

  if(value < 0)
  {
    value = -value;
    neg = true;
  }

  switch(neg)
  {
    case true:
    {
      if(value > 999)
      {
        value = 10000;
      }
      if((value >= 100) && (value <= 999))
      {
        write_segments(0, 0x40, false);
      }
      else if((value >= 10) && (value <= 99))
      {
        write_segments(0, 0x00, false);
        write_segments(1, 0x40, false);
      }
      else if((value >= 1) && (value <= 9))
      {
        write_segments(0, 0x00, false);
        write_segments(1, 0x00, false);
      }
      break;
    }

    default:
    {
      if(value > 9999)
      {
        value = 10000;
      }
      if((value >= 100) && (value <= 999))
      {
        write_segments(0, 0x00, false);
      }
      else if((value >= 10) && (value <= 99))
      {
        write_segments(0, 0x00, false);   
        write_segments(1, 0x00, false);   
      }
      else if((value >= 0) && (value <= 9))
      {
        write_segments(0, 0x00, false);   
        write_segments(1, 0x00, false); 
      }
      break;
    }
  }

  if(value > 9999)
  {
    for(neg = 0; neg < TM1637_POSITION_MAX; neg++)
    {
      write_segments(neg, 0x40, false);     
    }
  }
  
  if((value >= 1000) && (value <= 9999))
  {
    write_number(0, (value / 1000), false);
    write_number(1, ((value % 1000) / 100), false);
    write_number(2, ((value % 100) / 10), dot_state);
    write_number(3, (value % 10), false);
  }

  else if((value >= 100) && (value <= 999))
  {
    write_number(1, (value / 100), false);
    write_number(2, ((value % 100) / 10), dot_state);
    write_number(3, (value % 10), false);
  }

  else if((value >= 10) && (value <= 99))
  {
    write_number(2, (value / 10), dot_state);
    write_number(3, (value % 10), false);
  }

  else if((value >= 0) && (value <= 9))
  {
    if(dot_state)
    {
      if(neg)
      {
        write_segments(1, 0x40, false);
      }
      write_number(2, 0, true);
    }
    else
    {
      if(neg)
      {
        write_segments(2, 0x40, false);
      }
      else
      {
        write_segments(2, 0x00, false);
      }
    }
    write_number(3, value, false);
  }
}




