#include <TimerOne.h>

#define SAMPLE_PERIOD 100

#define LEVEL_THRESHOLD 4

const int sensorPin = A0;

int state = 0;
int vertexValue = 0;

void timerInterrupt() {
  int sensorValue = analogRead(sensorPin);

  if (abs(sensorValue - vertexValue) >= LEVEL_THRESHOLD) {
    state = sensorValue < vertexValue;
    
    vertexValue = sensorValue;
  }

  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.println(vertexValue);
}

void setup() {
  Serial.begin(9600);
  
  Timer1.initialize(SAMPLE_PERIOD);
  Timer1.attachInterrupt(timerInterrupt);
}

void loop() {
}
