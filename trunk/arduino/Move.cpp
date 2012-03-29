#include "Arduino.h"
#include "Move.h"
#include "Configuration.h"
#include "Communication.h"

Move Move::_instance; ///< Static reference to the singleton object
boolean Move::_instanceCreated = false;

/**
 * Get the Move instance
 * @return Pointer to the move instance
 */
Move* Move::getInstance()
{
  if ( !_instanceCreated ){
    _instance = Move();
    _instanceCreated = true;

    //do the configuration stuff so that a call to begin() from the outside of the
    //class is not necessary
    _instance.begin();
  }
  return &_instance;
}


/**
 * Constructur for the class.
 */
Move::Move()
{

}

/**
 * Prepares the Move object to work according to the values defined in the configuration.
 * The preparations done in here can not be done from within the constructor because of
 * dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b> In this class it is done
 * when getting an instance of the class the first time.
 * @see getInstance()
 */
void Move::begin(){
  _conf = Configuration::getInstance();
  _com = Communication::getInstance();

  _pwmLeftPin = _conf->getMovePwmLeftPin();
  _pwmRightPin = _conf->getMovePwmRightPin();
  _modeFirstLeftPin = _conf->getMoveModeFirstLeftPin();
  _modeSecondLeftPin = _conf->getMoveModeSecondLeftPin();
  _modeFirstRightPin = _conf->getMoveModeFirstRightPin();
  _modeSecondRightPin = _conf->getMoveModeSecondRightPin();

  pinMode(_pwmLeftPin, OUTPUT);
  pinMode(_pwmRightPin, OUTPUT);
  pinMode(_modeFirstLeftPin, OUTPUT);
  pinMode(_modeSecondLeftPin, OUTPUT);
  pinMode(_modeFirstRightPin, OUTPUT);
  pinMode(_modeSecondRightPin, OUTPUT);
}

/**
 * Stops both motors immediately.
 */
void Move::performFastStop()
{
  _com->send(60);
  digitalWrite(_modeFirstLeftPin, HIGH);
  digitalWrite(_modeSecondLeftPin, HIGH);
  digitalWrite(_modeFirstRightPin, HIGH);
  digitalWrite(_modeSecondRightPin, HIGH);
}

/**
 * Set the diraction and the speed of both motors.
 * First the speed is set then the direction. First the left motor is set then the right.
 * @param dirLeftMotor direction of left motor
 * @param speedLeftMotor speed of left motor (0-255)
 * @param dirRightMotor direction of right motor
 * @param speedRightMotor speed of right motor (0-255)
 */
void Move::controlMotors(TMotorDirection dirLeftMotor, byte speedLeftMotor, TMotorDirection dirRightMotor, byte speedRightMotor){
  //_com = Communication::getInstance();
  _speedLeft = speedLeftMotor;
  _speedRight = speedRightMotor;

  _com->send(58, _speedLeft);
  analogWrite(_pwmLeftPin, _speedLeft);
  setLeftMotorDirection(dirLeftMotor);
  _com->send(59, _speedLeft);
  analogWrite(_pwmRightPin, _speedRight);
  setRightMotorDirection(dirRightMotor);
}

/**
 * Set the direction of the left motor according to the given enum value.
 * @param dir direction to set for the left motor
 */
void Move::setLeftMotorDirection(enum TMotorDirection dir){
  int dirCode;
  _dirLeftMotor = dir;
  if(dir == phaseOut){
    digitalWrite(_modeFirstLeftPin, LOW);
    digitalWrite(_modeSecondLeftPin, LOW);
    dirCode = 0;
  }
  else if(dir == forward){
    digitalWrite(_modeFirstLeftPin, HIGH);
    digitalWrite(_modeSecondLeftPin, LOW);
    dirCode = 1;
  }
  else if(dir == backwards){
    digitalWrite(_modeFirstLeftPin, LOW);
    digitalWrite(_modeSecondLeftPin, HIGH);
    dirCode = 2;
  }
  else if(dir == fastStop){
    digitalWrite(_modeFirstLeftPin, HIGH);
    digitalWrite(_modeSecondLeftPin, HIGH);
    dirCode = 3;
  }

  _com->send(51, dirCode);
}

/**
 * Set the direction of the right motor according to the given enum value.
 * @param dir direction to set for the right motor
 */
void Move::setRightMotorDirection(TMotorDirection dir){
  int dirCode;
  _dirRightMotor = dir;
  if(dir == phaseOut){
    digitalWrite(_modeFirstRightPin, LOW);
    digitalWrite(_modeSecondRightPin, LOW);
    dirCode = 0;
  }
  else if(dir == forward){
    digitalWrite(_modeFirstRightPin, HIGH);
    digitalWrite(_modeSecondRightPin, LOW);
    dirCode = 1;
  }
  else if(dir == backwards){
    digitalWrite(_modeFirstRightPin, LOW);
    digitalWrite(_modeSecondRightPin, HIGH);
    dirCode = 2;
  }
  else if(dir == fastStop){
    digitalWrite(_modeFirstRightPin, HIGH);
    digitalWrite(_modeSecondRightPin, HIGH);
    dirCode = 3;
  }

  _com->send(52, dirCode);
}

/**
 * Changes the speed of the two motors according to the difference which can be a positiv or negativ value. The speed will not be less than 0 or more than 255 at the end.
 * The direction of both motors is not affected. If they are not moving at the moment, a call to this method will not cause them rotating.
 * @param changeLeftMotor The value that shall be added to the current speed of the left motor. Can be a positive or negativ value.
 * @param changeRightMotor The value that shall be added to the current speed of the right motor. Can be a positive or negativ value.
 */
void Move::changeMotorSpeed(int changeLeftMotor, int changeRightMotor){
  int parameters[6];
  parameters[0] = _speedLeft;
  parameters[1] = changeLeftMotor;
  parameters[3] = _speedRight;
  parameters[4] = changeRightMotor;

  if(changeLeftMotor > 0 && ((int)_speedLeft) + changeLeftMotor > 255){
    changeLeftMotor = 255 - _speedLeft; 
  }
  else if(changeLeftMotor < 0 && ((int)_speedLeft) + changeLeftMotor < 0){
    changeLeftMotor = _speedLeft * (-1); 
  }
  if(changeRightMotor > 0 && ((int)_speedRight) + changeRightMotor > 255){
    changeRightMotor = 255 - _speedRight; 
  }
  else if(changeRightMotor < 0 && ((int)_speedRight) + changeRightMotor < 0){
    changeRightMotor = _speedRight * (-1); 
  }

  _speedLeft += changeLeftMotor;
  _speedRight += changeRightMotor;
  analogWrite(_pwmLeftPin, _speedLeft);
  analogWrite(_pwmRightPin, _speedRight);

  parameters[2] = _speedLeft;
  parameters[5] = _speedRight;

  _com->send(53, parameters, 6);
}

/**
 * Equalizes the speed of both motors by calculating the average speed of both motors and applying that to both motors.
 * The direction of the motors will not be changed.
 * The method can be used e. g. to move the car straight forward after a courve.
 */
void Move::equalizeMotorsSpeed(){
  //calculating the average speed. we don't care of misscalculation of 0.5
  byte averageSpeed = (byte)((((int)_speedLeft) + ((int)_speedRight)) / 2);

  int parameters[] = {
    _speedLeft, _speedRight, averageSpeed        };

  _com->send(54, parameters, 3);
  _speedLeft = averageSpeed;
  _speedRight = averageSpeed;

  analogWrite(_pwmLeftPin, _speedLeft);
  analogWrite(_pwmRightPin, _speedRight);
}


