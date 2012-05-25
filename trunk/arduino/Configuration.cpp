#include "Configuration.h"

Configuration Configuration::_instance; ///< Static reference to the singleton object
boolean Configuration::_instanceCreated = false;

byte const Configuration::_CONFIGURATIONVERSION = 2; ///< Returns the version of the configuration (number will be increased after a set of new configuration values is added)
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
  lineFollowActivateFrontSensorOffset = 500;

  //-----------------------------------
  //LINE CENTER
  lineCenterInterval = 15;
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

  //-----------------------------------
  //CURVE
  curveStopDuration = 500;
  curveStopSpeed = 130;
  curveInterval = 0;
  curveSpeedSlowMotor = 10;
  curveSpeedFastMotor = 180;
  curveDriveStraightTime = 1250;
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
  spaceForConfigValues[16] = lineFollowActivateFrontSensorOffset;
  spaceForConfigValues[17] = lineCenterInterval;
  spaceForConfigValues[18] = lineCenterLineInMiddleDifference;
  spaceForConfigValues[19] = lineCenterStraightSpeed;
  spaceForConfigValues[20] = lineCenterFromLeftMotorLeft;
  spaceForConfigValues[21] = lineCenterFromLeftMotorRight;
  spaceForConfigValues[22] = lineCenterFromRightMotorLeft;
  spaceForConfigValues[23] = lineCenterFromRightMotorRight;
  spaceForConfigValues[24] = lineCenterDriveBackDuration;
  spaceForConfigValues[25] = liftCubeUpPosition;
  spaceForConfigValues[26] = liftCubeDownPosition;
  spaceForConfigValues[27] = liftCubeDownUpDuration;
  spaceForConfigValues[28] = curveStopDuration;
  spaceForConfigValues[29] = curveStopSpeed;
  spaceForConfigValues[30] = curveInterval;
  spaceForConfigValues[31] = curveSpeedSlowMotor;
  spaceForConfigValues[32] = curveSpeedFastMotor;
  spaceForConfigValues[33] = curveDriveStraightTime;
  spaceForConfigValues[34] = curveActivateEndSensorOffset;
  spaceForConfigValues[35] = cubeApproachUseTopSensors;
  spaceForConfigValues[36] = cubeApproachLineFollowReduceSpeedTime;
  spaceForConfigValues[37] = cubeApproachLineFollowReducedSpeedLeft;
  spaceForConfigValues[38] = cubeApproachLineFollowReducedSpeedRight;
  spaceForConfigValues[39] = cubeApproachDetectThreshold;
  spaceForConfigValues[40] = cubeApproachInterval;
  spaceForConfigValues[41] = cubeApproachTurnDuration;
  spaceForConfigValues[42] = cubeApproachTurnSpeedSlowMotor;
  spaceForConfigValues[43] = cubeApproachTurnSpeedFastMotor;
  spaceForConfigValues[44] = cubeApproachStraightSpeed;
  spaceForConfigValues[45] = sensorDebugInterval;
  spaceForConfigValues[46] = sensorDebugReadGap;
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
  lineFollowActivateFrontSensorOffset = parameters[16];
  lineCenterInterval = parameters[17];
  lineCenterLineInMiddleDifference = parameters[18];
  lineCenterStraightSpeed = parameters[19];
  lineCenterFromLeftMotorLeft = parameters[20];
  lineCenterFromLeftMotorRight = parameters[21];
  lineCenterFromRightMotorLeft = parameters[22];
  lineCenterFromRightMotorRight = parameters[23];
  lineCenterDriveBackDuration = parameters[24];
  liftCubeUpPosition = parameters[25];
  liftCubeDownPosition = parameters[26];
  liftCubeDownUpDuration = parameters[27];
  curveStopDuration = parameters[28];
  curveStopSpeed = parameters[29];
  curveInterval = parameters[30];
  curveSpeedSlowMotor = parameters[31];
  curveSpeedFastMotor = parameters[32];
  curveDriveStraightTime = parameters[33];
  curveActivateEndSensorOffset = parameters[34];
  cubeApproachUseTopSensors = parameters[35];
  cubeApproachLineFollowReduceSpeedTime = parameters[36];
  cubeApproachLineFollowReducedSpeedLeft = parameters[37];
  cubeApproachLineFollowReducedSpeedRight = parameters[38];
  cubeApproachDetectThreshold = parameters[39];
  cubeApproachInterval = parameters[40];
  cubeApproachTurnDuration = parameters[41];
  cubeApproachTurnSpeedSlowMotor = parameters[42];
  cubeApproachTurnSpeedFastMotor = parameters[43];
  cubeApproachStraightSpeed = parameters[44];
  sensorDebugInterval = parameters[45];
  sensorDebugReadGap = parameters[46];
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




















