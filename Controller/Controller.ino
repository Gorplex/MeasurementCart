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

//buttons
#define SAFTY 24
#define SAFTY_LED 25
#define UP 27
#define UP_LED 28
#define DWN 29
#define DWN_LED 30
#define RELEASE 31
#define RELEASE_LED 32
#define FORCE 33
#define FORCE_LED 34

//timing  in ms
#define LCD_UPDATE_PERIOD 1000
#define BUTTONS_PERIOD 100


//LCD on pin 18
#define LCD_BOOT_TIME 500
#define LCD Serial1
#define CART Serial3


void setup()
{
  LCD.begin(9600);
  LCD.flush();
  CART.begin(9600);
  CART.flush();
  
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
  
  //delay(500); // wait for display to boot up
}


#define MAX_LINE_LEN 25
void readLine(){
  static char line[MAX_LINE_LEN];
  static char len = 0;

  if(0 < CART.available()){
    line[len++] = CART.read();
  }
  if(line[len] == '\n' || line[len] == '\4' || len >= 25){
    line[len] = '\0';
    len = 0;
    return &line;
  }
}

void readCartData(){  
  int avail;
  char received;
  while(0 < (avail = CART.available())){
    received = CART.read();
    //LCD.write(CTRL1);
    //LCD.write(LINE3);
    if(received == '\n'){
         LCD.write(CTRL1);
         LCD.write(LINE4);
    }else{
      LCD.write(received);
    }  
  }
}

void checkPistonButtons(){
  digitalWrite(UP_LED, HIGH);
  digitalWrite(DWN_LED, HIGH);
  digitalWrite(RELEASE_LED, HIGH);
  digitalWrite(FORCE_LED, HIGH);
  
  //vertical piston
  if(!digitalRead(UP) && digitalRead(DWN)){
    CART.write("R0");
    digitalWrite(UP_LED, LOW);
  }else if(digitalRead(UP) && !digitalRead(DWN)){
    CART.write("R1");
    digitalWrite(DWN_LED, LOW);
  }

  //force piston
  if(!digitalRead(RELEASE) && digitalRead(FORCE)){
    CART.write("F0");
    digitalWrite(RELEASE_LED, LOW);
  }else if(digitalRead(RELEASE) && !digitalRead(FORCE)){
    CART.write("F1");
    digitalWrite(FORCE_LED, LOW);
  }
}

void sendButtons(){
  static int nextUpdate=LCD_BOOT_TIME;
   
  if(millis() > nextUpdate ) {
    if(digitalRead(SAFTY)){
      digitalWrite(SAFTY_LED, HIGH);
      digitalWrite(UP_LED, LOW);
      digitalWrite(DWN_LED, LOW);
      digitalWrite(RELEASE_LED, LOW);
      digitalWrite(FORCE_LED, LOW);
    }else{
      digitalWrite(SAFTY_LED, LOW);
      checkPistonButtons();
    }
    nextUpdate=millis()+BUTTONS_PERIOD;
  }
}

void updateLCD(){
  static int nextUpdate=0;  

  if(millis() > nextUpdate ) {
    LCD.write(CTRL1);
    LCD.write(CLEAR);
  
    LCD.write(CTRL1);
    LCD.write(LINE1);
    LCD.write("Sensor Cart   ");
    LCD.print(millis()/1000);
    LCD.write(CTRL1);
    LCD.write(LINE2);
    LCD.write("File: test.csv");
        
    nextUpdate = millis() + LCD_UPDATE_PERIOD;
  }
}

void loop(){  
  sendButtons();
  
  updateLCD();
  readCartData();
}
