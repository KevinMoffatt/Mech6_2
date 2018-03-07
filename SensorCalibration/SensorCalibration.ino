#define MEGA
#define RIGHT_IR A1 //Right IR Rangefinder Pin
#define LEFT_IR A2 //Left IR Rangefinder Pin

double leftIRVolt = 0;
double rightIRVolt = 0;
double leftDist = 0;
double rightDist = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.print("Left Sensor: ");
  leftIRVolt = (analogRead(LEFT_IR))*5.0/1023.0;
  leftDist = pow(leftIRVolt,-1.1809)*12.52;
  Serial.print(leftIRVolt);
  Serial.print("\t");
  Serial.println(leftDist);
  Serial.print("Right Sensor: ");
  rightIRVolt = (analogRead(RIGHT_IR))*5.0/1023.0;
  rightDist = pow(rightIRVolt,-1.1528)*11.71;
  Serial.print(rightIRVolt);
  Serial.print("\t");
  Serial.println(rightDist);
  delay(1000);
}
