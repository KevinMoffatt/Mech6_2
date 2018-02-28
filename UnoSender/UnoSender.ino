#include <SoftwareSerial.h>

const byte numChars = 32; //length of maximum serial monitor message
char receivedChars[numChars]; // Array to store received data
boolean newData = false;
String message;
double messData;

int responseVal;
int goIter = 0;
String mess;  // Initialize message variable
String reset;

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()  
{
  // Open serial communications with computer and wait for port to open:wa
  Serial.begin(9600);

  Serial.println("MESSAGE FORMAT:");
  Serial.println("<MESSAGE>");

  // Print a message to the computer through the USB
  Serial.println("enter message in correct format!");

  // Open serial communications with the other Arduino board
  mySerial.begin(9600);
}
void loop() // run over and over
{
  receiveSerialMessage(); //check for serial monitor messages
  delay(100); //minimum delay for showNewData to output correctly, if this is not a concern, set the delay to 3.
  showNewData();
  if(String(receivedChars) == "go servo"){
    mySerial.write(255);
    mySerial.write(50);
    goIter++;
    //Serial.println(goIter);
    receivedChars[0] = 'x';
  }
  if(String(receivedChars) == "go bot"){
    mySerial.write(254);
    mySerial.write(50);
    goIter++;
    //Serial.println(goIter);
    receivedChars[0] = 'x';
  }
  if(String(receivedChars) == "go forward and reverse"){
    mySerial.write(253);
    goIter++;
    //Serial.println(goIter);
    receivedChars[0] = 'x';
  }
  if(String(receivedChars) == "go turn"){
    mySerial.write(252);
    goIter++;
    //Serial.println(goIter);
    receivedChars[0] = 'x';
  }
  if(String(receivedChars) == "go manipulators"){
    mySerial.write(251);
    goIter++;
    //Serial.println(goIter);
    receivedChars[0] = 'x';
  }
  xbeeMegaResponse();
}

void receiveSerialMessage() // ONLY WORKS WITH NEWLINE SELECTED IN SERIAL MONITOR
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData(){
  if(newData == true){
    Serial.print("Serial Message : ");
    Serial.println(receivedChars);
    newData = false;
  }
}

void xbeeMegaResponse(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 255){ // check for flag value
      continue;
    }
    responseVal = mySerial.read();
    Serial.println(responseVal);
  }
}
