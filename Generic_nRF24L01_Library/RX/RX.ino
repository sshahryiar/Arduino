#define IRQ_pin                          8
#define CE_pin                           9
#define SS_pin                          10
#define MOSI_pin                        11
#define MISO_pin                        12
#define SCK_pin                         13

#define R_REGISTER_cmd                0x00
#define W_REGISTER_cmd                0x20
#define R_RX_PL_WID_cmd               0x60
#define R_RX_PAYLOAD_cmd              0x61
#define W_TX_PAYLOAD_cmd              0xA0  
#define W_ACK_PAYLOAD_cmd             0xA8
#define W_TX_PAYLOAD_NO_ACK_cmd       0xB0
#define FLUSH_TX_cmd                  0xE1
#define FLUSH_RX_cmd                  0xE2
#define REUSE_TX_PL_cmd	              0xE3
#define NOP_cmd                       0xFF

#define CONFIG_reg   		      0x00
#define EN_AA_reg                     0x01
#define EN_RXADDR_reg                 0x02
#define SETUP_AW_reg		      0x03
#define SETUP_RETR_reg                0x04                             
#define RF_CH_reg		      0x05                 
#define RF_SETUP_reg                  0x06
#define STATUS_reg                    0x07
#define OBSERVE_TX_reg                0x08
#define RPD_reg                       0x09
#define RX_ADDR_P0_reg                0x0A
#define RX_ADDR_P1_reg                0x0B
#define RX_ADDR_P2_reg                0x0C
#define RX_ADDR_P3_reg                0x0D
#define RX_ADDR_P4_reg                0x0E
#define RX_ADDR_P5_reg                0x0F
#define TX_ADDR_reg                   0x10
#define RX_PW_P0_reg                  0x11
#define RX_PW_P1_reg                  0x12
#define RX_PW_P2_reg                  0x13               
#define RX_PW_P3_reg                  0x14
#define RX_PW_P4_reg                  0x15
#define RX_PW_P5_reg                  0x16
#define FIFO_STATUS_reg               0x17
#define DYNPD_reg                     0x1C
#define FEATURE_reg                   0x1D


unsigned char x = 0;


unsigned char nRF24L01_read()
{
  unsigned char s = 0;
  unsigned char msg = 0;
  
  for(s = 0; s < 8; s++)
  {
      msg <<= 1;
      digitalWrite(SCK_pin, HIGH); 
      if(digitalRead(MISO_pin) != 0)
      {
        msg |= 1;
      }
      digitalWrite(SCK_pin, LOW); 
  }
  
  return msg; 
}


void nRF24L01_write(unsigned char d)
{
  unsigned char s = 0;
  
  for(s = 0; s < 8; s++)
  {
    if((d & 0x80) != 0)
    {
      digitalWrite(MOSI_pin, HIGH);
    } 
    else
    {
      digitalWrite(MOSI_pin, LOW);
    }
    d <<= 1;
    digitalWrite(SCK_pin, HIGH); 
    digitalWrite(SCK_pin, LOW); 
  }
}


void register_write(unsigned char reg, unsigned char value)
{
  digitalWrite(SS_pin, LOW);
  nRF24L01_write((reg | W_REGISTER_cmd));
  nRF24L01_write(value);
  digitalWrite(SS_pin, HIGH);
}


void write_command(unsigned char cmd)
{
  digitalWrite(SS_pin, LOW);
  nRF24L01_write(cmd);
  digitalWrite(SS_pin, HIGH);
}


unsigned char register_read(unsigned char reg)
{
  unsigned char value = 0;
  
  digitalWrite(SS_pin, LOW);
  nRF24L01_write((reg | R_REGISTER_cmd));
  value = nRF24L01_read();
  digitalWrite(SS_pin, HIGH);
  
  return value;
}


void set_TX_RX_address(unsigned char *addr, unsigned char bytes, unsigned char reg)
{
  unsigned char n = 0;
  
  digitalWrite(SS_pin, LOW);
  nRF24L01_write((reg | W_REGISTER_cmd));
  for(n = 0; n < bytes; n++)
  {
    nRF24L01_write(addr[n]);
  }
  digitalWrite(SS_pin, HIGH);
}


void flush_TX_RX()
{
  register_write(STATUS_reg, 0x70);
  write_command(FLUSH_TX_cmd);
  write_command(FLUSH_RX_cmd);
}


void send_data(unsigned char bytes, unsigned char *value)
{
  unsigned char s = 0;
  
  flush_TX_RX();
  register_write(CONFIG_reg, 0x3A);
  
  digitalWrite(SS_pin, LOW);
  nRF24L01_write(W_TX_PAYLOAD_cmd);
  for(s = 0; s < bytes; s++)
  {
    nRF24L01_write(value[s]);
  }
  digitalWrite(SS_pin, HIGH);

  digitalWrite(CE_pin, HIGH);
  delayMicroseconds(60);
  digitalWrite(CE_pin, LOW);
}

void receive_data(unsigned char bytes, unsigned char *value)
{
  unsigned char s = 0;
  
  digitalWrite(SS_pin, LOW);
  nRF24L01_write(R_RX_PAYLOAD_cmd);
  for (s = 0; s < bytes; s++)
  {
    value[s] = nRF24L01_read();
  }
  digitalWrite(SS_pin, HIGH);
}


unsigned char get_Status_Reg()
{
  return register_read(STATUS_reg);
}


void nrF24L01_init()
{
  unsigned char address[5] = {0x99, 0x99, 0x99, 0x99, 0x99};
  
  digitalWrite(CE_pin, LOW);
  
  register_write(CONFIG_reg, 0x38);
  register_write(SETUP_RETR_reg, 0x00);
  register_write(SETUP_AW_reg, 0x03);
  register_write(RF_SETUP_reg, 0x0E);
  register_write(RF_CH_reg, 0x09);  
  register_write(EN_AA_reg, 0x00);
  register_write(RX_PW_P0_reg, 0x01);
  register_write(CONFIG_reg, 0x3B);
  set_TX_RX_address(address, 5, TX_ADDR_reg);
  set_TX_RX_address(address, 5, RX_ADDR_P0_reg);
  flush_TX_RX();
  
  digitalWrite(CE_pin, HIGH);
}

void setup()
{
  unsigned char val = 0;
  
  pinMode(IRQ_pin, INPUT_PULLUP);
  pinMode(MISO_pin, INPUT);
  pinMode(CE_pin, OUTPUT);
  pinMode(SS_pin, OUTPUT);
  pinMode(SCK_pin, OUTPUT);
  pinMode(MOSI_pin, OUTPUT);

  digitalWrite(CE_pin, LOW);
  digitalWrite(SS_pin, LOW);
  digitalWrite(SCK_pin, LOW);
  digitalWrite(MOSI_pin, LOW);
  
  Serial.begin(9600);
  Serial.flush();
  
  nrF24L01_init();
  
  delay(900);
  
  val = register_read(SETUP_RETR_reg);
  Serial.print("SETUP_RETR: 0x");
  Serial.println(val, HEX);
  
  val = register_read(SETUP_AW_reg);
  Serial.print("SETUP_AW: 0x");
  Serial.println(val, HEX);
  
  val = register_read(RF_SETUP_reg);
  Serial.print("RF_SETUP: 0x");
  Serial.println(val, HEX);
  
  val = register_read(RF_CH_reg);
  Serial.print("RF_CH: 0x");
  Serial.println(val, HEX);
  
  val = register_read(EN_AA_reg);
  Serial.print("EN_AA: 0x");
  Serial.println(val, HEX);
  
  val = register_read(CONFIG_reg);
  Serial.print("CONFIG: 0x");
  Serial.println(val, HEX);
}


void loop()
{
  if(get_Status_Reg() == 0x40)
  {
    receive_data(1, &x);
    Serial.println(x);
  }
}

