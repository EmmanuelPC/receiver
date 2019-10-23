#include <SoftwareSerial.h>

SoftwareSerial GSerial(3,2,false);
char rec=0;

void setup() {
  pinMode(3, INPUT);
  Serial.begin(9600);
  GSerial.begin(400);
}

void loop() {
  int val = analogRead(3);
  Serial.println(val);
  delay(100);
  if(GSerial.available() != 0) {    
    rec = GSerial.read();
    Serial.print(rec);
  }
}
