#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotSensor left_sensor = RedBotSensor(A1);
RedBotSensor center_sensor = RedBotSensor(A3);
RedBotSensor right_sensor = RedBotSensor(A6);
RedBotMotors motors;

#define s_drive 0
#define s_reverse 1
#define s_turn_left 2 
#define s_turn_right 3

int robot_state = s_drive;
float line_median = 0;
float line_max = 0;
float line_min = 800;

float TotalSensordata;
float RightSensordata;
float LeftSensordata;
float CenterSensordata;


void setup() {
  Serial.begin(9600);
  //calibrate();
}

//Calibrates the sensor
//void calibrate()
//{
//  Serial.print("Starting line sensor calibration");
//  int duration = 300;
//  for (int i = 0; i < duration; i++)
//  {
//    int value = read_sensor();
//    if (value <= line_min)
//    {
//      line_min = value;
//      }
//    if (value >= line_max)
//    {
//      line_max = value;
//      }
//      
//    motor_turn_left();
//    motor_turn_right();
//    motor_reverse();
//    }
//  line_median = ((line_max - line_min) / 2) + line_min;
//  Serial.print("Calibrated");
//  }

void loop() {
  transition(robot_state);
  
  do_behaviour(robot_state);
  
  Serial.print(left_sensor.read());
  Serial.print("\t");
  Serial.print(center_sensor.read());
  Serial.print("\t");
  Serial.print(right_sensor.read());
  Serial.println();
  delay(100); //Delays sensor readings
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
      if (test_sensor() == "reverse")
       {
        robot_state = s_reverse;
        }
      break;

    case s_turn_left:
      if (test_sensor() == "drive")
      {
        robot_state = s_drive;
        }
      if (test_sensor() == "reverse")
       {
        robot_state = s_reverse;
        }
      break;

     case s_turn_right:
      if (test_sensor() == "drive")
      {
        robot_state = s_drive;
        }
      if (test_sensor() == "reverse")
       {
        robot_state = s_reverse;
        }
      break;

      case s_reverse:
      if (test_sensor() == "drive")
      {
        robot_state = s_drive;
        }
      if (test_sensor() == "turn_left")
      {
        robot_state = s_turn_left;
        }
      if (test_sensor() == "turn_right")
       {
        robot_state = s_turn_right;
        }
      break;
    }
  }


void do_behaviour(int state)
{
  switch (state)
  {
    case s_drive:
      motor_drive();
      break;

    case s_turn_left:
      motor_turn_left();
      break;

     case s_turn_right:
      motor_turn_right();
      break;

     case s_reverse:
      motor_reverse();
      break;
  }
}


String test_sensor()
{
  String sensor_reading = "";

  if (center_sensor.read() > line_min && right_sensor.read() > line_min && left_sensor.read() > line_min)
  {
    sensor_reading = "drive";
    }
  else if (center_sensor.read() > line_min && right_sensor.read() > line_min)
  {
    sensor_reading = "turn_right";
    }  
  else if (center_sensor.read() > line_min && left_sensor.read() > line_min)
  {
    sensor_reading = "turn_left";
    }
    
  else if (center_sensor.read() < line_min && right_sensor.read() < line_min && left_sensor.read() < line_min)
  {
    sensor_reading = "reverse";
    }
    
  Serial.println(left_sensor.read());
  Serial.println(center_sensor.read());
  Serial.println(right_sensor.read());
  return sensor_reading;
  }


//Drive methods
void motor_drive()
{
  Serial.println("Driving...");
  motors.drive(-100);
  }


void motor_turn_left()
{
  Serial.println("TURN LEFT");
  motors.pivot(100);
  
  }


void motor_turn_right()
{
  Serial.println("TURN RIGHT");
  motors.pivot(-100);
  
  }


void motor_reverse()
{
  Serial.println("Reverse");
  motors.drive(80);
  
  }
