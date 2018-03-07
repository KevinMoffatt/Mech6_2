#include <SoftwareSerial.h>
#include <Servo.h>

double messData;

//Initialize pins
int servoArmPin = 9;

// Initialize message variables
double messDouble;
SoftwareSerial mySerial(2, 3); // RX, TX

Servo armServo;

int initialArmPos = 0;
char buffer[50];
int num[3];
int i=0;
int angle = initialArmPos;
const byte numChars = 32;
char receivedChars[numChars]; // Array to store received data
boolean newData = false;

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
  armServo.write(initialArmPos);
  armServo.attach(servoArmPin);
}

void loop() // run over and over
{
  GetServoAngle();
}

void GetServoAngle(){
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
  angle = atoi(receivedChars);
  Serial.println(angle);
  armServo.write(angle);
  newData = false;
}

double ReceiveSerialDouble()
{
  if (mySerial.available())
  { // If data comes in from serial, send it to message
    messData = mySerial.read();
    Serial.println(messData);
  }
  return messData;
}

void SendSerialDouble(double sendDoub)
{
  if (mySerial.available()){
    mySerial.write(sendDoub);
  }
}
