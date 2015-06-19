#include "avr/interrupt.h"
#include <LiquidCrystal.h>

const uint8_t frontSensor = 5, rearSensor = 6;
const uint8_t D5 = 0b00100000, D6 = 0b01000000;
uint8_t sensorID = 0, distance = 0, fprintable = 0, rprintable = 0, loc = 0;
volatile uint16_t rintr = 0, rintf = 0, rvalue = 0;
volatile uint16_t fintr = 0, fintf = 0, fvalue = 0;
volatile uint16_t delta = 0;
volatile uint8_t rctr = 0, fctr = 0;
volatile uint8_t lastPCINT2state=0, thisPCINT2state=0;
uint8_t F1 = 0, F2 = 0, F3 = 0, F4 = 0;
uint8_t R1 = 0, R2 = 0, R3 = 0, R4 = 0;

LiquidCrystal lcd(12, 11, 7, 8, 9, 10);

uint16_t tempSum = 0, tempIn[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t tempPin = 1, tmpArrayCurrent = 0;
float temp = 0.0;

uint16_t deltaT = 0, deltaF = 0, deltaR = 0;

void setup() {
	pinMode(frontSensor, INPUT);
	pinMode(rearSensor, INPUT);
	analogReference(INTERNAL); //change reference to 1.1V instead of 5V
	cli();
	PCICR = PCICR | 0b100;
	PCMSK2 = PCMSK2 | 0b01100000;	//Interrupts on PCINT 21, and 22
	sei();
	lcd.begin(20, 4);
}

void loop() {
	if(fprintable) 	{
		distance = fvalue & 0xFF;
		sensorID = (fvalue>>8) & 0x07;
		fprintable = 0;
		if(sensorID == 0) {
			loc = 1;
		}
		else if(sensorID == 1) {
			loc = 6;
		}
		else if(sensorID == 2) {
			loc = 11;
		}
		else if(sensorID == 3) {
			loc = 16;
		}
		lcd.setCursor(loc, 1);  lcd.print("   ");
		lcd.setCursor(loc, 1);  lcd.print(distance);
		deltaF = millis();
	}
	if(rprintable) {
		distance = rvalue & 0xFF;
		sensorID = (rvalue>>8) & 0x07;
		rprintable = 0;
		if(sensorID == 0) {
			loc = 1;
		}
		else if(sensorID == 1) {
			loc = 6;
		}
		else if(sensorID == 2) {
			loc = 11;
		}
		else if(sensorID == 3) {
			loc = 16;
		}
		lcd.setCursor(loc, 2);  lcd.print("   ");
		lcd.setCursor(loc, 2);  lcd.print(distance);
		deltaR = millis();
	}
	if((millis() - deltaT) >= 2500) {
		deltaT = millis();
		temperatureRead();
		lcd.setCursor(14, 0);
		lcd.print(temp, 1); lcd.write(223); lcd.print("C");
	}
}

void temperatureRead() {
	tempSum = tempSum - tempIn[tmpArrayCurrent];
	tempIn[tmpArrayCurrent] = analogRead(tempPin);
	tempSum = tempSum + tempIn[tmpArrayCurrent];
	if(tmpArrayCurrent == 9) {
		tmpArrayCurrent = 0;
	}
	else {
		tmpArrayCurrent = tmpArrayCurrent + 1;		
	}
	temp = tempSum * 11.0 / (1023);
}

ISR(PCINT2_vect) {
	thisPCINT2state = PIND;
	if((thisPCINT2state ^ lastPCINT2state) & D5) {
		if (fctr == 16) {
			fctr = 0;
			fprintable = 1;
		}
		else if (PIND & D5)        // pin 5 direct port access {
			fintr = micros();
		}
		else {
			fintf = micros();
			delta = fintf - fintr;
			if (delta <150) {
			  fvalue = fvalue << 1;
			  fctr = fctr + 1;
			}
			else if (delta < 300) {
			  fvalue = fvalue << 1;
			  fvalue = fvalue | 1;
			  fctr = fctr + 1;
			}     
		}    
	}
	else if((thisPCINT2state ^ lastPCINT2state) & D6) {
		if (rctr == 16) {
			rctr = 0;
			rprintable = 1;
		}
		else if (PIND & D6)        // pin 6 direct port access {
			rintr = micros();
		}
		else {
			rintf = micros();
			delta = rintf - rintr;
			if (delta <150) {
				rvalue = rvalue << 1;
				rctr = rctr + 1;
			}
			else if (delta < 300) {
				rvalue = rvalue << 1;
				rvalue = rvalue | 1;
				rctr = rctr + 1;
			}
		}	
	}
	lastPCINT2state = thisPCINT2state;
}
