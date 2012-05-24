#include "Arduino.h"
#include "CubeApproach.h"
#include "LineFollow.h"
#include "Configuration.h"

/**
 * Constructur for the class.
 */
CubeApproach::CubeApproach()
{
  amountOfCubeDetections = 0;
}

/**
 * Prepares the CubeApproach object to work together with other classes. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void CubeApproach::begin(){
  _com = Communication::getInstance();
  _move = Move::getInstance();
  _conf = Configuration::getInstance();
  _extMove = ExtendedMove::getInstance();

  pinMode(_conf->cubeApproachLeftBottomSensor, INPUT);
  pinMode(_conf->cubeApproachLeftTopSensor, INPUT);
  pinMode(_conf->cubeApproachRightBottomSensor, INPUT);
  pinMode(_conf->cubeApproachRightTopSensor, INPUT);

  //make sure, the pull up resistors are off
  digitalWrite(_conf->cubeApproachLeftBottomSensor, LOW);
  digitalWrite(_conf->cubeApproachLeftTopSensor, LOW);
  digitalWrite(_conf->cubeApproachRightBottomSensor, LOW);
  digitalWrite(_conf->cubeApproachRightTopSensor, LOW);

  _lineFollow.begin();
}

/**
 * Starts the cube detection process.
 */
void CubeApproach::startIt(){
  _timeLastCubeApproachCheck = millis();
  amountOfCubeDetections = 0;
  _lineFollow.startIt(_conf->lineFollowInitialSpeedLeft, _conf->lineFollowInitialSpeedRight,
  _conf->lineFollowReduceSpeedTimeThirdLine, _conf->lineFollowReducedSpeedLeft, _conf->lineFollowReducedSpeedRight);
  _lineFollow.frontLineSensorsEnabled = false;
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals.<br>
 * The sensor data of the distance sensors is captured and based on the result
 * the car is controlled to either follow the line or drive to the cube.
 *
 * <b>Important: Call startIt() first!</b>
 * 
 * What's done in detail:
 * <ol>
 * <li>@todo Put some details in here.</li>
 * </ol>
 * @param followLine If true, the car follows the line and tries to detect the cube. If false, the car drives straight forward and tries to detect.
 */
void CubeApproach::doJob(boolean followLine){
  if(followLine){
    //follow the line
    _lineFollow.doJob();
  }
  else{
    //control the car directly without following a line.
    //@todo just drive straight
  }

  if(_conf->cubeApproachInterval == 0 || millis() > _timeLastCubeApproachCheck + _conf->cubeApproachInterval){
    _timeLastCubeApproachCheck = millis();

    //only try to detect cube if no turn is in progress
    if(!_extMove->isExecutionInProcess()){
      CubeDetection* cd = tryToDetectCube();
      if(cd != 0){
        amountOfCubeDetections++;
        if(cd->turnedRight){
          _com->send(78);
          turn(2);
        }
        else if(cd->turnedLeft){
          _com->send(79);
          turn(1);
        }
      }
    }
    else{
      _com->send(83);
    }
  }
}

/**
 * Reads the two bottom distance sensors and returns their values.
 * @param Pointer to an int array where the two result values will be filled in. [0] bottom left sensor; [1] bottom right sensor
 */
void CubeApproach::readBottomSensors(int* resultArray){

  resultArray[0] = analogRead(_conf->cubeApproachLeftBottomSensor);
  ///@todo Maybe a delay is needed between the readings.
  resultArray[1] = analogRead(_conf->cubeApproachRightBottomSensor);

  _com->send(18, resultArray, 2);
}

/**
 * Reads the two top distance sensors and returns their values.
 * @param Pointer to an int array where the two result values will be filled in. [0] top left sensor; [1] top right sensor
 */
void CubeApproach::readTopSensors(int* resultArray){

  resultArray[0] = analogRead(_conf->cubeApproachLeftTopSensor);
  ///@todo Maybe a delay is needed between the readings.
  resultArray[1] = analogRead(_conf->cubeApproachRightTopSensor);

  _com->send(19, resultArray, 2);
}

/**
 * Tries to detect the cube and returns 0 if no detection was possible or a reference to the latest object inside cubeDetections if the cube could be detected.
 * @return 0 if the cube was not detected or a reference to the lastest object inside cubeDetections if the cube could be detected.
 */
CubeDetection* CubeApproach::tryToDetectCube(){
  //get the next free spot.
  CubeDetection* cd = &cubeDetections[amountOfCubeDetections];
  cd->turnedRight = false;
  cd->turnedLeft = false;
  cd->cubeIsCentered = false;

  boolean left = false;
  boolean right = false;
  boolean topSensorsRead = false;

  int sensorValues[2];
  readBottomSensors(sensorValues);
  cd->leftBottom = sensorValues[0];
  cd->rightBottom = sensorValues[1];

  int sensorTopValues[2];
  sensorTopValues[0] = 0;
  sensorTopValues[1] = 0;

  if(cd->leftBottom > _conf->cubeApproachDetectThreshold){
    //verify the result
    readTopSensors(sensorTopValues);
    cd->leftTop = sensorTopValues[0];
    cd->rightTop = sensorTopValues[1];
    topSensorsRead = true;
    if(cd->leftTop < _conf->cubeApproachDetectThreshold){
      //verification successful.
      left = true;
    }
    else{
      //verification not passed --> revert result from left bottom sensor.
      left = false;
      _com->send(74, sensorTopValues, 2);
    }
  }

  if(cd->rightBottom > _conf->cubeApproachDetectThreshold){
    //verify the result
    if(!topSensorsRead){
      readTopSensors(sensorTopValues);
      cd->leftTop = sensorTopValues[0];
      cd->rightTop = sensorTopValues[1];
    }
    if(cd->rightTop < _conf->cubeApproachDetectThreshold){
      //verification successful.
      right = true;
    }
    else{
      //verification not passed --> revert result from right bottom sensor.
      right = false;
      _com->send(75, sensorTopValues, 2);
    }
  }

  //nothing found --> return 0
  if(!left && !right){
    _com->send(72, sensorValues, 2);
    return 0;
  }

  if(left && right){
    cd->cubeIsCentered = true;
    _com->send(73, sensorValues, 2);
  }
  else if(left){
    cd->turnedRight = true;
    _com->send(76, sensorValues, 2);
  }
  else if(right){
    cd->turnedLeft = true;
    _com->send(77, sensorValues, 2);
  }
  return cd;
}

/**
 * Creates an extended move command and executes it.
 * @param direction 1=turn left, 2=turn right
 */
void CubeApproach::turn(int direction){
  MoveCommand* mc = _extMove->commandQueue;
  mc[0].duration = _conf->cubeApproachTurnDuration;

  int speedSlow = _conf->cubeApproachTurnSpeedSlowMotor;
  int speedFast = _conf->cubeApproachTurnSpeedFastMotor;
  TMotorDirection dirSlow;
  TMotorDirection dirFast;

  if(speedSlow >= 0){
    dirSlow = forward;
  }
  else
  {
    dirSlow = backwards;
    speedSlow = speedSlow * (-1);
  }

  if(speedFast >= 0){
    dirFast = forward;
  }
  else
  {
    dirFast = backwards;
    speedFast = speedFast * (-1);
  }

  if(direction == 1){
    mc[0].dirLeftMotor = dirSlow;
    mc[0].speedLeftMotor = speedSlow;
    mc[0].dirRightMotor = dirFast;
    mc[0].speedRightMotor = speedFast;
  }
  else
  {
    mc[0].dirLeftMotor = dirFast;
    mc[0].speedLeftMotor = speedFast;
    mc[0].dirRightMotor = dirSlow;
    mc[0].speedRightMotor = speedSlow;
  }

  _extMove->startCurrentQueue(1);
}





















