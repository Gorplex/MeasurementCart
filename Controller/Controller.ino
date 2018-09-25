
#include <SoftwareSerial.h>

#define CTRL1 0xFE
#define CTRL2 0x7C

#define CLEAR 0x01

#define SETCURS 0x80
#define LINE1 SETCURS
#define LINE2 SETCURS+64
#define LINE3 SETCURS+20
#define LINE4 SETCURS+84

#define QUANTUM 10

SoftwareSerial LCD(3,2); // pin 2 = TX, pin 3 = RX (unused)
SoftwareSerial Data(0,1);

void setup()
{
  LCD.begin(9600); // set up serial port for 9600 baud
  Data.begin(9600); // set up serial port for 9600 baud
  
  delay(500); // wait for display to boot up
}

void updateLCD(int ct){
  if((ct%1000)==0){
    LCD.write(CTRL1);
    LCD.write(CLEAR);
  }
  
  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("Hello, world!");
  LCD.write(CTRL1);
  LCD.write(LINE2);
  LCD.write("TEST: ");
  LCD.print(3.145);
  LCD.write("m/s");
  LCD.write(CTRL1);
  LCD.write(LINE3);
  LCD.write("Line 3 is here");
  LCD.write(CTRL1);
  LCD.write(LINE4);
  LCD.write("Line 4 is here");
}

int ct = 0;
void loop(){
  updateLCD(ct);
  delay(QUANTUM);
  ct++;
}

