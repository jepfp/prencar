#ifndef Configuration_h
#define Configuration_h

#include "Arduino.h"

class Configuration
{
private:
  static Configuration _instance;
  static boolean _instanceCreated;
  static byte const _CONFIGURATIONVERSION;
  static int const _SERIALSPEED;
  byte _messageFilterLevel;
  byte _movePwmLeftPin;
  byte _movePwmRightPin;
  byte _moveModeFirstLeftPin;
  byte _moveModeSecondLeftPin;
  byte _moveModeFirstRightPin;
  byte _moveModeSecondRightPin;
  Configuration();
public:
  static Configuration* getInstance();
  byte getConfigurationVersion();
  int getSerialSpeed();
  byte getMessageFilterLevel();
  void setMessageFilterLevel(byte value);
  byte getMovePwmLeftPin();
  void setMovePwmLeftPin(byte value);
  byte getMovePwmRightPin();
  void setMovePwmRightPin(byte value);
  byte getMoveModeFirstLeftPin();
  void setMoveModeFirstLeftPin(byte value);
  byte getMoveModeSecondLeftPin();
  void setMoveModeSecondLeftPin(byte value);
  byte getMoveModeFirstRightPin();
  void setMoveModeFirstRightPin(byte value);
  byte getMoveModeSecondRightPin();
  void setMoveModeSecondRightPin(byte value);
  
  byte lineFollowLeftSensorPin;
  byte lineFollowRightSensorPin;
  byte lineFollowLeftFrontSensorPin;
  byte lineFollowRightFrontSensorPin;
  unsigned long lineCorrectionDuration; ///< Time in milliseconds which defines, for how long the car shall do a correction after white ground has been detected.
  byte lineFollowInitialSpeed; ///< The initial speed of both motor when the parcours starts (0-255).
  /** \brief Defines the value that shall be added to the current speed in case of a correction.
   * 
   * In case of a correction one motor needs to rotate faster than the other one. Depending if a correction to the left or to the right 
   * side is needed, this positive value will be added to the left or right motor.<br>
   * If the car drives away from the line left hand side the right line sensor will measure white ground. The starting correction will now add
   * the value defined in lineFollowCorrectionPlus to the current speed of the left motor. If the car drives away from the line to the right hand side
   * the value will be added to the right motor.
   */
  byte lineFollowCorrectionPlus;
  /** \brief Defines the value that shall be subtraced from the current speed in case of a correction.
   * 
   * In case of a correction one motor needs to rotate slower than the other one. Depending if a correction to the left or to the right 
   * side is needed, this positive value will be added to the left or right motor.<br>
   * If the car drives away from the line left hand side the right line sensor will measure white ground. The starting correction will now subtract
   * the value defined in lineFollowCorrectionMinus from the current speed of the right motor. If the car drives away from the line to the right hand side
   * the value will be added to the left motor.
   * <b>Important: The positive value defined here will be subtracted.</b>
   */
  byte lineFollowCorrectionMinus;
  
  /** \brief Threshold value from black to white.
   * 
   * If the sensor measures a value below that threshold value than the software considers that sensors physical position to be on "white" ground.
   * This treshold value is calculated during the calibration of the sensors.<br>
   * The default value 156 is based on first measurements and set as long as no calibration is done.
   * @see LineFollow::calibrateSensors()
   */
  int lineFollowWhiteThreshold;
  
  void getCurrentConfiguration(long* spaceForConfigValues);
  
  
};

#endif




