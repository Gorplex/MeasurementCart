
#include <SoftwareSerial.h>
#include <HX711_ADC.h>

//LOAD CELL
#define FACTOR -.022
#define FACTORSTEP -.01

//PINS
#define loadDat 6
#define loadClk 5

#define LCDTX 2
#define LCDRX 3 //(unused)
 
//LCD
#define CTRL1 0xFE
#define CTRL2 0x7C

#define CLEAR 0x01

#define SETCURS 0x80
#define LINE1 SETCURS
#define LINE2 SETCURS+64
#define LINE3 SETCURS+20
#define LINE4 SETCURS+84

//SYSTEM
#define QUANTUM 10


//ENCODER
int val;
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

//LCD
SoftwareSerial LCD(LCDRX,LCDTX);

//load cell
HX711_ADC LoadCell(loadDat, loadClk);
long t;
double factor = FACTOR;

int curTime;
unsigned long lastMS;

void setup()
{
  LCD.begin(9600); // set up serial port for 9600 baud
  
  delay(500); // wait for display to boot up
  
  LoadCell.begin();
  long stabilisingtime = 500;   //CHECK
  LoadCell.start(stabilisingtime);
  LoadCell.setCalFactor(696.0); 
  t=0;

  //ENCODER
  pinMode (encoder0PinA, INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP);
  lastMS = millis();
  curTime = 0;
}

void updateLCD(){
  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("LOAD CELL TEST   ");
  LCD.print(curTime);
  
  LCD.write(CTRL1);
  LCD.write(LINE3);
  LCD.print("Scaler: ");
  LCD.print(factor, 5);
}

void checkEncoder(){
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    //Serial.print (encoder0Pos);
    //Serial.print ("\n");
  }
  encoder0PinALast = n;
}

void updateFactor(){
  factor = FACTOR +encoder0Pos*FACTORSTEP;
}

int cnt = 0;

void loop(){
  checkEncoder();
  if(curTime==0){//if((curTime%20)==0){
    LCD.write(CTRL1);
    LCD.write(CLEAR);
  }
  checkEncoder();
  if((cnt%5000)==0){
    updateFactor();
    updateLCD();
  }

  LoadCell.update();
  if (millis() > t + 250) {
    float i = LoadCell.getData();
    LCD.write(CTRL1);
    LCD.write(LINE2);
    LCD.print("RawVal: ");
    LCD.print(i, 5);
      
    LCD.write(CTRL1);
    LCD.write(LINE4);
    LCD.print("Scaled: ");
    LCD.print(i*factor,5);
    t = millis();
  }
  
  if(millis()>=lastMS+1000){
    curTime+=(millis()-lastMS)/1000;
    lastMS = millis();
  }
  //delay(QUANTUM);
  cnt++;
}

