#include <HX711_ADC.h>

//LOAD CELL
#define SETTLE_TIME 1000
#define UPDATE_TIME 500
#define CAL_FACTOR -15312.0
#define loadClk 18  
#define loadDat 19


//load cell
HX711_ADC LoadCell(loadDat, loadClk);

void setup()
{
  Serial.begin(9600); 
  Serial.flush();
  Serial.write("Strating\n");
  LoadCell.begin();
  LoadCell.start(SETTLE_TIME);
  LoadCell.setCalFactor(CAL_FACTOR); 
  Serial.write("Zeroed\n");
}

void loop(){
  static unsigned long updateTime = 0;
  float val;

  //called offten
  LoadCell.update();
  if (millis() >= updateTime) {
    val = LoadCell.getData();
    Serial.print("RawVal: ");
    Serial.print(val, 5);
    Serial.print("\n");
    updateTime = millis()+UPDATE_TIME;
  }
}
