/*

  Binary counter [0-255]
  This program uses a bunch of LEDs to count from 0 upto 255 in binary
  255 because we're using only 8 pins and 2^8 - 1 = 255
  You can easily extend this program and use more LEDs and pins to count larger numbers
  The best item to use would be an LED bargraph
  
  This program avoid the usage of delay (since I've been told using it is bad).
  
  
*/

const int ledArray[] = { 13, 12, 11, 10, 9, 8, 7, 6};  // This is the variable where you declare the LED pin-out in the order you're connecting it
byte ledData=0;                                        // This is the actual number that's going to be displayed. Byte is used since only 8 LEDs are used. After 255 it overflows back to 0.
unsigned int timerRef, timerNow;                       // These are the time tracking variables. For better understanding check out arduino.cc's BlinkWithoutDelay
const unsigned int waitms = 5000;                       // This is the delay in milliseconds
int diffms;                                            // Time difference in milliseconds
int ledNo = 8;                                         // Total number of LEDs being used

void setup()
{
  // Here we initialize all the LED pins to output and start our 'clock' timer
  for(int i=0; i < ledNo; i++)
  {
    pinMode(ledArray[i], OUTPUT);
  }
  timerRef = millis();
  
}

void loop()
{
  timerNow = millis();
  diffms = timerNow - timerRef;
  diffms = abs(diffms);
  /*
    The Loop() runs as fast as possible, and at every point that it runs I check if enough time has elapsed between the previous
    execution of the if statement below using the handy millis(). The if statement below gets tested all the time, but since most
    of the time it's false, the program simply continues without processing statements inside the if-block
  */
  if(diffms >= waitms)
  {
    setLED(ledData);
    ledData = ledData + 1;
    timerRef = millis();
  }
}

/* 
  Since I'm not dependent on previous hardware states I thought that writing this out as a separate function makes it look neater
  This function basically right-shifts the number one bit at a time and directly writes the bit value to digitalWrite.
*/

void setLED(byte number)
{
  int  bitOutput;
  for(int i = 0; i < 8; i++) //Although I've directly used the number '8' the more correct way would be to use sizeof(number) in case you want a generic solution
  {
    bitOutput = number & 1;
    digitalWrite(ledArray[i], bitOutput);
    number=number>>1;
  }
}

