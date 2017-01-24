#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "myo.h"


int main(int argc, char **argv) {
   //int hold = 0;

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
   DDRB |= (1 << PB5); // pinMode(13,OUTPUT); // PORTB5 = output
   DDRC &= ~(1 << PC0); // Sets A0 for input
}

void InitADC()
{
    // Select Vref=AVcc
    ADMUX |= (1<<REFS0);
    //set prescaller to 128 and enable ADC
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

void run() {
   float sensorValue = readSensorVoltage();
   
   advanceState(sensorValue);
}

void advanceState(float voltage) {
   switch(currentState) {
      case LOWPOW:
         // remain in low power state until interrupted
         nextState = LOWPOW;
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
   //float sensorValue = 

   // Convert the analog reading
   //(which goes from 0 - 1023) to a voltage (0 - 5V)
   return sensorValue * (5.0 / 1023.0);
}

uint16_t ReadADC(uint8_t ADCchannel)
{
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
    //single conversion mode
    ADCSRA |= (1<<ADSC);
    // wait until ADC conversion is complete
    while( ADCSRA & (1<<ADSC) );
    return ADC;
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
