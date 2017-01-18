enum state {
   LOWPOW,		// low power state
   OPEN,		// opened hand, wait for EMG signal
   CLOSED,		// closed hand, wait for EMG signal
   OPENING,		// opening hand, accept no input
   CLOSING		// closing hand, accept no input
}

enum state currentState;
enum state nextState;