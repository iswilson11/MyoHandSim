#include <stdint.h>

const float Threshold = 2.5;

#define TIME_TO_OPEN 1000
#define TIME_TO_CLOSE 1000
#define TIME_STEP 100
#define INIT_STATE LOWPOW

typedef enum {
   LOWPOW,		// low power state
   OPEN,		// opened hand, wait for EMG signal
   CLOSED,		// closed hand, wait for EMG signal
   OPENING,		// opening hand, accept no input
   CLOSING		// closing hand, accept no input
} state;

state currentState;
state nextState;
int ADC_Status;

int openingDuration;
int closingDuration;
int isObstructed;

void setup();
void initADC();
void run();
void advanceState(float voltage);
void maintainState();
float readSensorVoltage();
uint8_t ReadADC(uint8_t ADCchannel);
void printInputVoltageHigh();
void printInputVoltageLow();
void getStateString(char stateString[8], state current);
