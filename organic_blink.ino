const uint8_t LEDpin = 9;
const uint8_t LEDlookup[] = { 0, 1, 2, 3, 4, 5, 7, 9, 12, 15, 18, 22, 27, 32, 38, 44, 51, 58, 67, 76, 86, 96, 108, 120, 134, 148, 163, 180, 197, 216, 235, 255 };
const uint16_t fadeTime = 75;
int i = 0;

void setup()
{
  pinMode(LEDpin, OUTPUT);
}

void loop()
{
  for(i=0; i<32; i++)
  {
    analogWrite(LEDpin, LEDlookup[i]);
    delay(fadeTime);
  }
  for(i=31; i>=0; i--)
  {
    analogWrite(LEDpin, LEDlookup[i]);
    delay(fadeTime);
  }
}
