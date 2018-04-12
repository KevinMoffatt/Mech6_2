#define MEGA
#include <DualVNH5019MotorShieldMod3.h>
#include <Encoder.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <QTRSensors.h>
#define NUM_SENSORS 8 //number sensors used
#define TIMEOUT     2500 //waits 2.5 sec before setting output to low

#define EMITTER_PIN 41 //emitter controlled by pin 2

QTRSensorsRC qtrrc((unsigned char[]) {39, 37, 35, 33, 31, 29, 27, 25}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

int sensorLowVals[NUM_SENSORS] = {340, 340, 240, 240, 400, 300, 450, 600};

int sensorBias[8];
int sensorNums[NUM_SENSORS] = {1,2,3,4,5,6,7,8};

#define FRONT_IR A1 //Right IR Rangefinder Pin
double frontIRVolt = 0;
double frontDist = 0;
#define BACK_IR A2 //Left IR Rangefinder Pin
double backIRVolt = 0;
double backDist = 0;

double messData;

//Initialize pins for servo
int servoArmPin = 11; // THIS CORRESPONDTDS TO THE SERVO'S ORANGE WIRE

// Initialize message variables
double messDouble;
SoftwareSerial mySerial(50, 51); // RX, TX WIRE DIGITAL PINS 2 AND 3 TO PINS 50 AND 51 RESPECTIVELY

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

// configure hall sensor
int Hall = A0; // hall effect sensor input
double hallVolt = 2.55;

Servo armServo;
int initialArmPos = 50;
int i=0;
int wallAngle = 90;

//encoder instance using interrupt pins
int leftInt1 = 18;
int leftInt2 = 19;
Encoder leftEnc(leftInt1,leftInt2);
int rightInt1 = 20;
int rightInt2 = 21;
Encoder rightEnc(rightInt1,rightInt2);

double GearRatio = 131;
int countsPerRev_motor = 64;
long leftCounts = 0;
long rightCounts = 0;

//time variables
double t = 0;
double t_old = 0;
double leftPos = 0;
double leftVel = 0;
double leftPos_old = 0;
double rightPos = 0;
double rightVel = 0;
double rightPos_old = 0;

double f = 2;
double rWheel = 42.0;

double wallDist = 315; //distance in mm
double postWallDist = 50; //distance in mm
double gain = 100;
double leftLoc = 0;
double rightLoc = 0;
double leftPow = 0;
double rightPow = 0;
double velocityDesired = 50;

int linePass = 0;
double rightIRDistOld = 0;
double leftIRDistOld = 0;
int IRChange = 0;
int hallCheck = 0;
int comm = 0;
int wallStatus = 0;
int IRCheck = 1;
//double rightDistDes = 0;
//double leftDistDes = 0;
double frontDistDes = 0;
double backDistDes = 0;
double hallRef = 0;


void setup()  
{
  // Open serial communications with computer and wait for port to open:
  Serial.begin(9600);

  Serial.println("MESSAGE CODES:");
  // Serial.println("");

  // Print a message to the computer through the USB
  // Serial.println("enter message!");

  // Open serial communications with the other Arduino board
  mySerial.begin(9600);
  armServo.write(initialArmPos);
  armServo.attach(servoArmPin);
  md.setM1Speed(400); // right motor forward for forward movement
  md.setM2Speed(-400); // left motor reverse for forward movement
  delay(2000);
  armServo.write(wallAngle);
  md.init();
  hallRef = analogRead(Hall);
}

void loop() // run over and over
{
  if(comm == 0){
    xbeeComms();  
  }
  if(comm > 0){
      hallTest();
        while(hallCheck){
          if(comm == 1){
            rangeFind();
            while(IRChange == 1){
              wallDrive();
              if(wallStatus){
                lineFollow();
              }
            }
      }
      if(comm == 2){
        wallDrive();
        if(wallStatus){
          lineFollow();
        }
      }
      if(comm == 3){
        lineFollow();
      }
    }    
  }
}

void hallTest(){
  while(hallCheck == 0){
    hallVolt = analogRead(Hall); 
    //hallVolt = hallVolt*(5.0/1023.0);
    double hallDiff = abs(hallRef-hallVolt);
    Serial.println(hallVolt);
    if(hallDiff >= 25){ 
      //mySerial.write(255);
      //Serial.print(hallVolt);
      hallCheck = 1;
  }
  }
}

void xbeeComms(){
  while(mySerial.available() > 1){
    if(mySerial.read() != 255){ // check for flag value
      continue;
    }               
    comm = mySerial.read();
    mySerial.write(255);
    mySerial.write(comm);
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

void driveStraight(){
  t_old = t;
  leftPos_old = leftPos;
  rightPos_old = rightPos;
  leftCounts = leftEnc.read();
  rightCounts = rightEnc.read();
  leftPos = leftCounts*2.0*PI/(GearRatio*countsPerRev_motor);
  rightPos = rightCounts*2.0*PI/(GearRatio*countsPerRev_motor);
  t = micros()/1000000.0;
  leftLoc = rWheel*leftPos;
  rightLoc = -rWheel*rightPos;
  double leftDesired = leftPos+velocityDesired*(t-t_old);
  double rightDesired = rightPos+velocityDesired*(t-t_old);
  leftPow = gain*(leftDesired - leftPos);  
  rightPow = gain*(rightDesired - rightPos);
  md.setM1Speed(rightPow);               //command voltage to motor
  md.setM2Speed(-leftPow);               //command voltage to motor
  if(wallDist-leftLoc <= 0 && wallDist-rightLoc <= 0){
    md.setM1Brake(400);               //command voltage to motor
    md.setM2Brake(400);               //command voltage to motor
    delay(1000);
    md.setM1Brake(0);               //command voltage to motor
    md.setM2Brake(0);               //command voltage to motor
    t = 0;
    t_old = t;
    leftCounts = 0;
    rightCounts  = 0;
    leftPos = 0;
    rightPos = 0;
    leftPos_old = leftPos;
    rightPos_old = rightPos;
  }
}

void wallDrive(){
  t_old = t;
  leftPos_old = leftPos;
  rightPos_old = rightPos;
  leftCounts = leftEnc.read();
  rightCounts = rightEnc.read();
  leftPos = leftCounts*2.0*PI/(GearRatio*countsPerRev_motor);
  rightPos = rightCounts*2.0*PI/(GearRatio*countsPerRev_motor);
  t = micros()/1000000.0;
  leftLoc = rWheel*leftPos;
  rightLoc = -rWheel*rightPos;
  double leftDesired = leftPos+velocityDesired*(t-t_old);
  double rightDesired = rightPos+velocityDesired*(t-t_old);
  leftPow = gain*(leftDesired - leftPos);  
  rightPow = gain*(rightDesired - rightPos);
  md.setM1Speed(rightPow);               //command voltage to motor
  md.setM2Speed(-leftPow);               //command voltage to motor
  if(wallDist-leftLoc <= 0 && wallDist-rightLoc <= 0){
    md.setM1Brake(400);               //command voltage to motor
    md.setM2Brake(400);               //command voltage to motor
    delay(1000);
    md.setM1Brake(0);               //command voltage to motor
    md.setM2Brake(0);               //command voltage to motor
    t = 0;
    t_old = t;
    leftCounts = 0;
    rightCounts  = 0;
    leftPos = 0;
    rightPos = 0;
    leftPos_old = leftPos;
    rightPos_old = rightPos;
    armServo.write(wallAngle);
    delay(2000);
    driveStraight();
    wallStatus = 1;
  }
}

void rangeFind(){
//  t_old = t;
//  leftPos_old = leftPos;
//  rightPos_old = rightPos;
//  leftCounts = leftEnc.read();
//  rightCounts = rightEnc.read();
//  leftPos = leftCounts*2.0*PI/(GearRatio*countsPerRev_motor);
//  rightPos = rightCounts*2.0*PI/(GearRatio*countsPerRev_motor);
//  t = micros()/1000000.0;
//  leftLoc = rWheel*leftPos;
//  rightLoc = -rWheel*rightPos;
//  double leftDesired = leftPos+velocityDesired*(t-t_old);
//  double rightDesired = rightPos+velocityDesired*(t-t_old);
//  leftPow = 20*(leftDesired - leftPos);  
//  rightPow = 20*(rightDesired - rightPos);
  frontIRVolt = analogRead(FRONT_IR)*5.0/1023.0;
  frontDist = pow(frontIRVolt,-1.1528)*11.71;
  backIRVolt = (analogRead(BACK_IR))*5.0/1023.0;
  backDist = pow(backIRVolt,-1.1809)*12.52;
  double proportion = 25;
  if(IRCheck){
      frontDistDes =  frontDist;
      backDistDes =  backDist;
      double frontDistErr = frontDist-frontDistDes;
      double backDistErr = backDist-backDistDes;
  }
  double frontDistErr = frontDist-frontDistDes;
  double backDistErr = backDist-backDistDes;
  double frontBackErr = frontDistErr-backDistErr;
  IRCheck = 0;
  int leftMotorSpeed = -45 - proportion*frontBackErr;
  int rightMotorSpeed = 50 - proportion*frontBackErr; 
 
  //int leftMotorSpeed = constrain(50+proportion*rightDistErr,-200,200);
//  Serial.print(leftMotorSpeed);
  Serial.print("\t");
  md.setM2Speed(leftMotorSpeed);
  //int rightMotorSpeed = //constrain(50+proportion*leftDistErr,-200,200);
  Serial.println(rightMotorSpeed);
  md.setM1Speed(rightMotorSpeed);
  if(abs(rightIRDistOld - frontDist) > 50){
    IRChange = 1;
    driveStraight();
//    t = 0;
//    t_old = t;
//    leftCounts = 0;
//    rightCounts  = 0;
//    leftPos = 0;
//    rightPos = 0;
//    leftPos_old = leftPos;
//    rightPos_old = rightPos;
  }
  else{
    IRChange = 0;
  }
  rightIRDistOld = frontDist;
}

void lineFollow(){
  qtrrc.read(sensorValues);
  //print sensor values from 0 to 25000, 0 means max reflectance (white)
  //2500 means minimum reflectance (black)
  for(unsigned char i = 0; i<NUM_SENSORS; i++){
    sensorBias[i]=sensorValues[i]-sensorLowVals[i]+200;
  }
  double lineLoc = 0;
  int sumVals;
  double sumMult = 0;
  for(unsigned char i = 0; i<NUM_SENSORS; i++){
    sumVals+=sensorBias[i];
    sumMult+=sensorBias[i]*sensorNums[i];
  }
  lineLoc = float(sumMult)/float(sumVals)-4.5;
  Serial.println(lineLoc);
  if(lineLoc < -.25 && lineLoc > -9){
    md.setM2Speed(-40);
    md.setM1Speed(0);
  }
  else if(lineLoc > .25 && lineLoc < 9){
    md.setM1Speed(40);
    md.setM2Speed(0);
  }
  else if(lineLoc >=9  || lineLoc <=-9){
    md.setM1Brake(400);
    md.setM2Speed(400);
    linePass = 1;
  }
  else
  {
    md.setM1Speed(40);
    md.setM2Speed(-40);
  }
  //delay(250);
}

