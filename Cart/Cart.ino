//from https://github.com/olkal/HX711_ADC library (in arduino lib manager)
#include <HX711_ADC.h>

#define CONTROLLER Serial3

//Hall Efect Encoder Pins
#define ENCA 2
#define ENCB 3
#define ISR_PIN ENCA
volatile long encCount = 0;

//Dist Measurment
#define MAG_PER_ROT 4
#define FT_PER_ROT  1.375  //circumference of wheel on track 

//piston pins
#define RELAY_EN A0
#define RAISE_PISTON A1
#define FORCE_PISTON A2

//timing
#define time_t unsigned long
#define SENSOR_PERIOD 1000

//load cell
#define LC_CLK 18
#define LC_DAT 19
#define SETTLE_TIME 500
#define CAL_FACTOR -21982.125//696.0
HX711_ADC LoadCell(LC_DAT, LC_CLK);

//string pot gauge measurment
#define GAUGE_PIN A5
#define MIN_READ 4.0
#define MAX_READ 985.0
#define MIN_LEN 0.0
#define MAX_LEN 322.2625 //in mm
#define MM_TO_THOU 39.3701
#define THOU_TO_IN 1000
#define MIN_PIST_LEN .787             //length of sting pot at piston min (in inches)
#define TOTAL_GAUGE_AT_MIN_PIST 17.18825   //length of entire gauge at piston min (in inches)




void loadCellSetup(){
  LoadCell.begin();
  LoadCell.start(SETTLE_TIME);
  LoadCell.setCalFactor(CAL_FACTOR); 
}

void pistonSetup(){
  pinMode(RELAY_EN, OUTPUT);
  digitalWrite(RELAY_EN, HIGH);
  
  pinMode(RAISE_PISTON, OUTPUT);
  pinMode(FORCE_PISTON, OUTPUT);
  digitalWrite(RAISE_PISTON, LOW);
  digitalWrite(FORCE_PISTON, LOW);
}
void gaugeSetup(){
  pinMode(GAUGE_PIN, INPUT);
}

void EncISR(){ 
  if(!digitalRead(ENCA)){  //to prevent double positive counts ISR triggered on B falling if A was high (causing double count)
    if(!digitalRead(ENCB)){
      encCount++;
    }else{
      encCount--;
    }
  }
}

void setup()
{
  //Controlelr comunication
  CONTROLLER.begin(9600);
  CONTROLLER.flush();
  
  //DEBUG
  //Serial.begin(9600);
  
  loadCellSetup();
  pistonSetup();
  gaugeSetup();

  attachInterrupt(digitalPinToInterrupt(ISR_PIN), EncISR, FALLING);
}

//runs as often as posible
void setPistons(){
  char input;
  
  if(2 <= CONTROLLER.available()){
    input = CONTROLLER.read();
    if(input == 'R'){
      input = CONTROLLER.read();
      if(input == '0'){
        digitalWrite(RAISE_PISTON, LOW);
      }else if(input == '1'){
        digitalWrite(RAISE_PISTON, HIGH);
      }
    }else if (input == 'F'){
      input = CONTROLLER.read();
      if(input == '0'){
        digitalWrite(FORCE_PISTON, LOW);
      }else if(input == '1'){
        digitalWrite(FORCE_PISTON, HIGH);
      }
    }
  }
}

void readLC(){
  CONTROLLER.print(LoadCell.getData(), 4);
}

//takes 10 bit analog read value and converts to mm
int readTomm(int value){
  return (int) (MAX_LEN-MIN_LEN)/(MAX_READ-MIN_READ)*(value-MIN_READ);
}

void readGauge(){
  CONTROLLER.print(MM_TO_THOU*readTomm(analogRead(GAUGE_PIN))/THOU_TO_IN + TOTAL_GAUGE_AT_MIN_PIST - MIN_PIST_LEN, 3);
}

void readEnc(){
  CONTROLLER.print(((long)encCount)*FT_PER_ROT/MAG_PER_ROT, 1);
  
}

//runs based on load cell period
void readSensors(){
  static time_t nextUpdate=0;  
  
  if(millis() > nextUpdate ) {
    CONTROLLER.print("D");
    readEnc();
    CONTROLLER.print(",");
    readLC();
    CONTROLLER.print(",");
    readGauge();
    CONTROLLER.print("\n");
    nextUpdate = millis() + SENSOR_PERIOD;
  }
}

//runs evry second
#define DEBUG_PERIOD 1000
void debugSerial(){
  static int nextUpdate=0;  
  float value;
  
  if (millis() > nextUpdate ) {
    Serial.print(nextUpdate/DEBUG_PERIOD);
    Serial.print("\n");
    nextUpdate = millis() + DEBUG_PERIOD;
  }
}

void loop()
{
  //run as often as posible
  LoadCell.update();
  setPistons();
  readSensors();
  //debugSerial();
}
