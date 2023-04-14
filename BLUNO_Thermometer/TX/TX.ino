#define statusLED        13
#define samples          10


unsigned int t = 0.0;


unsigned int adc_avg()
{
  unsigned char s = samples;
  unsigned int avg = 0;
  
  while(s > 0)
  {
    avg += analogRead(0);
    delay(10);
    s -= 1;
  };
  
  avg /= samples;
  
  return avg;
}


void setup()
{
  Serial.begin(9600);
  Serial.flush();
  analogReference(INTERNAL);
  pinMode(statusLED, OUTPUT);
  digitalWrite(statusLED, LOW);
}


void loop()
{
  t = adc_avg();

  Serial.println(t);
  
  digitalWrite(statusLED, HIGH);
  delay(450);
  digitalWrite(statusLED, LOW);
  delay(450);
}
