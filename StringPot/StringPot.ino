
//gauge
#define READ_PIN A0
#define MIN_READ 4.0
#define MAX_READ 985.0
#define MIN_LEN 0.0
#define MAX_LEN 324.0 //in mm
#define MM_TO_THOU 39.3701

void setup() {
  pinMode(READ_PIN, INPUT);
  Serial.begin(9600);
}

//takes 10 bit analog read value and converts to mm
int readTomm(int value){
  return (int) MM_TO_THOU*(MAX_LEN-MIN_LEN)/(MAX_READ-MIN_READ)*(value-MIN_READ);
}

void loop() {
  Serial.println(readTomm(analogRead(READ_PIN)));
  delay(100);
}
