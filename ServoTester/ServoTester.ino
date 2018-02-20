#include <SoftwareSerial.h>
#include <Servo.h>

double messData;

//Initialize pins
int servoArmPin = 11; // THIS CORRESPONDTDS TO THE SERVO'S ORANGE WIRE

// Initialize message variables
double messDouble;
SoftwareSerial mySerial(50, 51); // RX, TX WIRE DIGITAL PINS 2 AND 3 TO PINS 10 AND 11 RESPECTIVELY

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
  xbeeServo();
  //armServo.write(50);
  //mySerial.write(255);
  //mySerial.write(50);
  delay(5000);
  armServo.write(initialArmPos);
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
  mySerial.write(255);
  mySerial.write(angle);
  newData = false;
}

void xbeeServo(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 255){ // check for flag value
      continue;
    }
    angle = mySerial.read();
    armServo.write(angle);
    Serial.println(angle);
    mySerial.write(255);
    mySerial.write(angle);
    delay(4000);
  }
}
