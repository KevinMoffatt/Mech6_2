#define MEGA
#include <DualVNH5019MotorShieldMod3.h>
//#include <Encoder.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#include <QTRSensors.h>
#define NUM_SENSORS 8 //number sensors used
#define TIMEOUT     2500 //waits 2.5 sec before setting output to low

#define EMITTER_PIN 41 //emitter controlled by pin 2

QTRSensorsRC qtrrc((unsigned char[]) {39, 37, 35, 33, 31, 29, 27, 25}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

int sensorLowVals[NUM_SENSORS] = {300, 244, 244, 244, 196, 300, 196, 300};

int sensorBias[8];
int sensorNums[NUM_SENSORS] = {1,2,3,4,5,6,7,8};


#define RIGHT_IR A1 //Right IR Rangefinder Pin
#define LEFT_IR A2 //Left IR Rangefinder Pin

double leftIRVolt = 0;
double rightIRVolt = 0;
double leftDist = 0;
double rightDist = 0;

double messData;

//Initialize pins for servo
int servoArmPin = 11; // THIS CORRESPONDTDS TO THE SERVO'S ORANGE WIRE

// Initialize message variables
double messDouble;
SoftwareSerial mySerial(50, 51); // RX, TX WIRE DIGITAL PINS 2 AND 3 TO PINS 24 AND 25 RESPECTIVELY

// configure library with pins as remapped
unsigned char INA3 = 22; // motor 3 directional input 1
unsigned char INB3 = 23; // motor 3 directional input 2
unsigned char EN3DIAG3 = 26; // motor 3 encoder
unsigned char CS3 = 27; // dummy current sensing pin
unsigned char PWM3 = 13; // motor 3 power
unsigned char INA4 = 52; // motor 4 directional input 1
unsigned char INB4 = 53; // motor 4 directional input 2
unsigned char EN4DIAG4 = 29; // motor 4 encoder
unsigned char CS4 = 30; // dummy current sensing pin
unsigned char PWM4 = 46; // motor 4 power
DualVNH5019MotorShieldMod3 md(INA3, INB3, EN3DIAG3, CS3, PWM3, INA4, INB4, EN4DIAG4, CS4, PWM4); //Use default pins for motor shield 1 and remapped pins for motor shield 2

Servo armServo;
int initialArmPos = 0;
int i=0;
int angle = initialArmPos;

void setup()  
{
  // Open serial communications with computer and wait for port to open:
  // Serial.begin(9600);

  // Serial.println("MESSAGE CODES:");
  // Serial.println("");

  // Print a message to the computer through the USB
  // Serial.println("enter message!");

  // Open serial communications with the other Arduino board
  Serial.begin(9600);
  //armServo.write(initialArmPos);
  //armServo.attach(servoArmPin);
  //md.init();
}

void loop() // run over and over
{
  //xbeeTestBot();
  //xbeeTestBotStraight();
  //xbeeServo();
  //xbeeTestBotTurns();
  //xbeeTestBotManip();
  lineFollow();
  //rangeTest();
  
}

void rangeTest(){
  //leftIRVolt = analogRead(LEFT_IR)*5.0/1023.0;
  //leftDist = pow(leftIRVolt,-1.1809)*12.52;
  rightIRVolt = analogRead(RIGHT_IR)*5.0/1023.0;
  rightDist = pow(rightIRVolt,-1.1528)*11.71;
  if(rightDist>10.5){
    md.setM2Speed(-70);
    md.setM1Speed(65);
  //    Serial.println("turn right");
  }
  else if(rightDist<9.5){
    md.setM1Speed(70);
    md.setM2Speed(-65);
   //   Serial.println("turn left");
  }
  else{
    md.setM1Speed(70);
    md.setM2Speed(-70);
    //  Serial.println("straight");
  }
  delay(250);
}

void lineFollow(){
  qtrrc.read(sensorValues);
  //print sensor values from 0 to 25000, 0 means max reflectance (white)
  //2500 means minimum reflectance (black)
  for(unsigned char i = 0; i<NUM_SENSORS; i++){
    sensorBias[i]=sensorValues[i]-sensorLowVals[i]+200;
    Serial.print(sensorValues[i]);
    Serial.print("\t");
  }
  Serial.println();
  double lineLoc = 0;
  int sumVals;
  double sumMult = 0;
  for(unsigned char i = 0; i<NUM_SENSORS; i++){
    sumVals+=sensorBias[i];
    sumMult+=sensorBias[i]*sensorNums[i];
  }
  lineLoc = float(sumMult)/float(sumVals)-3.85;
  //Serial.println(lineLoc);
  if(lineLoc < -.25){
    //md.setM2Speed(-40);
    //md.setM1Speed(0);
  }
  else if(lineLoc > .25){
    //md.setM1Speed(40);
    //md.setM2Speed(0);
  }
  else
  {
    //md.setM1Speed(40);
    //md.setM2Speed(-40);
  }
  delay(250);
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
  }
}

void xbeeTestBotStraight(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 253){ // check for flag value
      continue;
    }
    md.setM1Speed(400); // right motor forward for forward movement
    md.setM2Speed(-400); // left motor reverse for forward movement
    delay(10000); // wait for 10 seconds
    md.setM1Speed(0); // single-channel motor stop (coast)
    md.setM2Speed(0); // single-channel motor stop (coast)
    delay(2000);
    md.setM1Speed(-400); // right motor reverse for reverse movement
    md.setM2Speed(400); // left motor forward for reverse movement
    delay(10000); // wait for 10 seconds
    md.setM1Speed(0); // single-channel motor stop (coast)
    md.setM2Speed(0); // single-channel motor stop (coast)
    mySerial.write(255);
    mySerial.write(12);
  }
}

void xbeeTestBotTurns(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 252){ // check for flag value
      continue;
    }
    md.setM1Speed(400); // right motor forward for left turn 
    md.setM2Speed(400); // left motor forward for left turn
    delay(10000); // wait for 10 seconds
    md.setM1Speed(0); // single-channel motor stop (coast)
    md.setM2Speed(0); // single-channel motor stop (coast)
    delay(2000);
    md.setM1Speed(-400); // right motor reverse for right turn 
    md.setM2Speed(-400); // left motor reverse for left turn 
    delay(10000); // wait for 10 seconds
    md.setM1Speed(0); // single-channel motor stop (coast)
    md.setM2Speed(0); // single-channel motor stop (coast)
    mySerial.write(255);
    mySerial.write(13);
  }
}

void xbeeTestBotManip(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 251){ // check for flag value
      continue;
    }
    armServo.write(50); // write servo to rail value
    delay(3000); // wait for 3 seconds
    md.setM3Speed(400); // run rail motor
    delay(5000); // wait for 5 seconds
    md.setM3Speed(0); // single-channel motor stop (coast)
    armServo.write(initialArmPos);
    delay(3000); // wait for 3 seconds
    md.setM4Speed(400); // run jack motor to lift robot
    delay(3000);
    md.setM4Speed(-400); // run jack motor to lift robot
    delay(2000);
    md.setM4Speed(0); // single-channel motor stop (coast)
    mySerial.write(255);
    mySerial.write(14);
  }
}

void xbeeTestBot(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 254){ // check for flag value
      continue;
    }
    angle = mySerial.read();
    armServo.write(angle);
    Serial.println(angle);
    md.setM1Speed(400); // single-channel motor full-speed "forward"

    delay(2000); // wait for 2 seconds
    
    md.setM1Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM2Speed(400); // single-channel motor full-speed "reverse"
    
    delay(2000); // wait for 2 seconds
    
    md.setM2Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM3Speed(400); // single-channel motor full-speed "forward"
    
    delay(2000); // wait for 2 seconds
    
    md.setM3Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM4Speed(400); // single-channel motor full-speed "forward"
    
    delay(2000); // wait for 2 seconds
    
    md.setM4Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM1Speed(0); // single-channel motor stop (coast)
    md.setM2Speed(0); // single-channel motor stop (coast)
    md.setM3Speed(0); // single-channel motor stop (coast)
    md.setM4Speed(0); // single-channel motor stop (coast)
    
    // the following code is a simple example:
    //
    md.setM1Speed(-400); // single-channel motor full-speed "forward"
    
    delay(2000); // wait for 2 seconds
    
    md.setM1Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM2Speed(-400); // single-channel motor full-speed "reverse"
    
    delay(2000); // wait for 2 seconds
    
    md.setM2Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM3Speed(-400); // single-channel motor full-speed "forward"
    
    delay(2000); // wait for 2 seconds
    
    md.setM3Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s
    
    md.setM4Speed(-400); // single-channel motor full-speed "forward"
    
    delay(2000); // wait for 2 seconds
    
    md.setM4Speed(0); // single-channel motor stop (coast)
    
    delay(500); // wait for 0.5 s

    md.setM1Speed(0); // single-channel motor stop (coast)
    md.setM2Speed(0); // single-channel motor stop (coast)
    md.setM3Speed(0); // single-channel motor stop (coast)
    md.setM4Speed(0); // single-channel motor stop (coast)

    mySerial.write(255);
    mySerial.write(angle);
  }
}
