#include <TimerOne.h>
#include "Manchester.h"

#define SYMBOL_PERIOD 3000
#define SAMPLE_PERIOD 100
#define SAMPLE_PER_SYMBOL (SYMBOL_PERIOD / SAMPLE_PERIOD)
#define DOUBLE_SYMBOL_THRESHOLD 1.2
#define NUM_SYMBOLS 20
#define LEVEL_THRESHOLD 4

const int sensorPin = A0;

int state = 0, steadyCounter = 0;
int vertexValue = 0;

unsigned long symbols = 0;
int symbolCounter;

inline void pushSymbol(int symbol) {
  symbols = (symbols << 1) | state;
  symbolCounter++;
  if (symbolCounter >= NUM_SYMBOLS) {
    if (checkWord(symbols)) {
      char ch;

      if (decodeWord(symbols, &ch)) {
        if (isAlpha(ch)) {
          Serial.print(ch);
          symbolCounter = 0;
        }
      }
    }
  }
}

void timerInterrupt() {
  int sensorValue = analogRead(sensorPin);

  if (abs(sensorValue - vertexValue) >= LEVEL_THRESHOLD) {
    state = sensorValue < vertexValue;

    pushSymbol(state);

    vertexValue = sensorValue;
    steadyCounter = 0;
  } else {
    if ((!state && sensorValue > vertexValue) || (state && sensorValue < vertexValue)) {
      vertexValue = sensorValue;
    }

    steadyCounter++;

    if (steadyCounter >= SAMPLE_PER_SYMBOL * DOUBLE_SYMBOL_THRESHOLD) {
      pushSymbol(state);
      steadyCounter = 0;
    }
  }
}

void setup() {
  Serial.begin(9600);

  Timer1.initialize(SAMPLE_PERIOD);
  Timer1.attachInterrupt(timerInterrupt);
}

void loop() {
}