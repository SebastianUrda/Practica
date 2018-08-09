#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <SoftwareSerial.h>

AccelStepper myStepperX(AccelStepper::FULL4WIRE, 8,9,10,11);
AccelStepper myStepperY(AccelStepper::FULL4WIRE,   4,5,6,7);

int TabelCoordonate[100][6]; //DEfinim tabel de valori
int row,col;
int i=0,j=0;
int state=0;//0-calibrate 1-fill matrix 2- print matrix  3- draw
int l=0;
const int buttonPin = 2;
int buttonState = 0;
int r=0;
int integerValue=0;  // Max value is 65535
char incomingByte;
String inString = "";
MultiStepper steppers;
int n=0;
bool nRead=false;
ISR(INT4_vect)
{
	//external interrupt for button for next coordinate
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	//debouncer strategy
	if (interrupt_time - last_interrupt_time > 200)
	{
		if(r<n)
		r++;
		else r=0;
		Serial.print(r);
	}
	last_interrupt_time = interrupt_time;
}
ISR(INT5_vect)
{
    //external interrupt for button for previous coordinate
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
   //debouncer strategy
	if (interrupt_time - last_interrupt_time > 200)
	{
		if(r>0)
		r--;
		else r=n;
		Serial.print(r);
	}
	last_interrupt_time = interrupt_time;
}

void calibrate()
{
	myStepperY.setMaxSpeed(100);//Make motors move to blocks
	myStepperX.setMaxSpeed(100);
	steppers.addStepper(myStepperY);
	steppers.addStepper(myStepperX);
	long positions[2];
	long positions1[2];
	positions[0] = -360;
	positions[1] = 360;
	steppers.moveTo(positions);
	steppers.runSpeedToPosition();
	delay(100);
	myStepperY.setCurrentPosition(0);
	myStepperX.setCurrentPosition(0);
	//turn motors until laser point is visible
	positions1[0] = 45;
	positions1[1] = -45;
	steppers.moveTo(positions1);
	steppers.runSpeedToPosition();
	delay(100);
	myStepperY.setCurrentPosition(0);
	myStepperX.setCurrentPosition(0);
	Serial.println("DONE!");
	//turn motors at high speed
	myStepperX.setSpeed(9999);
	myStepperY.setSpeed(9999);
	state=1;
}
void citire()
{
	while (Serial.available() > 0) {
		int inChar = Serial.read();
		inString += (char)inChar;
		if (inChar == '\n') {
			Serial.print("Value:");
			integerValue = inString.toInt();
			Serial.println(integerValue);
			inString = "";
			if(!nRead)
			{
				n=integerValue;
				nRead=true;
				Serial.print("n=");
				Serial.print(n);
			}else{
				TabelCoordonate[i][j]=integerValue;
				j++;
				if(j==6)
				{
					j=0;
					i++;
				}
				if(i==n)
				state=2;
			}
		}
	}
}
void afisare()
{
	Serial.println("Matrix Full");
	for(row=0;row<n;row++){
		for(col=0;col<6;col++){
			Serial.print(TabelCoordonate[row][col]);
			Serial.print(" ");
		}
		Serial.println();
	}
	state=3;
}
void work()
{
	long pos[2];
	for(int k=0;k<=n;k=k+2)
	{
		pos[0] = TabelCoordonate[r][k];
		pos[1] = TabelCoordonate[r][k+1];
		steppers.moveTo(pos);
		steppers.runSpeedToPosition(); // Blocks until all are in position
		digitalWrite(LED_BUILTIN, HIGH);
		delay(50);
		digitalWrite(LED_BUILTIN, LOW);
		delay(50);
	}
}

void loop()
{
	switch(state)
	{
		case 0: calibrate(); break;
		case 1: citire(); break;
		case 2: afisare(); break;
		case 3: work(); break;
	}
}


