#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "skeleton.h"

#define F_CPU 16000000

const short LOWPOW = 0;
const short OPENED = 1;
const short CLOSE  = 2;
const short CLOSED = 3;
const short OPEN   = 4;

const float Threshold = 2.5;

// the setup routine runs once when you press reset
void setup() {
   Serial.begin(9600); // init serial comm's @ 9600 bps
   pinMode(13,OUTPUT);
}



void changeState(int state) {
   STATE = state;
}


void run() {
    int sensorValue = analogRead(A0); // read input on analog pin 0
   
   // Convert the analog reading
   //(which goes from 0 - 1023) to a voltage (0 - 5V)
   float voltage = sensorValue * (5.0 / 1023.0);
   
   // print out the value you read
   if (State = 0 && voltage >= Threshold) {
      State = 1;
      //hold = !hold;
      digitalWrite(13,HIGH);
   }
   else if (State = 0 && voltage < Threshold) {
      State = 0;
   }
   
   if (State = 1 && voltage >= Threshold) {
      State = 0;
      //hold = !hold;
      digitalWrite(13,LOW);
   }
   else if (State = 1 && voltage < Threshold) {
      State = 1;
   }
   //Serial.println(hold);
   delay(100);
}

int main(int argc, char **argv) {
   int hold = 0;
   STATE = 0;

   setup();
   run();
   
   return 0;
}
