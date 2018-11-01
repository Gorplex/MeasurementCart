//from https://github.com/olkal/HX711_ADC library (in arduino lib manager)
#include <HX711_ADC.h>

#define CONTROLLER Serial1

#define RAISE_PISTON 9
#define FORCE_PISTON 11

//load cell
#define LC_PERIOD 100
#define LC_DAT 6
#define LC_CLK 5
#define SETTLE_TIME 500
#define CAL_FACTOR 696.0
#define LC_FACTOR -.022
HX711_ADC LoadCell(LC_DAT, LC_CLK);


void loadCellSetup(){
  LoadCell.begin();
  LoadCell.start(SETTLE_TIME);
  LoadCell.setCalFactor(CAL_FACTOR); 
}


void setup()
{
  loadCellSetup();
  
  //Controlelr comunication
  CONTROLLER.begin(9600);
  CONTROLLER.flush();

  //DEBUG
  Serial.begin(9600);

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

//runs based on load cell period
void updateLoadCell(){
  static int nextUpdate=0;  
  float value;
  
  if(millis() > nextUpdate ) {
    LoadCell.update();
    value = LoadCell.getData();
    CONTROLLER.print("LC:");
    CONTROLLER.print(LC_FACTOR*value, 6);
    CONTROLLER.print("kg\n");
    nextUpdate = millis() + LC_PERIOD;
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
  updateLoadCell();
  //sendData();
  debugSerial();
}
