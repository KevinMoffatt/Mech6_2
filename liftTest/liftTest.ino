#define MEGA
#include <DualVNH5019MotorShieldMod3.h>

int servoArmPin = 11; // THIS CORRESPONDTDS TO THE SERVO'S ORANGE WIRE

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

void setup()  
{
  // Open serial communications with computer and wait for port to open:
  // Serial.begin(9600);

  // Serial.println("MESSAGE CODES:");
  // Serial.println("");

  // Print a message to the computer through the USB
  // Serial.println("enter message!");

  // Open serial communications with the other Arduino board
  md.init();
}

void loop() // run over and over
{
  md.setM4Speed(400);
  delay(5000);
  md.setM4Speed(-400);
  delay(5000);
  md.setM4Speed(0);
  while(servoArmPin == 11){
  }
}

