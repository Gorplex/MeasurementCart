//BTNS
#define BTN1 10
#define LED1 12
#define BTN2 8

//SOL
#define SOL1 11
#define SOL2 9

void setup() {
  //BTNS
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(BTN2, INPUT_PULLUP);
  
  //SOL
  pinMode(SOL1, OUTPUT);
  pinMode(SOL2, OUTPUT);
}

void loop() {
  if(LOW == digitalRead(BTN1)){
    digitalWrite(SOL1, HIGH);
    digitalWrite(LED1, LOW);
  }else{
    digitalWrite(SOL1, LOW);
    digitalWrite(LED1, HIGH);
  }
  if(LOW == digitalRead(BTN2)){
    digitalWrite(SOL2, HIGH);
  }else{
    digitalWrite(SOL2, LOW);
  }
}
