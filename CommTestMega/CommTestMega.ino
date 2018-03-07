#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 45); // RX, TX

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
  while(mySerial.available()>=1){
    if(mySerial.read()!= 255){
      continue;
    }
    Serial.println(mySerial.read());
    delay(10);
  }
  delay(1000);
}
