const float Threshold = 2.5;
#define TIME_TO_OPEN 1000
#define TIME_TO_CLOSE 1000
#define TIME_STEP 100

enum state {
   LOWPOW,		// low power state
   OPEN,		// opened hand, wait for EMG signal
   CLOSED,		// closed hand, wait for EMG signal
   OPENING,		// opening hand, accept no input
   CLOSING		// closing hand, accept no input
};

enum state currentState;
enum state nextState;

int openingDuration;
int closingDuration;
bool isObstructed;

void setup();
void run();
void advanceState();
void maintainState();
float readSensorVoltage();
void printInputVoltageHigh();
void printInputVoltageLow();