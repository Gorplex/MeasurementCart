
#define KNOB_BTN 4

#define KNOB_RED A9
#define KNOB_GRN A11
#define KNOB_BLU A10

#define KNOB_ENCA 2
#define KNOB_ENCB 3

#define ISR_PIN KNOB_ENCA

volatile int count = 0;

void setupKnob(){
  pinMode(KNOB_BTN, INPUT);
  
  pinMode(KNOB_RED, OUTPUT);
  pinMode(KNOB_GRN, OUTPUT);
  pinMode(KNOB_BLU, OUTPUT);

  digitalWrite(KNOB_RED, HIGH);
  digitalWrite(KNOB_GRN, HIGH);
  digitalWrite(KNOB_BLU, HIGH);

  pinMode(KNOB_ENCA, INPUT_PULLUP);
  pinMode(KNOB_ENCB, INPUT_PULLUP);  
  
}

void EncISR(){ 
  if(!digitalRead(KNOB_ENCA)){  //to prevent double positive counts ISR triggered on B falling if A was high (causing double count)
    if(!digitalRead(KNOB_ENCB)){
      count++;
    }else{
      count--;
    }
  }
}

void setup() {
  Serial.begin(9600);

  setupKnob();

  attachInterrupt(digitalPinToInterrupt(ISR_PIN), EncISR, FALLING);
}

int waitForBtn(){
  return 1;
  while(!digitalRead(KNOB_BTN));
}

void loop(){
  PrintTest();
  
}

int readEnc(){
  static int count = 0;
  static int state = 0;//0 no magnet, 1 magnet
  int readA = digitalRead(KNOB_ENCA);
  int readB = digitalRead(KNOB_ENCB); 
  
  if(!state && !readA){
    state = 1;  //magnet there
    if(!readB){
      count++;
    }else{
      count--;
    }
  }
  if(state && readA){
    state = 0;
  }
  return count;
}

void PrintTest() {
  //int count = readEnc();
  Serial.print("A: ");
  Serial.println(digitalRead(KNOB_ENCA));
  Serial.print("B: ");
  Serial.println(digitalRead(KNOB_ENCB));
  Serial.print("C: ");
  Serial.println(count);
  Serial.print("\n");
  delay(200);
}
