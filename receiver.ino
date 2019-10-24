#include <TimerOne.h>
#include "Manchester.h"

#define SYMBOL_PERIOD 3000
#define SAMPLE_PERIOD 100
#define SAMPLE_PER_SYMBOL (SYMBOL_PERIOD / SAMPLE_PERIOD)
#define DOUBLE_SYMBOL_THRESHOLD 1.2
#define NUM_SYMBOLS 20
#define LEVEL_THRESHOLD 5

#define SYN 0x04
#define STX 0x02
#define ETX 0x03

#define SENSOR_PIN A0
#define LED_PIN 2

int state = 0, steadyCounter = 0;
int vertexValue = 0;

unsigned long symbols = 0;
int symbolCounter;

enum ParsingState {
  Idle,
  Started,
};

ParsingState ps = Idle;

void pushSymbol(int symbol) {
  char ch;

  symbols = (symbols << 1) | state;
  symbolCounter++;
  if (symbolCounter >= NUM_SYMBOLS) {
    if (ps == Idle) {
      if (checkWord(symbols) && decodeWord(symbols, &ch)) {
        if (ch == SYN) {
          digitalWrite(LED_PIN, HIGH);
          symbolCounter = 0;
        } else if (ch == STX) {
          ps = Started;
          symbolCounter = 0;
        } else {
          digitalWrite(LED_PIN, LOW);
        }
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    } else {
      if (checkWord(symbols) && decodeWord(symbols, &ch)) {
        if (ch == ETX) {
          ps = Idle;
          symbolCounter = 0;
          Serial.println();
        } else {
          symbolCounter = 0;
          Serial.print(ch);
        }
      } else {
        ps = Idle;
        Serial.println();
      }
    }
  }
}

void timerInterrupt() {
  int sensorValue = analogRead(SENSOR_PIN);

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
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);

  Timer1.initialize(SAMPLE_PERIOD);
  Timer1.attachInterrupt(timerInterrupt);
}

void loop() {
}
