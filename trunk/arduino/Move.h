#ifndef Move_h
#define Move_h

#include "Arduino.h"
#include "Communication.h"

/**
 * The enum with values about the direction one motor rotates.
 */
enum TMotorDirection{ 
  phaseOut, ///< Phase out the motor. The power is taken away from the motor but the car can roll a couple of centimers still.  
  forward, ///< The motor rotates forward.
  backwards, ///< The motor rotates backward.
  fastStop ///< The motor is stopped immediately. This is done by shortly giving power in the other direction.
};

class Move
{
private:
  Move();
  static Move _instance;
  static boolean _instanceCreated;
  void begin();
  TMotorDirection _dirLeftMotor; ///< current direction of left motor
  TMotorDirection _dirRightMotor; ///< current direction of right motor
  byte _speedLeft; ///< current speed of left motor (pwm value 0-255)
  byte _speedRight; ///< current speed of right motor (pwm value 0-255)

  Communication* _com; ///< The reference to the serial communication class.
  Configuration* _conf; ///< The reference to the configuration class.
public:
  static Move* getInstance();

  void performFastStop();
  void controlMotors(TMotorDirection dirLeftMotor, int speedLeftMotor, TMotorDirection dirRightMotor, int speedRightMotor);
  void setLeftMotorDirection(TMotorDirection dir);
  void setRightMotorDirection(TMotorDirection dir);
  void changeMotorSpeed(int changeLeftMotor, int changeRightMotor);
  void setMotorSpeed(int speedLeftMotor, int speedRightMotor);
  void equalizeMotorsSpeed();
};
#endif
















