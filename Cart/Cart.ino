
#include <SoftwareSerial.h>

SoftwareSerial Data(0,1);

void setup()
{
  Data.begin(9600);
  
  delay(500); // wait for display to boot up
}

void loop()
{
  while(1){
    Data.write("N:3.145\n");
  }
}

