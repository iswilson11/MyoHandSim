/*
   Modified Code for Toggling MyoElectric Hand
   FFD Engineering 2016/2017

   Updated: 1/10/17
   ---------------------------------------------------------------------------
   ReadAnalogVoltage
   Reads an analog input on pin 0, converts it to voltage, and prints the 
      result to the serial monitor.
   Graphical representation is available using serial plotter 
      (Tools > Serial Plotter menu)
   Attach the center pin of a potentiometer to pin A0, 
      and the outside pins to +5V and ground.

   This example code is in the public domain.
*/



// Global Variable Definition
const int Threshold = 2.5;
int State = 0;
int hold = 0;

// the setup routine runs once when you press reset:
void setup() {
   Serial.begin(9600); // initialize serial communication @ 9600 bps
   pinMode(13,OUTPUT);
}

// the loop routine runs over and over again forever
void loop() {
   int sensorValue = analogRead(A0); // read the input on analog pin 0

   // Convert the analog reading 
   //(which goes from 0 - 1023) to a voltage (0 - 5V)
   float voltage = sensorValue * (5.0 / 1023.0);

   // print out the value you read
   if (State = 0 && voltage >= Threshold) {
      State = 1;
      hold = !hold;
      digitalWrite(13,HIGH);
   }
   else if (State = 0 && voltage < Threshold) {
      State = 0;
   }
   if (State = 1 && voltage >= Threshold) {
      State = 0;
      hold = !hold;
      digitalWrite(13,LOW);
   }
   else if (State = 1 && voltage < Threshold) {
      State = 1;
   }
   Serial.println(hold);
   delay(100);
}


