//BTNS
#define BTN1 8
#define BTN2 10
#define TMP 9

//SOL
#define SOL1 6
#define SOL2 7

void setup() {
  //BTNS
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(TMP, OUTPUT);
  digitalWrite(TMP, LOW);

  //SOL
  pinMode(SOL1, OUTPUT);
  pinMode(SOL2, OUTPUT);
}

void loop() {
  if(LOW == digitalRead(BTN1)){
    digitalWrite(SOL1, HIGH);
  }else{
    digitalWrite(SOL1, LOW);
  }
  if(LOW == digitalRead(BTN2)){
    digitalWrite(SOL2, HIGH);
  }else{
    digitalWrite(SOL2, LOW);
  }
}
