#include "Arduino.h"
#include "CurveLeft.h"
#include "Move.h"
#include "Configuration.h"

/**
 * Constructur for the class with the hardware pins to use.
 */
CurveLeft::CurveLeft()
{

}

/**
 * Prepares the CurveLeft object to work. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void CurveLeft::begin(){
  LineFollow::begin();

  drivingCurveIsFinished = false;
}

/**
 * Starts the car doing the left curve.
 */
void CurveLeft::startIt(){
  int speedLeft;
  int speedRight;
  TMotorDirection leftMotorDir = forward;
  TMotorDirection rightMotorDir = forward;
  
  _com->send(67);
  
  drivingCurveIsFinished = false;

  _timeCurveLeftStarted = millis();
  
  if(_conf->curveSpeedSlowMotor < 0){
    speedLeft = _conf->curveSpeedSlowMotor * -1;
    leftMotorDir = backwards;
  }else{
    speedLeft = _conf->curveSpeedSlowMotor;
  }
  
  if(_conf->curveSpeedFastMotor < 0){
    speedRight = _conf->curveSpeedFastMotor * -1;
    rightMotorDir = backwards;
  }else{
    speedRight = _conf->curveSpeedFastMotor;
  }

  _move->controlMotors(leftMotorDir, speedLeft, rightMotorDir, speedRight);
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible).<br>
 * The 90° left curve is driven. For details, how this is done, see the class description of CurveLeft.
 */
void CurveLeft::doJob(){
  if(_conf->curveInterval == 0 || millis() > _timeLastCurveLeftCheck + _conf->curveInterval){
    _timeLastCurveLeftCheck = millis();

    //read the left line sensor, if it's already time to do so
    if(_conf->curveActivateEndSensorOffset == 0 || millis() > _timeCurveLeftStarted + _conf->curveActivateEndSensorOffset){
      int sensorValues[2];
      readLineSensors(sensorValues);

      if(sensorValues[0] < _conf->lineFollowWhiteThreshold){
        _com->send(66, sensorValues[0]);
        _move->performFastStop();
        drivingCurveIsFinished = true;
      }
    }
  }
}





















