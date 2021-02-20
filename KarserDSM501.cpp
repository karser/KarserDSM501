#include "KarserDSM501.h"

KarserDSM501::KarserDSM501(int pin, userfunc interruptDispatch) {
  _pin = pin;
  _interruptDispatch = interruptDispatch;
  _bootMillis = millis();
  _lastReadPmMillis = micros();
  pinMode(_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_pin), _interruptDispatch, CHANGE);
}

bool KarserDSM501::isReady() {
  return millis() - _bootMillis > DSM501_WARMUP_TIME;
}

int KarserDSM501::getReadyCountdown() {
  return (int) ( (DSM501_WARMUP_TIME - millis() - _bootMillis) / 1000 );
}

void KarserDSM501::handleInterrupt() {
  byte state = digitalRead(_pin);

  // On rising edge: report pulse length.
  if (_lastState == LOW && state == HIGH) {
    unsigned long lowPulseMicros = micros() - _lastMicros;
    _lowPulseTotalMicros += lowPulseMicros;
    _lastState = 1;
  }
  // On falling edge: record current time.
  if (_lastState == HIGH && state == LOW){
    _lastMicros = micros();
    _lastState = 0;
  }
}

float KarserDSM501::readPM() {
  if (!isReady()) {
    return 0;
  }
  unsigned long interval = millis() - _lastReadPmMillis;
  float ratio = _lowPulseTotalMicros / (interval * 10.0);
  float concentration = 1.1 * pow( ratio, 3) - 3.8 *pow(ratio, 2) + 520 * ratio + 0.62;
  if ( interval < 3600000 ) { concentration = concentration * ( interval / 3600000.0 ); }

  _lowPulseTotalMicros = 0;
  _lastReadPmMillis = millis();
 
  return concentration;
}

