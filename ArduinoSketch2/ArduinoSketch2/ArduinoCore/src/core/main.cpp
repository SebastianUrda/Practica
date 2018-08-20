/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }
void initializare()
{Serial.begin(9600);
	//configure external interrupt 4
	DDRD &= ~(1<<DDD2);
	EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (RISING);
	EIMSK |= (1 << INT0);
	//configure external interrupt 5
	DDRD &= ~(1<<DDD3);
	EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (RISING << ISC10);
	EIMSK |= (1 << INT1);
	//configure digital pin 13 as output, the laser pin
	DDRB |= 1<<PINB7;
	PORTB &= ~(1<<PINB7);
	
}

int main(void)
{
	init();

	initVariant();
	initializare();
#if defined(USBCON)
	USBDevice.attach();
#endif
	
	//setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
        
	return 0;
}

