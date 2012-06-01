#include "Arduino.h"
#include "MoveBackToLine.h"
#include "Move.h"
#include "Configuration.h"

/**
 * Constructur for the class with the hardware pins to use.
 */
MoveBackToLine::MoveBackToLine()
{

}

/**
 * Prepares the MoveBackToLine object to work. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void MoveBackToLine::begin(){
  LineFollow::begin();
  lineCenter.begin();

  _extMove = ExtendedMove::getInstance();

  lineHasBeenReached = false;
  isBackOnLine = false;
  errorNoCubeApproachInformation = false;
  _reachingLineFromLeft = true;
}

/**
 * Starts the "move back to line" process depending on the given cubeApproach object.
 */
void MoveBackToLine::startIt(CubeApproach* cubeApproach){
  _timeMoveBackToLineStarted = millis();
  lineHasBeenReached = false;
  isBackOnLine = false;
  errorNoCubeApproachInformation = false;
  _cubeApproach = cubeApproach;
  _reachingLineFromLeft = true;

  //first check, if the cube approach object has some valid information in it
  if(_cubeApproach->amountOfCubeDetections == 0){
    _com->send(153);
    errorNoCubeApproachInformation = true;
    return;
  }

  //second check, if the first detection already has found the cube with both sensors (CubeDetection::cubeIsCentered = true)
  if(_cubeApproach->cubeDetections[0].cubeIsCentered){
    _com->send(154);
    errorNoCubeApproachInformation = true;
    return;
  }

  //third check, if the car has left the line to the right or to the left side and make the command ready.
  MoveCommand* mc = _extMove->commandQueue;
  mc[0].duration = _conf->moveBackToLine1stTurnDuration;
  mc[0].dirLeftMotor = valueDependant;
  mc[0].dirRightMotor = valueDependant;
  if(_cubeApproach->cubeDetections[0].turnedRight){
    mc[0].speedLeftMotor = _conf->moveBackToLine1stLeftTurnLeftSpeed;
    mc[0].speedRightMotor = _conf->moveBackToLine1stLeftTurnRightSpeed;
    _reachingLineFromLeft = false;
  }
  else if(_cubeApproach->cubeDetections[0].turnedLeft){
    mc[0].speedLeftMotor = _conf->moveBackToLine1stRightTurnLeftSpeed;
    mc[0].speedRightMotor = _conf->moveBackToLine1stRightTurnLeftSpeed;
    _reachingLineFromLeft = true;
  }
  else{
    //should never happen
    _com->send(155);
    errorNoCubeApproachInformation = true;
    return;
  }

  mc[1].duration = 10000; //just long enough
  mc[1].dirLeftMotor = forward;
  mc[1].speedLeftMotor = _conf->moveBackToLineStraightSpeed;
  mc[1].dirRightMotor = forward;
  mc[1].speedRightMotor = _conf->moveBackToLineStraightSpeed;

  _extMove->startCurrentQueue(2);
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible).<br>
 * The left or right sensor (depending on the value _reachingLineFromLeft) is checked to see if the line has already been reached.
 * If so the line centering process is done.
 */
void MoveBackToLine::doJob(){
  if((_conf->moveBackToLineInterval == 0 || millis() > _timeLastMoveBackToLineCheck + _conf->moveBackToLineInterval) && !lineHasBeenReached){
    _timeLastMoveBackToLineCheck = millis();

    //check the left or right line sensor (depending on _reachingLineFromLeft)
    int sensorValues[2];
    readLineSensors(sensorValues);
    if((sensorValues[0] < _conf->whiteThresholdLineSensors && _reachingLineFromLeft) || (sensorValues[1] < _conf->whiteThresholdLineSensors && !_reachingLineFromLeft)){
      long parameters[2];
      parameters[0] = sensorValues[0];
      parameters[1] = sensorValues[1];
      _com->send(93, parameters, 2);
      _extMove->stopCurrentQueue();
      lineHasBeenReached = true;
      lineCenter.startIt(_reachingLineFromLeft);
    }
  }

  //line centering needs to be done after the line has been reached.
  if(lineHasBeenReached){
    lineCenter.doJob();
    if(lineCenter.lineCenteringIsFinished){
      isBackOnLine = true;
    }
  }
}





