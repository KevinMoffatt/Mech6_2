//Motor Test Example
#define MEGA
#include <DualVNH5019MotorShieldMod3.h>
//#include <Encoder.h>

// configure library with pins as remapped
unsigned char INA3 = 22; // motor 3 directional input 1
unsigned char INB3 = 23; // motor 3 directional input 2
unsigned char EN3DIAG3 = 26;
unsigned char CS3 = 27;
unsigned char PWM3 = 13; // motor 3 power
unsigned char INA4 = 52; // motor 4 directional input 1
unsigned char INB4 = 53; // motor 4 directional input 2
unsigned char EN4DIAG4 = 29;
unsigned char CS4 = 30;
unsigned char PWM4 = 46; // motor 4 power
DualVNH5019MotorShieldMod3 md(INA3, INB3, EN3DIAG3, CS3, PWM3, INA4, INB4, EN4DIAG4, CS4, PWM4); //Use default pins for motor shield 1 and remapped pins for motor shield 2

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  md.init();

}

void loop()
{
// loops endlessly; main loop goes here

// the following code is a simple example:
//
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

}
