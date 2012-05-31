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

  pinMode(_conf->movePwmLeftPin, OUTPUT);
  pinMode(_conf->movePwmRightPin, OUTPUT);
  pinMode(_conf->moveModeFirstLeftPin, OUTPUT);
  pinMode(_conf->moveModeSecondLeftPin, OUTPUT);
  pinMode(_conf->moveModeFirstRightPin, OUTPUT);
  pinMode(_conf->moveModeSecondRightPin, OUTPUT);
}

/**
 * Stops both motors immediately.
 */
void Move::performFastStop()
{
  _com->send(60);
  digitalWrite(_conf->moveModeFirstLeftPin, HIGH);
  digitalWrite(_conf->moveModeSecondLeftPin, HIGH);
  digitalWrite(_conf->moveModeFirstRightPin, HIGH);
  digitalWrite(_conf->moveModeSecondRightPin, HIGH);
}

/**
 * Set the direction and the speed of both motors. If the value speedLeftMotor or speedRightMotor is not in the range
 * between 0 and 255, the maximum / minimum value of 255 or 0 will be set.
 * If the direction is set to valueDependant, negative numbers are allowed which will cause the motor to rotate backwards (0 will set the motor to phaseOut).
 * First the speed is set then the direction. First the left motor is set then the right.
 * @param dirLeftMotor direction of left motor
 * @param speedLeftMotor speed of left motor (0-255)
 * @param dirRightMotor direction of right motor
 * @param speedRightMotor speed of right motor (0-255)
 */
void Move::controlMotors(TMotorDirection dirLeftMotor, int speedLeftMotor, TMotorDirection dirRightMotor, int speedRightMotor){
  if(dirLeftMotor = valueDependant){
    if(speedLeftMotor > 0){
      dirLeftMotor = forward;
    }
    else if(speedLeftMotor < 0){
      dirLeftMotor = backwards;
      speedLeftMotor = (-1) * speedLeftMotor;
    }
    else{ //0
      dirLeftMotor = phaseOut;
    }
  }
  
  if(dirRightMotor = valueDependant){
    if(speedRightMotor > 0){
      dirRightMotor = forward;
    }
    else if(speedRightMotor < 0){
      dirRightMotor = backwards;
      speedRightMotor = (-1) * speedRightMotor;
    }
    else{ //0
      dirRightMotor = phaseOut;
    }
  }

  if(speedLeftMotor > 255) speedLeftMotor = 255;
  if(speedRightMotor > 255) speedRightMotor = 255;

  if(speedLeftMotor < 0) speedLeftMotor = 0;
  if(speedRightMotor < 0) speedRightMotor = 0;

  _speedLeft = speedLeftMotor;
  _speedRight = speedRightMotor;

  int parameters[4];
  parameters[0] = _speedLeft;
  parameters[1] = dirLeftMotor;
  parameters[2] = _speedRight;
  parameters[3] = dirRightMotor;

  _com->send(58, parameters, 4);

  analogWrite(_conf->movePwmLeftPin, _speedLeft);
  setLeftMotorDirection(dirLeftMotor);
  analogWrite(_conf->movePwmRightPin, _speedRight);
  setRightMotorDirection(dirRightMotor);
}

/**
 * Set the direction of the left motor according to the given enum value.<br>
 * <b>The enum value valueDependant will be ignored.</b>
 * @param dir direction to set for the left motor
 */
void Move::setLeftMotorDirection(enum TMotorDirection dir){
  int dirCode;
  _dirLeftMotor = dir;
  if(dir == phaseOut){
    digitalWrite(_conf->moveModeFirstLeftPin, LOW);
    digitalWrite(_conf->moveModeSecondLeftPin, LOW);
    dirCode = 0;
  }
  else if(dir == forward){
    digitalWrite(_conf->moveModeFirstLeftPin, HIGH);
    digitalWrite(_conf->moveModeSecondLeftPin, LOW);
    dirCode = 1;
  }
  else if(dir == backwards){
    digitalWrite(_conf->moveModeFirstLeftPin, LOW);
    digitalWrite(_conf->moveModeSecondLeftPin, HIGH);
    dirCode = 2;
  }
  else if(dir == fastStop){
    digitalWrite(_conf->moveModeFirstLeftPin, HIGH);
    digitalWrite(_conf->moveModeSecondLeftPin, HIGH);
    dirCode = 3;
  }

  //_com->send(51, dirCode);
}

/**
 * Set the direction of the right motor according to the given enum value.
 * <b>The enum value valueDependant will be ignored.</b>
 * @param dir direction to set for the right motor
 */
void Move::setRightMotorDirection(TMotorDirection dir){
  int dirCode;
  _dirRightMotor = dir;
  if(dir == phaseOut){
    digitalWrite(_conf->moveModeFirstRightPin, LOW);
    digitalWrite(_conf->moveModeSecondRightPin, LOW);
    dirCode = 0;
  }
  else if(dir == forward){
    digitalWrite(_conf->moveModeFirstRightPin, HIGH);
    digitalWrite(_conf->moveModeSecondRightPin, LOW);
    dirCode = 1;
  }
  else if(dir == backwards){
    digitalWrite(_conf->moveModeFirstRightPin, LOW);
    digitalWrite(_conf->moveModeSecondRightPin, HIGH);
    dirCode = 2;
  }
  else if(dir == fastStop){
    digitalWrite(_conf->moveModeFirstRightPin, HIGH);
    digitalWrite(_conf->moveModeSecondRightPin, HIGH);
    dirCode = 3;
  }

  //_com->send(52, dirCode);
}

/**
 * Changes the <b>current</b> speed of the two motors according to the given difference which can be positiv or negativ. The speed will not be less than 0 or more than 255 at the end.
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

  parameters[2] = _speedLeft + changeLeftMotor;
  parameters[5] = _speedRight + changeRightMotor;

  _com->send(53, parameters, 6);

  controlMotors(_dirLeftMotor, parameters[2], _dirRightMotor, parameters[5]);
}

/**
 * Sets the speed of the left and right motor to the given speeds.
 * The speed will not be less than 0 or more than 255 at the end.
 * The direction of both motors is not affected. If they are not moving at the moment, a call to this method will not cause them rotating.
 * @param speedLeftMotor The value that shall be assigned to the left motor.
 * @param speedRightMotor The value that shall be assigned to the right motor.
 */
void Move::setMotorSpeed(int speedLeftMotor, int speedRightMotor){
  int parameters[2];
  parameters[0] = speedLeftMotor;
  parameters[1] = speedRightMotor;

  _com->send(70, parameters, 2);

  controlMotors(_dirLeftMotor, speedLeftMotor, _dirRightMotor, speedRightMotor);
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
    _speedLeft, _speedRight, averageSpeed
  };

  _com->send(54, parameters, 3);
  _speedLeft = averageSpeed;
  _speedRight = averageSpeed;

  analogWrite(_conf->movePwmLeftPin, _speedLeft);
  analogWrite(_conf->movePwmRightPin, _speedRight);
}






