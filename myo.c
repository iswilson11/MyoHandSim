/*
   Code for MyoElectric Hand using an ATMEGA328P 
   
   FFD Engineering 2016/2017
      - Kyle Brunnett
	  - Ian Wilson

   Updated: 2/2/17
   ---------------------------------------------------------------------------
   ReadAnalogVoltage
 ? Reads an analog input on pin 0, converts it to voltage, and prints the 
      result to the serial monitor.
 ? Attach the center pin of a potentiometer to pin A0, 
      and the outside pins to +5V and ground.
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "myo.h"

int main(int argc, char **argv) {
   
   setup();

   for (;;) {
      char stateString[8];
      getStateString(stateString, currentState);
      printf("currentState: %s", stateString);

      run();
      _delay_ms(100);

      currentState = nextState;
   }
   
   return 0;
}

// the setup routine runs once when you press reset
void setup() {
   currentState = INIT_STATE; // INIT_STATE defined in .h
   ADC_Status = 0;
   DDRB |= (1 << PB5); // pinMode(13,OUTPUT); // PORTB5 = output
}

void initADC()
{
	uint8_t pin = ANALOG_PIN;
	
	// Checks if valid Analog Pin for atmega328p
	if (ANALOG_PIN <= 7) {
		// Gets pin for ADMUX
		pin  = 0x00 & ANALOG_PIN;
	}
	else {
		exit(INVALID_ANALOG_PIN);
	}
	
	// ADC Settings
    ADMUX |= (1<<REFS0) | // Select Vref=AVcc
	         (1<<ADLAR) | // Left align data
	         pin; // Selects Analog Pin
	
	// Sets ADC Control and Status Register A
    ADCSRA |= (1<<ADEN)  | // Enables ADC
	          (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  // Sets prescaler to 128
}


void run() {
   float sensorValue = readSensorVoltage();
   
   advanceState(sensorValue);
}

void advanceState(float voltage) {
   switch(currentState) {
      case LOWPOW:
         // remain in low power state until interrupted by V = threshold
         if (voltage >= Threshold) {
            if (ADC_Status == 0) {
               initADC(); // Turn on the ADC
               ADC_Status = 1;
            }
	        nextState = OPEN; // (TODO) Fix in case starting pos. isn't open
         }
         else {
	        nextState = LOWPOW; // Stay in Low Power
         }
         
      break;
      
      case OPEN:
         // close hand if threshold reached
         if (voltage >= Threshold) {
            nextState = CLOSING;  
         }
         else {
            nextState = OPEN;
         }
      break;
      
      case CLOSED:
         // open hand if threshold reached
         if (voltage >= Threshold) {
            nextState = OPENING;
         }
         else {
            nextState = CLOSED;
         }
      break;
      
      case OPENING:
         // hand is open after defined time
         if (openingDuration >= TIME_TO_OPEN) {
            nextState = OPEN;
            openingDuration = 0;
         }
         else {
            nextState = OPENING;
            openingDuration += TIME_STEP;
         }
      break;
      
      case CLOSING:
         // hand is closed after defined time
         if (closingDuration >= TIME_TO_CLOSE || isObstructed) {
            nextState = CLOSED;
            openingDuration = 0;
         }
         else {
            nextState = CLOSING;
            closingDuration += TIME_STEP;
         }
      break;
   }
}

float readSensorVoltage() {
   float sensorValue = ReadADC(0);   // read input on analog pin 0

   // Convert the analog reading
   // (which goes from 0 - 255) to a voltage (0 - 5V)
   // Note: Change 255.0 to 1023.0 for 10-bit precision
   return sensorValue * (5.0 / 255.0);
}

uint8_t ReadADC(uint8_t ADCchannel)
{
	unsigned char analogVal;
	
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xE0) | ADCchannel;
	
    //AD Start Conversion in single conversion mode 
	// (?) We might want to use Free Running mode if Free Running doesn't 
	//     affect power consumption
    ADCSRA |= (1<<ADSC);
	
    // wait until ADC conversion is complete
    while( ADCSRA & (1<<ADSC));
	
    // We only need ADCH since ADLAR is set to 1 (ATMEGA328P Datasheet pg 267)
    /* TODO: Sampling just ADCH only gives 8-bit precision.
          Need to test to find out if 10-bit ADC value is needed. If so,
          we need logic for ADCH and ADCL combined.
    */
	analogVal = ADCH;
	
    return analogVal;
}

void printInputVoltageHigh() {
   PORTB |= (1<<PB5); // digitalWrite(13,HIGH);
}

void printInputVoltageLow() {
   PORTB &= ~(1<<PB5); //digitalWrite(13,LOW);
}

void getStateString(char stateString[8], state current) {
   //char stateString[8];
   switch (current) {
      case LOWPOW:
         stateString = "LOWPOW";
         //return "LOWPOW";
           
         break;

      case OPEN:
         stateString = "OPEN";
         //return "OPEN";
         break;
      
      case CLOSED:
         stateString = "CLOSED";
         //return "CLOSED";
         break;
      
      case OPENING:
         stateString = "OPENING";
         //return "OPENING";
         break;
      
      case CLOSING:
         stateString = "CLOSING";
         //return "CLOSING";
         break;
   }
}
