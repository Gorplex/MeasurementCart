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

//pins
#define SAFTY 25
#define SAFTY_LED 26
#define UP 27
#define UP_LED 28
#define DWN 29
#define DWN_LED 30
#define RELEASE 31
#define RELEASE_LED 32
#define FORCE 33
#define FORCE_LED 34


#define DELAY 100

#define LCD Serial1
#define CART Serial3


//SoftwareSerial LCD(3,2); // pin 2 = TX, pin 3 = RX (unused)
//SoftwareSerial Data(0,1);

void setup()
{
  //LCD pin 18
  
  LCD.begin(9600); // set up serial port for 9600 baud
  //
  CART.begin(9600); // set up serial port for 9600 baud
  
  delay(500); // wait for display to boot up
}

void updateLCD(int ticks){
  //if((ticks%10)==0){
  LCD.write(CTRL1);
  LCD.write(CLEAR);
  
  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("Hello, world!  ");
  LCD.print(ticks);
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
    if(received == '\n'){
         LCD.write(CTRL1);
         LCD.write(LINE4);
    }else{
      LCD.write(received);
    }  
  }
}

void sendButtons(){

  
}

void loop(){
  static int ticks = 0;
  
  sendButtons();
  
  updateLCD(ticks);
  readCartData();
  
  
  delay(DELAY);
  ticks++;
}
