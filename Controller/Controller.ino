//#include <SoftwareSerial.h>



//serial LCD codes
#define CTRL1 0xFE
#define CTRL2 0x7C

#define CLEAR 0x01

#define SETCURS 0x80
#define LINE1 SETCURS
#define LINE2 SETCURS+64
#define LINE3 SETCURS+20
#define LINE4 SETCURS+84
//end serial LCD codes

//knob encoder
#define KNOB_BTN 4  
#define KNOB_ENCA 2 
#define KNOB_ENCB 3 

#define KNOB_RED A9
#define KNOB_GRN A11
#define KNOB_BLU A10

volatile int knobCount = 0;

//buttons
#define SAFTY 24
#define SAFTY_LED 25
#define UP 40
#define UP_LED 41
#define DWN 36
#define DWN_LED 37
#define RELEASE 28
#define RELEASE_LED 29
#define FORCE 32
#define FORCE_LED 33

//timing  in ms
#define time_t unsigned long
#define LCD_UPDATE_PERIOD 1000
#define BUTTONS_PERIOD 100



#define MAX_LINE_LEN 30

//LCD on pin 18
#define LCD_BOOT_TIME 500
#define LCD Serial1
#define CART Serial3


void setupKnob(){
  pinMode(KNOB_BTN, INPUT);
  
  pinMode(KNOB_RED, OUTPUT);
  pinMode(KNOB_GRN, OUTPUT);
  pinMode(KNOB_BLU, OUTPUT);

  digitalWrite(KNOB_RED, HIGH);
  digitalWrite(KNOB_GRN, HIGH);
  digitalWrite(KNOB_BLU, HIGH);
  
}

void knobISR(){ 
  if(!digitalRead(KNOB_ENCA)){  //to prevent double positive counts ISR triggered on B falling if A was high (causing double count)
    if(!digitalRead(KNOB_ENCB)){
      knobCount++;
    }else{
      knobCount--;
    }
  }
}

void setup()
{
  LCD.begin(9600);
  LCD.flush();
  
  CART.begin(9600);
  CART.flush();

  //debug
  Serial.begin(9600);
  Serial.flush();
  
  //buttons
  pinMode(SAFTY, INPUT_PULLUP);
  pinMode(SAFTY_LED, OUTPUT);
  digitalWrite(SAFTY_LED, LOW);
    
  pinMode(UP, INPUT_PULLUP);
  pinMode(UP_LED, OUTPUT);
  digitalWrite(UP_LED, LOW);
  
  pinMode(DWN, INPUT_PULLUP);
  pinMode(DWN_LED, OUTPUT);
  digitalWrite(DWN_LED, LOW);
  
  pinMode(RELEASE, INPUT_PULLUP);
  pinMode(RELEASE_LED, OUTPUT);
  digitalWrite(RELEASE_LED, LOW);
  
  pinMode(FORCE, INPUT_PULLUP);
  pinMode(FORCE_LED, OUTPUT);
  digitalWrite(FORCE_LED, LOW);
  
  // wait for display to boot up
  delay(500);
  LCD.write(CTRL1);
  LCD.write(CLEAR);

  setupKnob();
  //enable ISR for knob encoder
  
}

void writeToLCD(char* line){
  //to skip over leading D
  line++;
  //clear lcd line
  LCD.write(CTRL1);
  LCD.write(LINE3);
  LCD.write("                    ");
  LCD.write(CTRL1);
  LCD.write(LINE3);
  LCD.write("Force: ");
  while(*line != ','){
    LCD.write(*line++);
  }
  line++;
  LCD.write(" kg");
  
  //clear lcd line
  LCD.write(CTRL1);
  LCD.write(LINE4);
  LCD.write("                    ");
  LCD.write(CTRL1);
  LCD.write(LINE4);
  LCD.write("Gauge: ");
  while(*line != '\0'){
    LCD.write(*line++);
  }
  LCD.write(" Thou");
}

int check_line(char* line, int len){
  if(line[0] != 'D' || line[len-1] != '\0'){
    return 0;
  }
  return 1;
}

void readCartData(){ 
  static char line[MAX_LINE_LEN];
  static char len = 0;

  if(0 < CART.available()){
    line[len++] = CART.read();
  }
  if(line[len-1] == '\n' || len >= MAX_LINE_LEN){
    line[len-1]='\0';
    if(check_line(line, len)){
      //Serial.print("valid -- ");
      //Serial.println(line);
      writeToLCD(line);
      //writeToFile(line);
    }
    //reset
    len = 0;
  }
}

void checkPistonButtons(){
  //vertical piston
  if(!digitalRead(UP) && digitalRead(DWN)){
    digitalWrite(UP_LED, LOW);
    CART.write("R0");
  }else if(digitalRead(UP) && !digitalRead(DWN)){
    digitalWrite(DWN_LED, LOW);
    CART.write("R1");
  }else{
    digitalWrite(UP_LED, HIGH);
    digitalWrite(DWN_LED, HIGH);
  }

  //force piston
  if(!digitalRead(RELEASE) && digitalRead(FORCE)){
    CART.write("F0");
    digitalWrite(RELEASE_LED, LOW);
  }else if(digitalRead(RELEASE) && !digitalRead(FORCE)){
    CART.write("F1");
    digitalWrite(FORCE_LED, LOW);
  }else{
    digitalWrite(RELEASE_LED, HIGH);
    digitalWrite(FORCE_LED, HIGH);
  }
}

void sendButtons(){
  static time_t nextUpdate=LCD_BOOT_TIME;
   
  if(millis() > nextUpdate ) {
    if(digitalRead(SAFTY)){
      //safty button not pressed
      digitalWrite(SAFTY_LED, HIGH);
      digitalWrite(UP_LED, LOW);
      digitalWrite(DWN_LED, LOW);
      digitalWrite(RELEASE_LED, LOW);
      digitalWrite(FORCE_LED, LOW);
    }else{
      //safty button pressed
      digitalWrite(SAFTY_LED, LOW);
      checkPistonButtons();
    }
    nextUpdate=millis()+BUTTONS_PERIOD;
  }
}

void updateLCD(){
  static time_t nextUpdate=0;  

  if(millis() > nextUpdate ) {
  
    LCD.write(CTRL1);
    LCD.write(LINE1);
    LCD.write("Sensor Cart     ");
    LCD.print(millis()/1000);
    //LCD.write(CTRL1);
    //LCD.write(LINE2);
    //LCD.write("File: test.csv");
        
    nextUpdate = millis() + LCD_UPDATE_PERIOD;
  }
}

void loop(){  
  sendButtons();
  
  updateLCD();
  readCartData();
}
