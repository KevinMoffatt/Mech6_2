#define MEGA
#include <DualVNH5019MotorShieldMod3.h>
#include <Encoder.h>
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
double leftPos_old = 0;
double leftVel = 0;
double rightPos = 0;
double rightPos_old = 0;
double rightVel = 0;

//PID variables
double error = 0;
double errorOld = error; 
double posDes = PI;
double derivativeError = 0;
double Kp = 8.25;
double Kd = 3;
double Ki = 0;
double MotorVoltage = 0;
double f = 2;
double rWheel = 42.0;

double counts = 0;

//Control Variables
int M = 0;

void setup() {
  md.init();                  //initialize motor controller
  Serial.begin(19200);         //initialize serial monitor 
}

void loop() {
  t = micros()/1000000.0;        //current time 
  
   //Step input
    
  if(t<=1.0){
    posDes = 0;  
  }
  else{
    posDes = PI;
  }
  
  // --------Position Controller----------------

  //Encoder sensing
  leftCounts = leftEnc.read();                            //get current counts
  leftPos = float(leftCounts)*2*PI/(float(countsPerRev_motor)*GearRatio); //Position in rad
  leftVel = (leftPos-leftPos_old)/(t-t_old);                    //Velocity in rad/sec
  rightCounts = rightEnc.read();                            //get current counts
  rightPos = -float(rightCounts)*2*PI/(float(countsPerRev_motor)*GearRatio); //Position in rad
  rightVel = (rightPos-rightPos_old)/(t-t_old);                    //Velocity in rad/sec

  error = posDes - (leftPos+rightPos)/2;
  derivativeError = (error-errorOld)/(t-t_old);
  errorOld = error;
  MotorVoltage = Kp*error + Kd*derivativeError;
  
  // ---------Motor Command---------------------
                              
  M = MotorVoltage*400.0/10.0;            //convert Voltage to motor command
  M = constrain(M,-400,400);  //constrian Motor command to -400 to 400
  md.setM1Speed(M);  
  md.setM2Speed(-M);      
  //command voltage to motor
  

  //-----------Printing-------------------------
  if(t<3){
    Serial.print(t,5);
    Serial.print("\t");
    Serial.print((leftPos+rightPos)/2,5);
    Serial.print("\t");
    Serial.print(M); 
    Serial.print(error);
    Serial.print("\t");
    Serial.print(derivativeError); 
    Serial.println();
  }
  //save current time and position
  
      t_old = t;
}






