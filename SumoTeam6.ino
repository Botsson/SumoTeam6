#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>

ZumoReflectanceSensorArray sensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

#define NUM_SENSORS 6
unsigned int sensorValues[NUM_SENSORS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.init();

  motors.setSpeeds(0, 0);

  

}

void loop() {
  // put your main code here, to run repeatedly:
  sensors.read(sensorValues);

  Serial.println(sensorValues[0]);

  motors.setSpeeds(100, 100);

}
