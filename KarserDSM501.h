#include "Arduino.h"

#define DSM501_WARMUP_TIME 60000

typedef void (*userfunc)();

class KarserDSM501 {
  public:
    KarserDSM501(int pin, userfunc interruptDispatch);
    void handleInterrupt();
    float readPM();
    bool isReady();
    int getReadyCountdown();
  private:
    int _pin;
    userfunc _interruptDispatch;
    byte _lastState;
    unsigned long _bootMillis;
    unsigned long _lastMicros;
    unsigned long _lowPulseTotalMicros;
    unsigned long _lastReadPmMillis;
};

