#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 75 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#define LED 13
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1500 // microseconds
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     250 // 0 is stopped, 400 is full speed
#define TURN_SPEED        350
#define FORWARD_SPEED     400
#define REVERSE_DURATION  300 // ms
#define TURN_DURATION     300 // ms
#define PUSH_SPEED        50
 
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

int forward = 0;
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
  
  buzzer.playNote(NOTE_G(4), 500, 15);  
  delay(3000);
}
 
void setup()
{
  Serial.begin(115200);
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
   
  pinMode(LED, HIGH);
   
  waitForButtonAndCountDown();
}

void turnVechile(int direction) {
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(REVERSE_DURATION + random(10, 250));

  if(shouldAttack()) return;
      
  if(direction == 0) { // turn left
      // if rightmost sensor detects line, reverse and turn to the left
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);

      if(shouldAttack()) return;
      
  } else { // turn right
      // if leftmost sensor detects line, reverse and turn to the right
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);

      if(shouldAttack()) return;
  }
  delay(TURN_DURATION + random(10, 250));
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  forward = 0;
}

boolean shouldAttack() {
  if(sonar.ping_cm() <= 5) {
      motors.setSpeeds(PUSH_SPEED, PUSH_SPEED);
      forward = forward + 1;
      return true;
  }
  return false;
}

void loop()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }

  sensors.read(sensor_values);

  if (sensor_values[0] < QTR_THRESHOLD)
  {
    turnVechile(1);
    /*
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION + random(10, 100));
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION + random(10, 100));
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    forward = 0;
    */
  }
  else if (sensor_values[5] < QTR_THRESHOLD)
  {
    turnVechile(0);
    /*
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION + random(10, 100));
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION + random(10, 100));
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    forward = 0;
    */
  }
  else if(sonar.ping_cm() <= 10) {
      motors.setSpeeds(PUSH_SPEED, PUSH_SPEED);
      forward = forward + 1;
  
      if(forward >= 200) {
        // if leftmost sensor detects line, reverse and turn to the right
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        /*motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        */
        forward = 0;
      }
  }
  else
  {
    //forward = 0;
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  
}
