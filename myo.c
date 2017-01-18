#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "skeleton.h"

#define F_CPU 16000000

const float Threshold = 2.5;

int main(int argc, char **argv) {
   int hold = 0;

   setup();

   for (;;) {
      run();
      delay(100);
      currentState = nextState;
   }
   
   return 0;
}

// the setup routine runs once when you press reset
void setup() {
   Serial.begin(9600); // init serial comm's @ 9600 bps
   pinMode(13,OUTPUT);
}

void run() {
    float sensorValue = readSensorVoltage();

   // advance state if voltage > threshold  
   if (voltage >= Threshold) {
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
         nextState = LowPow;
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
   analogRead(A0);   // read input on analog pin 0
   
   // Convert the analog reading
   //(which goes from 0 - 1023) to a voltage (0 - 5V)
   return sensorValue * (5.0 / 1023.0);
}

void printInputVoltageHigh() {
   digitalWrite(13,HIGH);
}

void printInputVoltageLow() {
   digitalWrite(13,HIGH);
}
