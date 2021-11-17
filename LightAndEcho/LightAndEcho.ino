#include <RedBot.h>
#include <RedBotSoftwareSerial.h>
#include <NewPing.h>

RedBotMotors motors;
RedBotSensor left_sensor = RedBotSensor(A2);
RedBotSensor right_sensor = RedBotSensor(A3);

#define s_drive 0
#define s_turn_left 1
#define s_turn_right 2
#define s_bumped 3

int robot_state = s_drive;

int LEFT_TRIG_PIN = A1;
int LEFT_ECHO_PIN = A0;

int RIGHT_TRIG_PIN = 11;
int RIGHT_ECHO_PIN = 10;

NewPing sonar_left(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
NewPing sonar_right(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);

RedBotAccel accel;

void setup() {
  Serial.begin(9600);
  accel.enableBump();
}

void loop() {        
  Serial.print("Left: ");
  Serial.print(sonar_left.ping_cm());
  Serial.print("\t");
  Serial.print("Right: ");
  Serial.print(sonar_right.ping_cm());
  Serial.println();

  Serial.print("Left Sensor");
  Serial.print(left_sensor.read());
  Serial.print("Right Sensor");
  Serial.print(right_sensor.read());
  Serial.println();
  transition(robot_state);
  
  do_behaviour(robot_state);
}

void transition(int state)
{
  switch (state)
  {
    case s_drive:   
      if (test_sensor() == "turn_left")
      {
        robot_state = s_turn_left;
        }
      if (test_sensor() == "turn_right")
       {
        robot_state = s_turn_right;
        }
      if (testBumped() == true)
        {
         robot_state = s_bumped;
          }
      break;

    case s_turn_left:
      if (test_sensor() == "drive")
      {
        robot_state = s_drive;
        }
      if (testBumped() == true)
        {
         robot_state = s_bumped;
          }
      break;

     case s_turn_right:
      if (test_sensor() == "drive")
      {
        robot_state = s_drive;
        }
      if (testBumped() == true)
        {
         robot_state = s_bumped;
          }
      break;

      case s_bumped:
      if (test_sensor() == "drive")
      {
        robot_state = s_drive;
        }
      break;
    }
  }

 void do_behaviour(int state) {
  switch (state)
  {
    case s_drive:
      follow_light();
      break;

    case s_turn_left:
      motor_turn_left();
      break;

     case s_turn_right:
      motor_turn_right();
      break;

     case s_bumped:
      testBumped();
      break;
  }
}

String test_sensor()
{
  int minDist = 20;
  String sensor_reading = "";

  if (sonar_left.ping_cm() > minDist && sonar_right.ping_cm() > minDist)
  {
    sensor_reading = "drive";
    }
  else if (sonar_left.ping_cm() <= minDist && sonar_left.ping_cm() > 0 && sonar_right.ping_cm() > minDist)
  {
    sensor_reading = "turn_right";
    }  
  else if (sonar_left.ping_cm() > minDist && sonar_right.ping_cm() <= minDist && sonar_right.ping_cm() > 0)
  {
    sensor_reading = "turn_left";
    }
  return sensor_reading;
  }

 void follow_light()
 {
  if(left_sensor.read() > 350 && right_sensor.read() > 350)
  {
      motor_drive();
  }

  if(left_sensor.read() < 350 && right_sensor.read() < 350)
  {
    Serial.print("Going towards the light");
      motor_drive();
  }
  else if(right_sensor.read() > left_sensor.read())
  {
    Serial.print("Turning right");
    motors.leftDrive(120);
    motors.rightDrive(90);
  }
  else if (left_sensor.read() > right_sensor.read())
  {
    Serial.print("Turning left");
    motors.leftDrive(90);
    motors.rightDrive(120); 
  }
 }

 bool testBumped() {
  
  bool bump = accel.checkBump();
  if (bump == true) {
      Serial.print("Bumped");
      motors.leftDrive(75);
      motors.rightDrive(50);
      delay(700);
  }
  return bump;
}

void motor_drive()
{
  Serial.println("Driving...");
  motors.leftDrive(120);
  motors.rightDrive(120);
  }

void motor_turn_left()
{
  Serial.println("TURN LEFT");
  motors.leftDrive(-90);
  motors.rightDrive(120);
  }

void motor_turn_right()
{
  Serial.println("TURN RIGHT");
  motors.leftDrive(120);
  motors.rightDrive(-90);
  }
