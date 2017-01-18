#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "myo.h"

const float Threshold = 2.5;

int main(int argc, char **argv) {
   //int hold = 0;

   setup();

   for (;;) {
      run();
      _delay_ms(100);
      currentState = nextState;
   }
   
   return 0;
}

// the setup routine runs once when you press reset
void setup() { 
   DDRB |= (1 << PB5); // pinMode(13,OUTPUT); // PORTB5 = output
   DDRC &= (1 << PC0); // Sets A0 for input
}

void run() {
    float sensorValue = readSensorVoltage();

   // advance state if sensorValue (voltage) > threshold  
   if (sensorValue >= Threshold) {
      advanceState();
      printInputVoltageHigh();
   }
   else {
      maintainState();
      printInputVoltageLow();
   }
}

void advanceState() {
   switch(currentState) {
      case LOWPOW:
         nextState = LOWPOW;
      break;
      
      case OPEN:
         nextState = CLOSING;
      break;
      
      case CLOSED:
         nextState = OPENING;
      break;
      
      case OPENING:
         nextState = OPEN;
      break;
      
      case CLOSING:
         nextState = CLOSED;
      break;
   }
}

void maintainState() {
   nextState = currentState;
}

float readSensorVoltage() {
   float sensorValue = analogRead(A0);   // read input on analog pin 0
   //float sensorValue = 

   // Convert the analog reading
   //(which goes from 0 - 1023) to a voltage (0 - 5V)
   return sensorValue * (5.0 / 1023.0);
}

void printInputVoltageHigh() {
   PORTB |= (1<<PB5); // digitalWrite(13,HIGH);
}

void printInputVoltageLow() {
   PORTB &= ~(1<<PB5); //digitalWrite(13,LOW);
}
