/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 (50 on mega)
 ** MISO - pin 12 (51 on mega)
 ** CLK - pin 13 (52 on mega)
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN) (53 on mega)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

int SDCardCSPin = 53;
/*
 * SDCard lib uses pins 50-53 on the mega
 * 
 * 
 */

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {  //defaults to pin 53
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  if(myFile = SD.open("test.txt", FILE_WRITE)){
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    myFile.println("Writen on 11/12/18");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }


  
}

void loop() {
  // put your main code here, to run repeatedly:

}
