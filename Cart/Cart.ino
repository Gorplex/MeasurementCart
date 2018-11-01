
#define CONTROLLER Serial1

#define RAISE_PISTON 9
#define FORCE_PISTON 11

#define DELAY 100

void setup()
{
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
void setPistons(){
  char input;
  if(2 <= CONTROLLER.available()){
    //Serial.write("cahr aval\n");
    input = CONTROLLER.read();
    if(input == 'R'){
      Serial.write("matched R\n");
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


void loop()
{
  static int ticks = 0;
  if(ticks%10){
    sendData();
  }
  //call twice to check delay
  setPistons();
  setPistons();
  setPistons();
  delay(DELAY);
  Serial.print(ticks);
  Serial.write("\n");
  ticks++;
}
