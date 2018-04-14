#define MEGA
#include <DualVNH5019MotorShieldMod3.h>
#include <Encoder.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <QTRSensors.h> //initialize sensor array
#define NUM_SENSORS 8 //number sensors used
#define TIMEOUT     2500 //waits 2.5 sec before setting output to low
#define EMITTER_PIN 41 //emitter controlled by pin 2
QTRSensorsRC qtrrc((unsigned char[]) {
  39, 37, 35, 33, 31, 29, 27, 25
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
int sensorLowVals[NUM_SENSORS] = {300, 244, 244, 244, 196, 300, 196, 300};
int sensorBias[8];
int sensorNums[NUM_SENSORS] = {1, 2, 3, 4, 5, 6, 7, 8};

#define FRONT_IR A1 //Right IR Rangefinder Pin
double frontIRVolt = 0; //Set initial voltage
double frontDist = 0; //Set initial distance
#define BACK_IR A2 //Left IR Rangefinder Pin
double backIRVolt = 0; //Set initial voltage
double backDist = 0; //Set initial distance

double messData; //Initialize message variable

//Initialize pins for servo
int servoArmPin = 11; // THIS CORRESPONDTDS TO THE SERVO'S ORANGE WIRE
Servo armServo;
int initialArmPos = 50; //starting servo angle
int wallAngle = 90; //servo angle for wall
int railAngle = 130; //servo angle to reach rail runner
int warpedAngle = 110; //servo angle to hook onto warped wall

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
double hallVolt = 2.55; //initialize a quiescent hall voltage
double hallRef = 0; //inital hall reference voltage

//encoder instances using interrupt pins
int leftInt1 = 18;
int leftInt2 = 19;
Encoder leftEnc(leftInt1, leftInt2);
int rightInt1 = 20;
int rightInt2 = 21;
Encoder rightEnc(rightInt1, rightInt2);

//encoding variables
double GearRatio = 131;
int countsPerRev_motor = 64;
long leftCounts = 0;
long rightCounts = 0;
double t = 0;
double t_old = 0;
double leftPos = 0;
double leftVel = 0;
double leftPos_old = 0;
double rightPos = 0;
double rightVel = 0;
double rightPos_old = 0;
double rWheel = 42.0;
double gain = 100;
double leftLoc = 0;
double rightLoc = 0;
double leftPow = 0;
double rightPow = 0;
double velocityDesired = 50;

//drive distance variables
double wallDist = 315; //distance in mm
double postWallDist = 50; //distance in mm

//rangefinding variables
double frontIRDistOld = 0;
double frontDistDes = 0;
double backDistDes = 0;

//obstacle pass checks
int linePass = 0;
int IRChange = 0;
int hallCheck = 1;
int comm = 3;
int wallStatus = 0;
int IRCheck = 1;
int railRunCheck = 0;

void setup()
{
  // Open serial communications with computer and wait for port to open:
  Serial.begin(9600);

  Serial.println("MESSAGE CODES:");

  // Open serial communications with the other Arduino board
  mySerial.begin(9600);
  armServo.attach(servoArmPin);
  armServo.write(initialArmPos);
  md.init();
  //md.setM1Speed(400); // right motor forward for forward movement
  //md.setM2Speed(-400); // left motor reverse for forward movement
  //delay(2000);
  //armServo.write(wallAngle);
  //initialize motor driver
  //take a reference hall voltage reading
  delay(1000);

  hallRef = analogRead(Hall);
}

void loop() // run over and over
{
  if (comm == 0) { //keep communications open until a command is recieved
    xbeeComms();
  }
  if (comm > 0) { //if a command is recieved
    hallTest();//check the hall sensor
    while (hallCheck) { //if the sensor picks up a significant reading
      if (comm == 1) { //paddle board command
        rangeFind();//use rangefinders to navigate the paddleboard
        while (IRChange == 1) { //if there is a significant change in IR sensor reading
          wallDrive();//perform wall lift
          if (wallStatus) { //if the distance has been detected to be past the wall
            lineFollow();
          }
        }
      }
      if (comm == 2) { //wall life command
        wallDrive();//perform wall lift
        if (wallStatus) { //if the distance has been detected to be past the wall
          lineFollow();
        }
      }
      if (comm == 3) { //line follow command
        lineFollow();
      }
    }
  }
}

void hallTest() { //check the hall sensor reading
  while (hallCheck == 0) { //continually read hall sensor until a significant change is detected
    hallVolt = analogRead(Hall); //read hall sensor, value between 0 and 1023
    //hallVolt = hallVolt*(5.0/1023.0);
    double hallDiff = abs(hallRef - hallVolt); //determine difference between hall sensor and the reference voltage
    Serial.println(hallVolt);
    if (hallDiff >= 25) { //if the distance is significant, set flag to TRUE
      //mySerial.write(255);
      //Serial.print(hallVolt);
      hallCheck = 1; //set flag to TRUE
    }
  }
}

void xbeeComms() { //function for wireless communication
  while (mySerial.available() > 1) { //if there is a message
    if (mySerial.read() != 255) { // check for flag value throw away if not 255
      continue;
    }
    comm = mySerial.read();//read the value following the flag
    mySerial.write(255);//confirm communications
    mySerial.write(comm);//communicate the command that is being executed
  }
}

void driveStraight() { //drive with proportional control until wallDist is reached
  t_old = t;
  leftPos_old = leftPos;
  rightPos_old = rightPos;
  leftCounts = leftEnc.read();
  rightCounts = rightEnc.read();
  leftPos = leftCounts * 2.0 * PI / (GearRatio * countsPerRev_motor);
  rightPos = rightCounts * 2.0 * PI / (GearRatio * countsPerRev_motor);
  t = micros() / 1000000.0;
  leftLoc = rWheel * leftPos;
  rightLoc = -rWheel * rightPos;
  double leftDesired = leftPos + velocityDesired * (t - t_old);
  double rightDesired = rightPos + velocityDesired * (t - t_old);
  leftPow = gain * (leftDesired - leftPos);
  rightPow = gain * (rightDesired - rightPos);
  md.setM1Speed(rightPow);               //command voltage to motor
  md.setM2Speed(-leftPow);               //command voltage to motor
  if (wallDist - leftLoc <= 0 && wallDist - rightLoc <= 0) { //when the wallDist is reached, stop and reset encoding variables
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

void wallDrive() { //code to attempt wall lift
  //drive straight using proportional control
  t_old = t;
  leftPos_old = leftPos;
  rightPos_old = rightPos;
  leftCounts = leftEnc.read();
  rightCounts = rightEnc.read();
  leftPos = leftCounts * 2.0 * PI / (GearRatio * countsPerRev_motor);
  rightPos = rightCounts * 2.0 * PI / (GearRatio * countsPerRev_motor);
  t = micros() / 1000000.0;
  leftLoc = rWheel * leftPos;
  rightLoc = -rWheel * rightPos;
  double leftDesired = leftPos + velocityDesired * (t - t_old);
  double rightDesired = rightPos + velocityDesired * (t - t_old);
  leftPow = gain * (leftDesired - leftPos);
  rightPow = gain * (rightDesired - rightPos);
  md.setM1Speed(rightPow);               //command voltage to motor
  md.setM2Speed(-leftPow);               //command voltage to motor

  if (wallDist - leftLoc <= 0 && wallDist - rightLoc <= 0) { //if the distance to the wall is reached, attempt to lift it
    md.setM1Speed(200);               //command voltage to motor
    md.setM2Speed(-200);               //command voltage to motor
    armServo.write(50);//raise the arm servo a little to prevent wedging against the ground
    delay(1000);
    //reset encoding
    t = 0;
    t_old = t;
    leftCounts = 0;
    rightCounts  = 0;
    leftPos = 0;
    rightPos = 0;
    leftPos_old = leftPos;
    rightPos_old = rightPos;
    //attempt lift
    armServo.write(wallAngle);
    delay(2000);
    armServo.write(wallAngle + 30);
    //drive through wall
    md.setM1Speed(50);
    md.setM2Speed(-45);
    //set wall completion flag to TRUE
    wallStatus = 1;
  }
}

void rangeFind() { //Paddle board completion
  //check the front sensor's distance to the wall
  frontIRVolt = analogRead(FRONT_IR) * 5.0 / 1023.0;
  frontDist = pow(frontIRVolt, -1.1528) * 11.71;
  //check the back sensor's distance to the wall
  backIRVolt = (analogRead(BACK_IR)) * 5.0 / 1023.0;
  backDist = pow(backIRVolt, -1.1809) * 12.52;
  //define proportional control
  double proportion = 25;
  if (IRCheck) { // if this is the first reading, set the desired distance to what we read in
    frontDistDes =  frontDist;
    backDistDes =  backDist;
    double frontDistErr = frontDist - frontDistDes; //this will initially be 0
    double backDistErr = backDist - backDistDes; //this will initially be 0
  }
  double frontDistErr = frontDist - frontDistDes; //calculate the error in the front reading
  double backDistErr = backDist - backDistDes; //calculate the error in the back reading
  double frontBackErr = frontDistErr - backDistErr; //calculate the error between front and back errors for a final control value
  IRCheck = 0; //ensure that we don't reset the reading for IR desired
  int leftMotorSpeed = -45 - proportion * frontBackErr; //proportionally control the left motor
  int rightMotorSpeed = 50 - proportion * frontBackErr; //proportionally control the right motor
  //Serial.print("\t");
  md.setM2Speed(leftMotorSpeed);
  //Serial.println(rightMotorSpeed);
  md.setM1Speed(rightMotorSpeed);
  Serial.println(abs(frontBackErr));
  if (abs(frontBackErr) > 5) { // if the difference between the current front distance and the previous one is large, this means a wall is no longer present
    IRChange = 1; // set paddleboard completion flag to TRUE
    //drive blindly forward
    md.setM1Speed(50);
    md.setM2Speed(-45);
    delay(3000);
  }
  frontIRDistOld = frontDist; //set the old front distance
}

void lineFollow() { //line following
  qtrrc.read(sensorValues);//read in sensor values from array
  //sensor values from 0 to 25000, 0 means max reflectance (white)
  //2500 means minimum reflectance (black)
  for (unsigned char i = 0; i < NUM_SENSORS; i++) { //bias the sensor values for a better reading
    sensorBias[i] = sensorValues[i] - sensorLowVals[i] + 200;
  }
  double lineLoc = 0;//set initial line location to 0
  double prevLoc = lineLoc;
  int sumVals;
  double sumMult = 0;
  //find line location
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    sumVals += sensorBias[i];
    sumMult += sensorBias[i] * sensorNums[i];
  }
  lineLoc = float(sumMult) / float(sumVals) - 4.25;
  Serial.println(lineLoc);
  double locDiff = abs(lineLoc - prevLoc);
  if (lineLoc < -.25 && lineLoc > -9) { //if the line center is a bit to the left, drive right
    md.setM2Speed(-40);
    md.setM1Speed(0);
  }
  else if (lineLoc > .25 && lineLoc < 9) { //if the line center is a bit to the right, drive left
    md.setM1Speed(40);
    md.setM2Speed(0);
  }
  else if (sensorBias[0] < 500 && sensorBias[1] < 500 && sensorBias[2] < 500 && sensorBias[3] < 500 && sensorBias[4] < 500 && sensorBias[5] < 500 && sensorBias[6] < 500 && sensorBias[7] < 500) { //if all sensors detect white
    md.setM1Brake(400);
    md.setM2Speed(400);
    linePass = 1;//set line pass flag to TRUE
  }
  else
  { //drive straight if within tolerances
    md.setM1Speed(40);
    md.setM2Speed(-40);
  }
  //delay(250);
}

void railRun() { //railrunner code
  md.setM4Speed(200); //raise the jack 
  delay(3000); //give the jack time to extend
  md.setM4Brake(400);  //stop the jack
  md.setM4Speed(0);
  armServo.write(railAngle);//move the servo up to hook onto the rail
  delay(1000);
  md.setM1Speed(50);//drive onto the rail
  md.setM2Speed(-45);
  delay(1000);
  md.setM1Speed(0);
  md.setM2Speed(0);
  md.setM3Speed(150);//run rail runner motor
  delay(7000);
  md.setM3Speed(0);
  railRunCheck = 1;
}

void walLift(){
  if(linePass == 1){//if we have passed the line
    armServo.write(warpedAngle); //set the arm to an angle to attempt to hook on
    md.setM4Speed(200);//drive jack up
    delay(3000);//give time for the jack to extend
    md.setM4Speed(0);//stop the jack
  }
  else{//follow the line until the wall is reached
    lineFollow();
  }
}

