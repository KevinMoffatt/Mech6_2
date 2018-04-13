#define LEFT_IR A2 //Left IR Rangefinder Pin

double leftIRVolt = 0;
double leftDist = 0;

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
  delay(1000);
}
