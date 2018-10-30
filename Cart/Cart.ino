
#define CONTROLLER Serial1


void setup()
{
  //Controlelr comunication
  CONTROLLER.begin(9600);

  //Load Cell comunication
  //Serial1.begin(9600);
  
  delay(1000); // wait
}

void loop()
{
  for(int i=0;i<10;i++){
    CONTROLLER.write("LC:");
    CONTROLLER.print(i);
    CONTROLLER.write("\n");
    CONTROLLER.write("POS:");
    CONTROLLER.print((float)(i)/(i+1));
    CONTROLLER.write("\n");
    delay(1000);
  }
}
