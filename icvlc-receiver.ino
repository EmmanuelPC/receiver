#include <TimerOne.h>

const int ledPin = 2;

int state = 0;

void timerInterrupt() {
  if (state == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  state = !state;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Timer1.initialize(500000);
  Timer1.attachInterrupt(timerInterrupt);
}

void loop() {
}
