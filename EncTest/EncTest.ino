//using encoder lib found at https://github.com/PaulStoffregen/Encoder
#include "Encoder.h"

#define KNOB_BTN A13

#define KNOB_RED A9
#define KNOB_GRN A11
#define KNOB_BLU A10

#define KNOB_ENCA A15
#define KNOB_ENCB A14

Encoder myEnc(KNOB_ENCA, KNOB_ENCB);

void setupKnob(){
  pinMode(KNOB_BTN, INPUT);
  
  pinMode(KNOB_RED, OUTPUT);
  pinMode(KNOB_GRN, OUTPUT);
  pinMode(KNOB_BLU, OUTPUT);

  digitalWrite(KNOB_RED, HIGH);
  digitalWrite(KNOB_GRN, HIGH);
  digitalWrite(KNOB_BLU, HIGH);

  
  
}

void setup() {
  Serial.begin(9600);

  setupKnob();
}

int waitForBtn(){
  return 1;
  while(!digitalRead(KNOB_BTN));
}

void readEnc(){
  long pos = myEnc.read();
  Serial.println(pos);
  
}

void loop() {
  delay(1000);
  readEnc();
  waitForBtn();
  digitalWrite(KNOB_RED, LOW);
  delay(1000);
  readEnc();
  waitForBtn();
  digitalWrite(KNOB_RED, HIGH);
  digitalWrite(KNOB_GRN, LOW);
  delay(1000);
  readEnc();
  waitForBtn();
  digitalWrite(KNOB_GRN, HIGH);
  digitalWrite(KNOB_BLU, LOW);
  delay(1000);
  readEnc();
  waitForBtn();
  digitalWrite(KNOB_BLU, HIGH);
  
}
