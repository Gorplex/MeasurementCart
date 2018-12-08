
//#include <SoftwareSerial.h>

#define CTRL1 0xFE
#define CTRL2 0x7C

#define CLEAR 0x01

#define SETCURS 0x80
#define LINE1 SETCURS
#define LINE2 SETCURS+64

#define LCD Serial1

void setup()
{
  LCD.begin(9600);
  LCD.write(0x12);  //reset to 9600 baud during startup
  
  delay(5000); // wait for display to boot up

  LCD.write(CTRL2);
  LCD.write(0x03);

  LCD.write(CTRL2);
  LCD.write(0x05);
  
  LCD.write(CTRL1);
  LCD.write(CLEAR);
  
  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("  Track Sensation   ");
  LCD.write(CTRL1);
  LCD.write(LINE2);
  LCD.write("   Cal Poly 2018    ");

  delay(5000);

  LCD.write(CTRL2);
  LCD.write(0x0A);//set splash

  delay(2000);

  LCD.write(CTRL2);
  LCD.write(0x0D);  //baud to 9600

  //LCD.write(CTRL2);
  //LCD.write(0x9D);  //backlight on

  LCD.write(CTRL1);
  LCD.write(CLEAR);

  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("Complete");
  
}

void loop()
{
  LCD.write(CTRL1);
  LCD.write(LINE1);
  LCD.write("Complete");
  
}
