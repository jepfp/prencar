#include "Arduino.h"
#include "LineFollow.h"
#include "Move.h"
#include "Configuration.h"

/**
 * Constructur for the class.
 */
LineFollow::LineFollow()
{

}

/**
 * Prepares the LineFollow object to work together with other classes. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void LineFollow::begin(){
  _com = Communication::getInstance();
  _move = Move::getInstance();
  _conf = Configuration::getInstance();

  pinMode(_conf->lineFollowLeftSensorPin, INPUT);
  pinMode(_conf->lineFollowRightSensorPin, INPUT);
  pinMode(_conf->lineFollowLeftFrontSensorPin, INPUT);
  pinMode(_conf->lineFollowRightFrontSensorPin, INPUT);

  //make sure, the pull up resistors are off
  digitalWrite(_conf->lineFollowLeftSensorPin, LOW);
  digitalWrite(_conf->lineFollowRightSensorPin, LOW);
  digitalWrite(_conf->lineFollowLeftFrontSensorPin, LOW);
  digitalWrite(_conf->lineFollowRightFrontSensorPin, LOW);

  hasReachedCurve = false;
  frontLineSensorsEnabled = true;
}

/**
 * Starts the line following process with the given initial speed.
 * @param initSpeedLeft Initial speed for the left motor.
 * @param initSpeedRight Initial speed for the right motor.
 */
void LineFollow::startIt(int initSpeedLeft, int initSpeedRight){
  startIt(initSpeedLeft, initSpeedRight, 0, 0, 0);
}

/**
 * \overload
 * @param initSpeedLeft Initial speed for the left motor.
 * @param initSpeedRight Initial speed for the right motor.
 * @param reduceSpeedTime Time after which the line follow process shall proceed with the speed of reducedSpeedLeftMotor and reducedSpeedRightMotor. 0 to not use this function.
 * @param reducedSpeedLeftMotor Base speed for the PD controller after reduceSpeedTime milliseconds.
 * @param reducedSpeedRightMotor Base speed for the PD controller after reduceSpeedTime milliseconds.
 */
void LineFollow::startIt(int initSpeedLeft, int initSpeedRight, int reduceSpeedTime, byte reducedSpeedLeftMotor, byte reducedSpeedRightMotor){
  hasReachedCurve = false;

  _deltasensor = 0;
  _sensoralt = 0;
  _deltaPWM = 0;
  _timeLastLineFollowCheck = millis();
  _timeLineFollowStarted = millis();
  _reduceSpeedTime = reduceSpeedTime;
  _reducedSpeedLeftMotor = reducedSpeedLeftMotor;
  _reducedSpeedRightMotor = reducedSpeedRightMotor;
  _currentSpeedLeftMotor = initSpeedLeft;
  _currentSpeedRightMotor = initSpeedRight;
  frontLineSensorsEnabled = true;

  _move->controlMotors(forward, initSpeedLeft, forward, initSpeedRight);
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals.<br>
 * The sensor data of the sensors is captured and based on the result
 * the motor speed is adjusted.
 *
 * <b>Important: Call startIt() first!</b>
 * 
 * What's done in detail:
 * <ol>
 * <li>Read the front sensors to check, if a 90° courve is needed.</li>
 * <li>Check _reduceSpeedTime (if not 0) if it's already time to proceed with the reduced speed.</li>
 * <li>Read the line sensor values to check if one line sensor has detected white ground (line follow algorithm).</li>
 * </ol>
 */
void LineFollow::doJob(){
  if(_conf->lineFollowInterval == 0 || millis() > _timeLastLineFollowCheck + _conf->lineFollowInterval){
    _timeLastLineFollowCheck = millis();

    int sensorValues[2];
    readFrontLineSensors(sensorValues);

    //check if a 90° curve is needed (but only if the lineFollowActivateFrontSensorOffset is already reached AND the front line sensors are enabled)
    if((_conf->lineFollowActivateFrontSensorOffset == 0 || millis() > _timeLineFollowStarted + _conf->lineFollowActivateFrontSensorOffset) && frontLineSensorsEnabled){
      if(sensorValues[0] < _conf->whiteThresholdFrontSensors){
        _com->send(55, sensorValues[0]);
        _move->performFastStop();
        hasReachedCurve = true;
        return;
      }
    }

    //check if it's already time to proceed with the reduced speed
    if(_reduceSpeedTime != 0 && millis() > _timeLineFollowStarted + _reduceSpeedTime){
      int parameters[3];
      _currentSpeedLeftMotor = _reducedSpeedLeftMotor;
      _currentSpeedRightMotor = _reducedSpeedRightMotor;
      parameters[0] = _reduceSpeedTime;
      parameters[1] = _currentSpeedLeftMotor;
      parameters[2] = _currentSpeedRightMotor;
      _com->send(71, parameters, 3);
      //no more checks are needed
      _reduceSpeedTime = 0;
    }

    //Do the line following
    readLineSensors(sensorValues);
    _deltasensor = sensorValues[0]-sensorValues[1];
    _com->send(3, _deltasensor);
    _deltaPWM = (int) ( ((float)_conf->lineFollowKp) * (((float)_deltasensor)/100) + ((float)_conf->lineFollowKd)/100 * (float)(_deltasensor-_sensoralt) );
    _com->send(4, _deltaPWM);
    _sensoralt = _deltasensor;
    _move->setMotorSpeed(_currentSpeedLeftMotor + _deltaPWM, _currentSpeedLeftMotor + ((-1)*_deltaPWM)); 
  }
}

/**
 * Reads the two front line sensors and returns their values.
 * @param Pointer to an int array where the two result values will be filled in. [0] front left sensor; [1] front right sensor
 */
void LineFollow::readFrontLineSensors(int* resultArray){

  resultArray[0] = analogRead(_conf->lineFollowLeftFrontSensorPin);
  ///@todo Maybe a delay is needed between the readings.
  resultArray[1] = analogRead(_conf->lineFollowRightFrontSensorPin);

  _com->send(1, resultArray, 2);
}

/**
 * Reads the line sensors (but not the front sensors) and returns their values.
 * @param Pointer to an int array where the two result values will be filled in. [0] left sensor; [1] right sensor
 */
void LineFollow::readLineSensors(int* resultArray){

  resultArray[0] = analogRead(_conf->lineFollowLeftSensorPin);
  ///@todo Maybe a delay is needed between the readings.
  resultArray[1] = analogRead(_conf->lineFollowRightSensorPin);

  _com->send(2, resultArray, 2);
}




