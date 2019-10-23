#include <TimerOne.h>

#define SAMPLE_PERIOD 10000

const int sensorPin = A0;

void timerInterrupt() {
  int sensorValue = analogRead(sensorPin);

  Serial.println(sensorValue);
}

void setup() {
  Serial.begin(9600);
  
  Timer1.initialize(SAMPLE_PERIOD);
  Timer1.attachInterrupt(timerInterrupt);
}

void loop() {
}
