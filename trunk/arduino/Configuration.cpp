#include "Configuration.h"

Configuration Configuration::_instance; ///< Static reference to the singleton object
boolean Configuration::_instanceCreated = false;

byte const Configuration::_CONFIGURATIONVERSION = 1; ///< Returns the version of the configuration (number will be increased after a set of new configuration values is added)
int const Configuration::_SERIALSPEED = 9600; ///< Defines the serial data rate.

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
  _messageFilterLevel = 0;
  lineFollowWhiteThreshold = 300;
  /*lineCorrectionDuration = 200;
   lineFollowInitialSpeed = 150; //150 out of 255
   lineFollowCorrectionPlus = 50;
   lineFollowCorrectionMinus = 50;*/
  lineCorrectionDuration = 0;
  lineFollowInitialSpeedLeft = 120; //150 out of 255
  lineFollowInitialSpeedRight = 120; //150 out of 255
  lineFollowCorrectionPlus = 30;
  lineFollowCorrectionMinus = 50;
  _movePwmLeftPin = 2;
  _movePwmRightPin = 3;
  _moveModeFirstLeftPin = 22;
  _moveModeSecondLeftPin = 23;
  _moveModeFirstRightPin = 24;
  _moveModeSecondRightPin = 25;
  lineFollowLeftSensorPin = A5;
  lineFollowRightSensorPin = A4;
  lineFollowLeftFrontSensorPin = A6;
  lineFollowRightFrontSensorPin = A7;
  lineFollowKp = 5;
  lineFollowKd = 25;
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
int Configuration::getSerialSpeed(){
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
 * @param spaceForConfigValues Pointer to a long array with 9 spaces to store the current configuration.
 */
void Configuration::getCurrentConfiguration(long* spaceForConfigValues){
  spaceForConfigValues[0] = ((long)_CONFIGURATIONVERSION);
  spaceForConfigValues[1] = _SERIALSPEED;
  spaceForConfigValues[2] = _messageFilterLevel;
  spaceForConfigValues[3] = lineFollowInitialSpeedLeft;
  spaceForConfigValues[4] = lineFollowInitialSpeedRight;
  spaceForConfigValues[5] = lineCorrectionDuration;
  spaceForConfigValues[6] = lineFollowKp;
  spaceForConfigValues[7] = lineFollowKd;
  spaceForConfigValues[8] = lineFollowWhiteThreshold;
}

/**
 * Updates all configuration values according to the given parameters.
 * @param parameters Pointer to a long array with 18 parameters.
 */
void Configuration::updateConfiguration(long* parameters){
}







