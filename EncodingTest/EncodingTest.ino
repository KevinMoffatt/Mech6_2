#define MEGA
#include <DualVNH5019MotorShieldMod3.h>
#include <Encoder.h>
#include <SoftwareSerial.h>
#include <Servo.h>

double messData;

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
double rWheel = 84.0;

void setup() {
  md.init();                      //initialize motor controller
  Serial.begin(9600);             //initialize serial monitor  
}

void loop() {
  int M = 400*sin(2*PI*f*t);      //Sinusoid motor voltage command
  //Serial.println(M);              //print to screen
  //md.setM1Speed(M);               //command voltage to motor
  //md.setM2Speed(M);               //command voltage to motor
  t_old = t;
  leftPos_old = leftPos;
  rightPos_old = rightPos;
  leftCounts = leftEnc.read();
  rightCounts = rightEnc.read();
  leftPos = leftCounts*2.*PI/(GearRatio*countsPerRev_motor);
  rightPos = rightCounts*2.*PI/(GearRatio*countsPerRev_motor);
  t = micros()/1000000.0;
  leftVel = (leftPos-leftPos_old)/(t-t_old);
  rightVel = (rightPos-rightPos_old)/(t-t_old);
  Serial.print(leftCounts); 
  Serial.print("\t");
  Serial.println(rightCounts);
  Serial.print(leftPos);
  Serial.print("\t");
  Serial.println(rightPos);
//  Serial.print("Time");
//  Serial.print("\t");
//  Serial.print("LPosition");
//  Serial.print("\t");
//  Serial.print("LVelocity");
//  Serial.print("\t");
//  Serial.print("RPosition");
//  Serial.print("\t");
//  Serial.println("RVelocity");
//  Serial.print(t);
//  Serial.print("\t");
//  Serial.print(leftPos);
//  Serial.print("\t");
//  Serial.print("\t");
//  Serial.print(leftVel);
//  Serial.print("\t");
//  Serial.print("\t");
//  Serial.print(rightPos);
//  Serial.print("\t");
//  Serial.print("\t");
//  Serial.println(rightVel);
}






