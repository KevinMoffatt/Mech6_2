#include <PID_v1.h>
double SetPoint, Input, Output;
PID leftMotor(&Input, &Output, &SetPoint, 2, 5, 1, P_ON_M, DIRECT);

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
