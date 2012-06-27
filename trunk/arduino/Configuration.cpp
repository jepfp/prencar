#include "Configuration.h"

Configuration Configuration::_instance; ///< Static reference to the singleton object
boolean Configuration::_instanceCreated = false;

byte const Configuration::_CONFIGURATIONVERSION = 5; ///< Returns the version of the configuration (number will be increased after a set of new configuration values is added)
long const Configuration::_SERIALSPEED = 9600; ///< Defines the serial data rate.

/**
 * Get the Configuration instance
 * @return Pointer to the configuration instance
 */
Configuration* Configuration::getInstance()
{
  if ( !_instanceCreated ){
    _instance = Configuration();
    _instanceCreated = true;
  }
  return &_instance;
}

Configuration::Configuration(){
  //set default values
  //-----------------------------------
  //GENERAL CONFIG VALUES
  movePwmLeftPin = 2;
  movePwmRightPin = 3;
  moveModeFirstLeftPin = 23;
  moveModeSecondLeftPin = 22;
  moveModeFirstRightPin = 25;
  moveModeSecondRightPin = 24;

  lineFollowLeftSensorPin = A5;
  lineFollowRightSensorPin = A4;
  lineFollowLeftFrontSensorPin = A6;
  lineFollowRightFrontSensorPin = A7;

  liftCubeSwitchPin = 27;
  liftCubePwmPin = 4;

  cubeApproachLeftBottomSensor = A0;
  cubeApproachLeftTopSensor = A1;
  cubeApproachRightBottomSensor = A2;
  cubeApproachRightTopSensor = A3;

  accuPin = A8;

  //-----------------------------------
  //GENERAL CONFIG VALUES
  messageFilterLevel = 99;
  activateMessageFilter = true; 
  doJobDelay = 0;
  whiteThresholdLineSensors = 300;
  whiteThresholdFrontSensors = 300; 

  //-----------------------------------
  //LINE FOLLOW
  lineFollowInterval = 10;
  lineFollowInitialSpeedLeft = 180;
  lineFollowInitialSpeedRight = 180;
  lineFollowKp = 20;
  lineFollowKd = 150;
  lineFollowReducedSpeedLeft = 130;
  lineFollowReducedSpeedRight = 130;
  lineFollowReduceSpeedTimeFirstLine = 2500;
  lineFollowReduceSpeedTimeSecondLine = 500;
  lineFollowReduceSpeedTimeThirdLineToFinish = 500;
  lineFollowActivateFrontSensorOffset = 500;

  //-----------------------------------
  //LINE CENTER
  lineCenterInterval = 15;
  lineCenterStopDuration = 650;
  lineCenterStopSpeed = 140;
  lineCenterLineInMiddleDifference = 280;
  lineCenterStraightSpeed = 150;
  lineCenterFromLeftMotorLeft = 180;
  lineCenterFromLeftMotorRight = 40;
  lineCenterFromRightMotorLeft = 40;
  lineCenterFromRightMotorRight = 180;
  lineCenterDriveBackDuration = 500;

  //-----------------------------------
  //LIFT CUBE
  liftCubeUpPosition = 26;
  liftCubeDownPosition = 47;
  liftCubeDownUpDuration = 700;
  liftCubeStopDuration = 350;
  liftCubeStopSpeed = 130;

  //-----------------------------------
  //CURVE
  curveStopDuration = 500;
  curveStopSpeed = 130;
  curveInterval = 0;
  curveSpeedSlowMotor = 10;
  curveSpeedFastMotor = 180;
  curveDriveStraightTime = 1250;
  curveDriveStraightSpeed = 125;
  curveActivateEndSensorOffset = 1500;

  //-----------------------------------
  //CUBE APPROACH
  cubeApproachUseTopSensors = false;
  cubeApproachLineFollowReduceSpeedTime = 800;
  cubeApproachLineFollowReducedSpeedLeft = 150;
  cubeApproachLineFollowReducedSpeedRight = 150;
  cubeApproachDetectThreshold = 40;
  cubeApproachInterval = 15;
  cubeApproachTurnDuration = 750;
  cubeApproachTurnSpeedSlowMotor = 0;
  cubeApproachTurnSpeedFastMotor = 180;
  cubeApproachStraightSpeed = 160;

  //-----------------------------------
  //MOVE BACK TO LINE
  moveBackToLineInterval = 0;
  moveBackToLine1stTurnDuration = 1750;
  moveBackToLine1stLeftTurnLeftSpeed = 0;
  moveBackToLine1stLeftTurnRightSpeed = 180;
  moveBackToLine1stRightTurnLeftSpeed = 180;
  moveBackToLine1stRightTurnRightSpeed = 0;
  moveBackToLineStraightSpeed = 180;

  //-----------------------------------
  //FINISH LINE
  finishLineDriveOverDuration = 1750;

  //-----------------------------------
  //SENSOR DEBUG
  sensorDebugInterval = 1000;
  sensorDebugReadGap = 0;
}

/**
 * Returns the version of the configuration. This value will be increased in case of
 * a new set of configuration values is applied.
 * @return version
 */
byte Configuration::getConfigurationVersion(){
  return _CONFIGURATIONVERSION;
}

/**
 * Returns the serial data rate used for the communication between the board and the host computer.
 * @return speed
 */
long Configuration::getSerialSpeed(){
  return _SERIALSPEED;
}

/**
 * Sends the whole current configuration as it is stored on the microcontroller at the moment.
 * @param spaceForConfigValues Pointer to a long array with SIZEOFDYNAMICCONFIGURATION spaces to store the current configuration.
 */
void Configuration::getCurrentConfiguration(long* spaceForConfigValues){
  spaceForConfigValues[0] = _CONFIGURATIONVERSION;
  spaceForConfigValues[1] = _SERIALSPEED;
  spaceForConfigValues[2] = messageFilterLevel;
  spaceForConfigValues[3] = activateMessageFilter; 
  spaceForConfigValues[4] = doJobDelay;
  spaceForConfigValues[5] = whiteThresholdLineSensors;
  spaceForConfigValues[6] = whiteThresholdFrontSensors; 
  spaceForConfigValues[7] = lineFollowInterval;
  spaceForConfigValues[8] = lineFollowInitialSpeedLeft;
  spaceForConfigValues[9] = lineFollowInitialSpeedRight;
  spaceForConfigValues[10] = lineFollowKp;
  spaceForConfigValues[11] = lineFollowKd;
  spaceForConfigValues[12] = lineFollowReducedSpeedLeft;
  spaceForConfigValues[13] = lineFollowReducedSpeedRight;
  spaceForConfigValues[14] = lineFollowReduceSpeedTimeFirstLine;
  spaceForConfigValues[15] = lineFollowReduceSpeedTimeSecondLine;
  spaceForConfigValues[16] = lineFollowReduceSpeedTimeThirdLineToFinish;
  spaceForConfigValues[17] = lineFollowActivateFrontSensorOffset;
  spaceForConfigValues[18] = lineCenterInterval;
  spaceForConfigValues[19] = lineCenterLineInMiddleDifference;
  spaceForConfigValues[20] = lineCenterStraightSpeed;
  spaceForConfigValues[21] = lineCenterFromLeftMotorLeft;
  spaceForConfigValues[22] = lineCenterFromLeftMotorRight;
  spaceForConfigValues[23] = lineCenterFromRightMotorLeft;
  spaceForConfigValues[24] = lineCenterFromRightMotorRight;
  spaceForConfigValues[25] = lineCenterDriveBackDuration;
  spaceForConfigValues[26] = liftCubeUpPosition;
  spaceForConfigValues[27] = liftCubeDownPosition;
  spaceForConfigValues[28] = liftCubeDownUpDuration;
  spaceForConfigValues[29] = liftCubeStopDuration;
  spaceForConfigValues[30] = liftCubeStopSpeed;
  spaceForConfigValues[31] = curveStopDuration;
  spaceForConfigValues[32] = curveStopSpeed;
  spaceForConfigValues[33] = curveInterval;
  spaceForConfigValues[34] = curveSpeedSlowMotor;
  spaceForConfigValues[35] = curveSpeedFastMotor;
  spaceForConfigValues[36] = curveDriveStraightTime;
  spaceForConfigValues[37] = curveActivateEndSensorOffset;
  spaceForConfigValues[38] = cubeApproachUseTopSensors;
  spaceForConfigValues[39] = cubeApproachLineFollowReduceSpeedTime;
  spaceForConfigValues[40] = cubeApproachLineFollowReducedSpeedLeft;
  spaceForConfigValues[41] = cubeApproachLineFollowReducedSpeedRight;
  spaceForConfigValues[42] = cubeApproachDetectThreshold;
  spaceForConfigValues[43] = cubeApproachInterval;
  spaceForConfigValues[44] = cubeApproachTurnDuration;
  spaceForConfigValues[45] = cubeApproachTurnSpeedSlowMotor;
  spaceForConfigValues[46] = cubeApproachTurnSpeedFastMotor;
  spaceForConfigValues[47] = cubeApproachStraightSpeed;
  spaceForConfigValues[48] = moveBackToLineInterval;
  spaceForConfigValues[49] = moveBackToLine1stTurnDuration;
  spaceForConfigValues[50] = moveBackToLine1stLeftTurnLeftSpeed;
  spaceForConfigValues[51] = moveBackToLine1stLeftTurnRightSpeed;
  spaceForConfigValues[52] = moveBackToLine1stRightTurnLeftSpeed;
  spaceForConfigValues[53] = moveBackToLine1stRightTurnRightSpeed;
  spaceForConfigValues[54] = moveBackToLineStraightSpeed;
  spaceForConfigValues[55] = finishLineDriveOverDuration;
  spaceForConfigValues[56] = sensorDebugInterval;
  spaceForConfigValues[57] = sensorDebugReadGap;
  spaceForConfigValues[58] = curveDriveStraightSpeed;
  spaceForConfigValues[59] = lineCenterStopDuration;
  spaceForConfigValues[60] = lineCenterStopSpeed;
}

/**
 * \brief Updates all configuration values according to the given parameters.
 *
 * <b>WARNING: Be aware of the fact that the Serial library of the Arduino board per default
 * only accepts 64 characters (because of a ring buffer). We had to adjust that ring buffer in the core libs!!!!<br>
 * The ringbuffer has been adjusted now inside
 * C:/Program Files (X86)/Arduino/arduino-1.0/hardware/arduino/cores/arduino/HardwareSerial.cpp to 256!!<br>
 * Also please make sure that the constant Command::_COMMANDMAXPARAMETERAMOUNT is big enough.</b>
 * @param parameters Pointer to a long array with Configuration::SIZEOFDYNAMICCONFIGURATION parameters.
 * @return True if the configuration was updated successfully, false otherwise.
 */
boolean Configuration::updateConfiguration(int* parameters){
  //CONSTANTS will be ingored
  if(parameters[0] != _CONFIGURATIONVERSION){
    return false;
  }

  messageFilterLevel = parameters[2];
  activateMessageFilter = parameters[3];
  doJobDelay = parameters[4];
  whiteThresholdLineSensors = parameters[5];
  whiteThresholdFrontSensors = parameters[6];
  lineFollowInterval = parameters[7];
  lineFollowInitialSpeedLeft = parameters[8];
  lineFollowInitialSpeedRight = parameters[9];
  lineFollowKp = parameters[10];
  lineFollowKd = parameters[11];
  lineFollowReducedSpeedLeft = parameters[12];
  lineFollowReducedSpeedRight = parameters[13];
  lineFollowReduceSpeedTimeFirstLine = parameters[14];
  lineFollowReduceSpeedTimeSecondLine = parameters[15];
  lineFollowReduceSpeedTimeThirdLineToFinish = parameters[16];
  lineFollowActivateFrontSensorOffset = parameters[17];
  lineCenterInterval = parameters[18];
  lineCenterLineInMiddleDifference = parameters[19];
  lineCenterStraightSpeed = parameters[20];
  lineCenterFromLeftMotorLeft = parameters[21];
  lineCenterFromLeftMotorRight = parameters[22];
  lineCenterFromRightMotorLeft = parameters[23];
  lineCenterFromRightMotorRight = parameters[24];
  lineCenterDriveBackDuration = parameters[25];
  liftCubeUpPosition = parameters[26];
  liftCubeDownPosition = parameters[27];
  liftCubeDownUpDuration = parameters[28];
  liftCubeStopDuration = parameters[29];
  liftCubeStopSpeed = parameters[30];
  curveStopDuration = parameters[31];
  curveStopSpeed = parameters[32];
  curveInterval = parameters[33];
  curveSpeedSlowMotor = parameters[34];
  curveSpeedFastMotor = parameters[35];
  curveDriveStraightTime = parameters[36];
  curveActivateEndSensorOffset = parameters[37];
  cubeApproachUseTopSensors = parameters[38];
  cubeApproachLineFollowReduceSpeedTime = parameters[39];
  cubeApproachLineFollowReducedSpeedLeft = parameters[40];
  cubeApproachLineFollowReducedSpeedRight = parameters[41];
  cubeApproachDetectThreshold = parameters[42];
  cubeApproachInterval = parameters[43];
  cubeApproachTurnDuration = parameters[44];
  cubeApproachTurnSpeedSlowMotor = parameters[45];
  cubeApproachTurnSpeedFastMotor = parameters[46];
  cubeApproachStraightSpeed = parameters[47];
  moveBackToLineInterval = parameters[48];
  moveBackToLine1stTurnDuration = parameters[49];
  moveBackToLine1stLeftTurnLeftSpeed = parameters[50];
  moveBackToLine1stLeftTurnRightSpeed = parameters[51];
  moveBackToLine1stRightTurnLeftSpeed = parameters[52];
  moveBackToLine1stRightTurnRightSpeed = parameters[53];
  moveBackToLineStraightSpeed = parameters[54];
  finishLineDriveOverDuration = parameters[55];
  sensorDebugInterval = parameters[56];
  sensorDebugReadGap = parameters[57];
  curveDriveStraightSpeed = parameters[58];
  lineCenterStopDuration = parameters[59];
  lineCenterStopSpeed = parameters[60];
  return true;
}

/**
 * Gets the currently free memory.
 * Copied from: http://forum.pololu.com/viewtopic.php?f=10&t=989&view=unread#p4218
 */
extern int __bss_end;
extern void *__brkval;
int Configuration::getFreeMemory()
{
  int free_memory;

  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}



























