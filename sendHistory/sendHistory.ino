#include <SoftwareSerial.h>

const byte numChars = 32;
char receivedChars[numChars]; // Array to store received data
boolean newData = false;
String message;

String mess;  // Initialize message variable

SoftwareSerial mySerial(2, 3); // RX, TX

/*MESSAGE CODES:
 * 1. Run component tests
 * 2. 
*/
void setup()  
{
  // Open serial communications with computer and wait for port to open:
  Serial.begin(9600);

  Serial.println("MESSAGE CODES:");
  Serial.println("");

  // Print a message to the computer through the USB
  Serial.println("enter message!");

  // Open serial communications with the other Arduino board
  mySerial.begin(9600);


}

void loop() // run over and over
{
  mess = receiveSerialMessage();
  showNewData();
}

String receiveSerialMessage() // ONLY WORKS WITH NEWLINE SELECTED IN SERIAL MONITOR
{
  static byte ndx = 0;
  char endMark = '\n';
  char rc;
  while(Serial.available() > 0 && newData == false){
    rc = Serial.read();

    if(rc != endMark){
      receivedChars[ndx] = rc;
      ndx++;
      if(ndx >= numChars){
        ndx = numChars - 1;
      }
    }
    else{
      receivedChars[ndx] = '\0'; // End the string
      ndx = 0;
      newData = true;
    }
  }
  message = atoi(receivedChars);
  return message;
}

void showNewData(){
  if(newData == true){
    Serial.print("Serial Message : ");
    Serial.println(receivedChars);
    newData = false;
  }
}
