//from https://github.com/olkal/HX711_ADC library (in arduino lib manager)
#include <HX711_ADC.h>

#define CONTROLLER Serial1

//piston pins
#define RAISE_PISTON 9
#define FORCE_PISTON 11

//timing
#define SENSOR_PERIOD 100

//load cell
#define LC_DAT 6
#define LC_CLK 5
#define SETTLE_TIME 500
#define CAL_FACTOR 696.0
#define LC_FACTOR -.022
HX711_ADC LoadCell(LC_DAT, LC_CLK);

//string pot gauge measurment
#define GAUGE_PIN 
#define GAUGE_FACTOR 1




void loadCellSetup(){
  LoadCell.begin();
  LoadCell.start(SETTLE_TIME);
  LoadCell.setCalFactor(CAL_FACTOR); 
}

void pistonSetup(){

  
}

void setup()
{
  //Controlelr comunication
  CONTROLLER.begin(9600);
  CONTROLLER.flush();
  loadCellSetup();
  pistonSetup();
  
  //DEBUG
  Serial.begin(9600);

  //piston setup
  pinMode(RAISE_PISTON, OUTPUT);
  pinMode(FORCE_PISTON, OUTPUT);
  digitalWrite(RAISE_PISTON, LOW);
  digitalWrite(FORCE_PISTON, LOW);



}

void sendData(){
    for(int i=0;i<10;i++){
    CONTROLLER.write("LC:");
    CONTROLLER.print(i);
    CONTROLLER.write("\n");
    CONTROLLER.write("POS:");
    CONTROLLER.print((float)(i)/(i+1));
    CONTROLLER.write("\n");
  } 
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
  float value;
  
  LoadCell.update();
  value = LoadCell.getData();
  CONTROLLER.print("LC:");
  CONTROLLER.print(LC_FACTOR*value, 6);
  CONTROLLER.print("kg\n");
}

void readGauge(){
  
}

//runs based on load cell period
void readSensors(){
  static int nextUpdate=0;  
  
  if(millis() > nextUpdate ) {
    readLC();
    readGauge();
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
  setPistons();
  readSensors();
  //sendData();
  debugSerial();
}
