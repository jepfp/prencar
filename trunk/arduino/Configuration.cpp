#include "Configuration.h"

Configuration Configuration::_instance; ///< Static reference to the singleton object
boolean Configuration::_instanceCreated = false;

byte const Configuration::_CONFIGURATIONVERSION = 1; ///< Returns the version of the configuration (number will be increased after a set of new configuration values is added)
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
  activateMessageFilter = true; //don't change this value because the master software expects it to be true at startup (state is not read at the beginning)!!
  _messageFilterLevel = 99;
  doJobDelay = 0;
  lineFollowWhiteThreshold = 300;
  lineFollowInterval = 15;
  lineFollowInitialSpeedLeft = 150; //150 out of 255
  lineFollowInitialSpeedRight = 150; //150 out of 255
  _movePwmLeftPin = 2;
  _movePwmRightPin = 3;
  _moveModeFirstLeftPin = 23;
  _moveModeSecondLeftPin = 22;
  _moveModeFirstRightPin = 25;
  _moveModeSecondRightPin = 24;
  lineFollowLeftSensorPin = A5;
  lineFollowRightSensorPin = A4;
  lineFollowLeftFrontSensorPin = A6;
  lineFollowRightFrontSensorPin = A7;
  lineFollowKp = 20;
  lineFollowKd = 150;
  liftCubePwmPin = 4;
  liftCubeUpPosition = 28;
  liftCubeDownPosition = 47;
  liftCubeSwitchPin = 27;
  liftCubeDownUpDuration = 700;

  curveSpeedSlowMotor = 60;
  curveSpeedFastMotor = 120;
  curveInterval = 0;
  curveActivateEndSensorOffset = 500;

  cubeApproachLeftBottomSensor = A0;
  cubeApproachLeftTopSensor = A1;
  cubeApproachRightBottomSensor = A2;
  cubeApproachRightTopSensor = A3;

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
 * Returns a byte value which represents the filter value. All message codes that are higher than that level
 * will be send through out the serial interface.<br>
 * Default level: 0
 * @see Communication::send()
 */
byte Configuration::getMessageFilterLevel(){
  return _messageFilterLevel;
}

/**
 * All message codes sent over the Communication::send() method that are higher than that level
 * will be send through out the serial interface.<br>
 * @see Communication::send()
 */
void Configuration::setMessageFilterLevel(byte value){
  _messageFilterLevel = value;
}

/**
 * @see _movePwmLeftPin
 */
byte Configuration::getMovePwmLeftPin(){
  return _movePwmLeftPin;
}

/**
 * @see _movePwmLeftPin
 */
void Configuration::setMovePwmLeftPin(byte value){
  _movePwmLeftPin = value;
}

/**
 * @see _movePwmRightPin
 */
byte Configuration::getMovePwmRightPin(){
  return _movePwmRightPin;
}

/**
 * @see _movePwmRightPin
 */
void Configuration::setMovePwmRightPin(byte value){
  _movePwmRightPin = value;
}

/**
 * @see _moveModeFirstLeftPin
 */
byte Configuration::getMoveModeFirstLeftPin(){
  return _moveModeFirstLeftPin;
}

/**
 * @see _moveModeFirstLeftPin
 */
void Configuration::setMoveModeFirstLeftPin(byte value){
  _moveModeFirstLeftPin = value;
}

/**
 * @see _moveModeSecondLeftPin
 */
byte Configuration::getMoveModeSecondLeftPin(){
  return _moveModeSecondLeftPin;
}

/**
 * @see _moveModeSecondLeftPin
 */
void Configuration::setMoveModeSecondLeftPin(byte value){
  _moveModeSecondLeftPin = value;
}

/**
 * @see _moveModeFirstRightPin
 */
byte Configuration::getMoveModeFirstRightPin(){
  return _moveModeFirstRightPin;
}

/**
 * @see _moveModeFirstRightPin
 */
void Configuration::setMoveModeFirstRightPin(byte value){
  _moveModeFirstRightPin = value;
}

/**
 * @see _moveModeSecondRightPin
 */
byte Configuration::getMoveModeSecondRightPin(){
  return _moveModeSecondRightPin;
}

/**
 * @see _moveModeSecondRightPin
 */
void Configuration::setMoveModeSecondRightPin(byte value){
  _moveModeSecondRightPin = value;
}

/**
 * Sends the whole current configuration as it is stored on the microcontroller at the moment.
 * @param spaceForConfigValues Pointer to a long array with SIZEOFDYNAMICCONFIGURATION spaces to store the current configuration.
 */
void Configuration::getCurrentConfiguration(long* spaceForConfigValues){
  spaceForConfigValues[0] = _CONFIGURATIONVERSION;
  spaceForConfigValues[1] = _SERIALSPEED;
  spaceForConfigValues[2] = _messageFilterLevel;
  spaceForConfigValues[3] = doJobDelay;
  spaceForConfigValues[4] = lineFollowInitialSpeedLeft;
  spaceForConfigValues[5] = lineFollowInitialSpeedRight;
  spaceForConfigValues[6] = lineFollowInterval;
  spaceForConfigValues[7] = lineFollowKp;
  spaceForConfigValues[8] = lineFollowKd;
  spaceForConfigValues[9] = lineFollowWhiteThreshold;
  spaceForConfigValues[10] = curveSpeedSlowMotor;
  spaceForConfigValues[11] = curveSpeedFastMotor;
  spaceForConfigValues[12] = curveInterval;
  spaceForConfigValues[13] = curveActivateEndSensorOffset;
  spaceForConfigValues[14] = sensorDebugInterval;
  spaceForConfigValues[15] = sensorDebugReadGap;
  spaceForConfigValues[16] = liftCubeDownUpDuration;
}

/**
 * Updates all configuration values according to the given parameters.
 *
 * <b>WARNING: Be aware of the fact that the Serial library of the Arduino board at the moment
 * only accepts 64 characters for the timebeing (because of a ring buffer). If more configuration
 * values are added, we have to adjust that ring buffer in the core libs!!!!</b><br>
 * <b>The ringbuffer has been adjusted now inside C:/Program Files (X86)/Arduino/arduino-1.0/hardware/arduino/cores/arduino/HardwareSerial.cpp to 128!!</b>
 * @param parameters Pointer to a long array with SIZEOFDYNAMICCONFIGURATION parameters.
 */
void Configuration::updateConfiguration(int* parameters){
  //CONSTANTS will be ingored
  ///@todo Add a check for the configuration version!
  _messageFilterLevel = parameters[2];
  doJobDelay = parameters[3];
  lineFollowInitialSpeedLeft = parameters[4];
  lineFollowInitialSpeedRight = parameters[5];
  lineFollowInterval = parameters[6];
  lineFollowKp = parameters[7];
  lineFollowKd = parameters[8];
  lineFollowWhiteThreshold = parameters[9];
  curveSpeedSlowMotor = parameters[10];
  curveSpeedFastMotor = parameters[11];
  curveInterval = parameters[12];
  curveActivateEndSensorOffset = parameters[13];
  sensorDebugInterval = parameters[14];
  sensorDebugReadGap = parameters[15];
  liftCubeDownUpDuration = parameters[16];
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











