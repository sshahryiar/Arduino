#include <Esplora.h>
#include <TFT.h>           
#include <SPI.h>


#define sensitivity           2.164974
#define volts_per_count       0.004875
#define dB_ref                94
#define no_of_samples         100


char tmp[16];

unsigned char h = 0;
unsigned char s = 0;

unsigned int db_value = 0;
unsigned int db_val = 0;
unsigned int db_max = 0;
unsigned int db_valp = 0;
unsigned int db_maxp = 0;

float db_valf = 0;
float db_maxf = 0;


void adc_rms(unsigned int *rms_value, unsigned int *peak_value)
{
    unsigned char samples = no_of_samples;
    unsigned int rms = 0;
    unsigned int peak = 0;
    unsigned long temp = 0;

    while(samples > 0)
    {
        rms = Esplora.readMicrophone();
        
        if(rms > peak)
        {
            peak = rms;
        }
        
        temp += ((float)rms * (float)rms);
        samples--;   
    }

    temp /= no_of_samples;
    rms = (unsigned int)sqrt(temp);
    
    if(rms <= 0)
    {
        rms = 1;
    }
    if(peak <= 0)
    {
        peak = 1;
    }

    *rms_value = rms;
    *peak_value = peak;
}


void draw_bar(unsigned char x_pos, unsigned char y_pos, unsigned int colour)
{
    EsploraTFT.drawLine(x_pos, 115, x_pos, y_pos, colour);
    EsploraTFT.drawLine((x_pos + 1), 115, (x_pos + 1), y_pos, colour);
}



void setup() 
{
    EsploraTFT.begin();
    EsploraTFT.background(0, 0, 0);
    EsploraTFT.setTextSize(1);
    EsploraTFT.stroke(0, 245, 0);
    EsploraTFT.text("Avg/dB:", 10, 4);
    EsploraTFT.text("Max/dB:", 90, 4);
    EsploraTFT.text("fb.com/MicroArena", 30, 120);
}


void loop() 
{
    adc_rms(&db_val, &db_max);

    db_valf = (db_val * volts_per_count);
    db_maxf = (db_max * volts_per_count);

    db_val = (dB_ref + (20 * (log10(db_valf / sensitivity))));
    db_max = (dB_ref + (20 * (log10(db_maxf / sensitivity))));


    h = map(db_val, 0, 120, 115, 15);

    draw_bar(s, 15, 0x0000);
    draw_bar(s, h, 0xFC00);
    
    h = map(db_max, 0, 120, 115, 15);
    EsploraTFT.drawFastHLine(s, h, 2, 0x00FF);
              
    EsploraTFT.setTextSize(1);
    EsploraTFT.stroke(0, 0, 0);
    itoa(db_valp, tmp, 10);
    EsploraTFT.text(tmp, 60, 4);
    itoa(db_maxp, tmp, 10);
    EsploraTFT.text(tmp, 140, 4);    
    
    EsploraTFT.setTextSize(1);
    EsploraTFT.stroke(0, 245, 0);
    itoa(db_val, tmp, 10);
    EsploraTFT.text(tmp, 60, 4);
    itoa(db_max, tmp, 10);
    EsploraTFT.text(tmp, 140, 4);

    db_valp = db_val;
    db_maxp = db_max;

    s++;
    if(s >= 159)
    {
        s = 0;
    }
    delay(100);
}
