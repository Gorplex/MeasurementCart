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


#define DELAY 100

//LCD on pin 18
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
  
  delay(500); // wait for display to boot up
  LCD.write(CTRL1);
  LCD.write(CLEAR);
}

void updateLCD(int ticks){
  /*
  if((ticks%(1000/DELAY))==0){
    LCD.write(CTRL1);
    LCD.write(CLEAR);
  }*/
  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("Hello, world!  ");
  LCD.print(ticks/(1000/DELAY));
  LCD.write(CTRL1);
  LCD.write(LINE2);
  LCD.write("TEST: ");
  LCD.print(3.145);
  LCD.write("m/s");
  LCD.write(CTRL1);
  LCD.write(LINE3);

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

void sendButtons(){
  if(digitalRead(SAFTY)){
    digitalWrite(SAFTY_LED, HIGH);
    CART.write("R0");
    CART.write("F0");
  }else{
    digitalWrite(SAFTY_LED, LOW);
    CART.write("R1");
    CART.write("F1");
  }
}

void loop(){
  static int ticks = 0;

  if(ticks%10){
    sendButtons();
  }
  
  
  updateLCD(ticks);
  readCartData();
  
  
  delay(DELAY);
  ticks++;
}
