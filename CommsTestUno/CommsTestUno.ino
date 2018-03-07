#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()  
{
  // Open serial communications with computer and wait for port to open:
  Serial.begin(9600);

  // Print a message to the computer through the USB
  Serial.println("Hello Computer!");

  // Open serial communications with the other Arduino board
  mySerial.begin(9600);

}

void loop() // run over and over
{
   mySerial.write(255);
   mySerial.write(230);
   mySerial.write(20);
   delay(1000);
}
