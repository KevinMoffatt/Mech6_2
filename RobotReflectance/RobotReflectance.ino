#define MEGA
#include <QTRSensors.h>
#define NUM_SENSORS 8 //number sensors used
#define TIMEOUT     2500 //waits 2.5 sec before setting output to low

#define EMITTER_PIN 41 //emitter controlled by pin 2

//sensors 0 thru 7 connected to digital pins 3 thru 10
QTRSensorsRC qtrrc((unsigned char[]) {39, 37, 35, 33, 31, 29, 27, 25}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

int sensorLowVals[NUM_SENSORS] = {340, 340, 240, 240, 400, 300, 450, 600};

int sensorBias[8];
int sensorNums[NUM_SENSORS] = {1,2,3,4,5,6,7,8};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //read raw sensor vals
  qtrrc.read(sensorValues);
  //print sensor values from 0 to 25000, 0 means max reflectance (white)
  //2500 means minimum reflectance (black)
  for(unsigned char i = 0; i<NUM_SENSORS; i++){
    //Serial.print(sensorValues[i]);
    //Serial.print(" ");
    sensorBias[i]=sensorValues[i]-sensorLowVals[i]+200;
    
    Serial.print(sensorBias[i]);
    Serial.print(" ");
  }
  Serial.println();
  double lineLoc = 0;
  int sumVals = 0;
  double sumMult = 0;
  for(unsigned char i = 0; i<NUM_SENSORS; i++){
    sumVals+=sensorBias[i];
    sumMult+=sensorBias[i]*sensorNums[i];
  }
  lineLoc = float(sumMult)/float(sumVals)-4.5;
  Serial.println(lineLoc);
  if(lineLoc < -.25){
    Serial.println("go left");
  }
  else if(lineLoc > .25){
    Serial.println("go right");
  }
  else
  {
    Serial.println("keep on truckin'");
  }
  
  delay(250);
}
