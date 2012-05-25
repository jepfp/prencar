#include "Arduino.h"
#include "LineCenter.h"
#include "Move.h"
#include "Configuration.h"

/**
 * Constructur for the class with the hardware pins to use.
 */
LineCenter::LineCenter()
{

}

/**
 * Prepares the LineCenter object to work. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void LineCenter::begin(){
  LineFollow::begin();

  _extMove = ExtendedMove::getInstance();

  lineCenteringIsFinished = false;
  _centeringFromLeft = true;
}

/**
 * Starts the car doing the line centering based on the value given in comingFrom.
 * comingFromLeft = true means that the car is reaching the line from the left hand side so the car has to constantly do a left curve until it is centered.
 * comingFromLeft = false means that the car is reaching the line from the right hand side so the car has to constantly do a right curve until it is centered.
 * @param comingFromLeft Set to true if the car drives on top of the line coming from the left side.
 */
void LineCenter::startIt(boolean comingFromLeft){
  _centeringFromLeft = comingFromLeft;
  lineCenteringIsFinished = false;
  _timeLineCenterStarted = millis();
  _timeLastLineCenterCheck = millis();

  if(comingFromLeft){
    _com->send(84);
  }
  else{
    _com->send(85);
  }
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible).<br>
 * The car centers itself on the line. For details see the class comment.
 */
void LineCenter::doJob(){
  if(_conf->lineCenterInterval == 0 || millis() > _timeLastLineCenterCheck + _conf->lineCenterInterval){
    _timeLastLineCenterCheck = millis();

    long parameters[3];

    //read the line sensors
    int sensorValues[2];
    readLineSensors(sensorValues);
    parameters[0] = sensorValues[0];
    parameters[1] = sensorValues[1];
    parameters[2] = _conf->whiteThresholdLineSensors;

    //first check, if the measured values are in the range defined in _conf->lineCenterLineInMiddleDifference
    int sensorGap = sensorValues[0] - sensorValues[1];
    if(sensorGap < 0) sensorGap *= -1;
    if(sensorGap < _conf->lineCenterLineInMiddleDifference && sensorValues[0] > _conf->whiteThresholdLineSensors){
      _extMove->stopCurrentQueue();
      lineCenteringIsFinished = true;
      _com->send(86);
    }
    
    //if a correction is in progress, let it finish first
    if(_extMove->isExecutionInProcess()){
      _com->send(89);
      return;
    }

    //now check, if the car has to drive straight because the left or right sensor (depenging on  is on "centeringFromLeft")
    //measures black ground.
    //if not drive a curve according to the value set in "centeringFromLeft"
    if(_centeringFromLeft){
      if(sensorValues[0] > _conf->whiteThresholdLineSensors){
        _com->send(87, parameters, 3);
        _move->controlMotors(forward, _conf->lineCenterStraightSpeed, forward, _conf->lineCenterStraightSpeed);
      }
      else{
        _com->send(88, parameters, 3);
        doCorrection();
      }
    }
    else{
      if(sensorValues[0] > _conf->whiteThresholdLineSensors){
        _com->send(87, parameters, 3);
        _move->controlMotors(forward, _conf->lineCenterStraightSpeed, forward, _conf->lineCenterStraightSpeed);
      }
      else{
        _com->send(88, parameters, 3);
        doCorrection();
      }
    }
  }
}

void LineCenter::doCorrection(){
  //create an extMove command and start it's execution
  MoveCommand* mc = _extMove->commandQueue;
  mc[0].duration = _conf->lineCenterDriveBackDuration;
  mc[0].dirLeftMotor = backwards;
  mc[0].dirRightMotor = backwards;

  if(_centeringFromLeft){
    mc[0].speedLeftMotor = _conf->lineCenterFromLeftMotorLeft;
    mc[0].speedRightMotor = _conf->lineCenterFromLeftMotorRight;
  }else{
    mc[0].speedLeftMotor = _conf->lineCenterFromRightMotorLeft;
    mc[0].speedRightMotor = _conf->lineCenterFromRightMotorRight;
  }

  _extMove->startCurrentQueue(1);
}




































