unsigned char ledPins[36] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
unsigned char pwmPins[6] = {8, 9, 10, 11, 12, 13};
unsigned char s = 0;
unsigned char n = 0;
unsigned char x = 0;
unsigned char dc = 0;


void columnsOff()
{
    for(s = 0; s < 36; s++)
    {
        digitalWrite(ledPins[s], LOW);        
    }
}


void columnsOn()
{
    for(s = 0; s < 36; s++)
    {
        digitalWrite(ledPins[s], HIGH);        
    }
}


void rowsOff()
{
    for(n = 0; n < 6; n++)
    {
        digitalWrite(pwmPins[n], LOW);        
    }
}


void rowsOn()
{
    for(n = 0; n < 6; n++)
    {
        digitalWrite(pwmPins[n], HIGH);        
    }
}


void allOff()
{
    columnsOff();
    rowsOff();
}


void allOn()
{
    columnsOn();
    rowsOn();
}


void randomLED()
{
    for(x = 0; x < 10; x++)
    {
        allOff();
        randomSeed(analogRead(0));
        s = random(0, 36);
        randomSeed(analogRead(0));
        n = random(0, 6);
        digitalWrite(ledPins[s], HIGH);
        digitalWrite(pwmPins[n], HIGH);
        delay(200);
    }
}


void randomColumn()
{
    for(x = 0; x < 10; x++)
    {
        allOff();
        rowsOn();
        randomSeed(analogRead(0));
        s = random(0, 36);
        digitalWrite(ledPins[s], HIGH);
        delay(200);
    }
}


void randomRow()
{
    for(x = 0; x < 10; x++)
    {
        allOff();
        columnsOn();
        randomSeed(analogRead(0));
        n = random(0, 6);
        digitalWrite(pwmPins[n], HIGH);
        delay(300);
    }
}


void totallyRandom()
{
    for(x = 0; x < 10; x++)
    {
        allOff();
        randomSeed(analogRead(0));
        s = random(0, 36);
        randomSeed(analogRead(1));
        n = random(0, 6);
        randomSeed(analogRead(2));
        dc = random(0, 256);
        digitalWrite(ledPins[s], HIGH);
        analogWrite(pwmPins[n], dc);
        delay(400);
    }
}


void strobe()
{
    for(x = 0; x < 6; x++)
    {
        allOn();
        delay(200);
        allOff();
        delay(50);
    }
}


void flash()
{
   for(x = 0; x < 4; x++)
   {
        allOn();
        delay(20);
        allOff();
        delay(100);
   }
}


void setup()
{
    for(s = 0; s < 36; s++)
    {
        pinMode(ledPins[s], OUTPUT);
        digitalWrite(ledPins[s], LOW);        
    }
    for(n = 0; n < 6; n++)
    {
        pinMode(pwmPins[n], OUTPUT);
        digitalWrite(pwmPins[n], LOW);
    }
}


void loop()
{
    randomLED();
    allOff();
    randomColumn();
    allOff(); 
    flash();
    allOff(); 
    totallyRandom();
    allOff();  
    strobe();
    allOff();
    randomRow();
    allOff();
}
