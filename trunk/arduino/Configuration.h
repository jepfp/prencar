#ifndef Configuration_h
#define Configuration_h

#include "Arduino.h"

class Configuration
{
private:
  static Configuration _instance;
  static boolean _instanceCreated;
  static byte const _CONFIGURATIONVERSION;
  static long const _SERIALSPEED;
  byte _messageFilterLevel;
  byte _movePwmLeftPin;
  byte _movePwmRightPin;
  byte _moveModeFirstLeftPin;
  byte _moveModeSecondLeftPin;
  byte _moveModeFirstRightPin;
  byte _moveModeSecondRightPin;
  Configuration();
public:
  /** \brief Defines the size of the array in which all dynamic configuration values are saved.
   * 
   * Update this, if you want to add new configuration values to the list of dynamic configuration values.
   */
  static byte const SIZEOFDYNAMICCONFIGURATION = 16;
  
  static Configuration* getInstance();
  byte getConfigurationVersion();
  long getSerialSpeed();
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

  int doJobDelay; ///< Time in milliseconds to wait before the main loop starts. Can be used for debug purposes.

  byte lineFollowLeftSensorPin;
  byte lineFollowRightSensorPin;
  byte lineFollowLeftFrontSensorPin;
  byte lineFollowRightFrontSensorPin;
  int lineFollowInterval; ///< Interval in milliseconds in which the line follow job shall be executed.
  byte lineFollowInitialSpeedLeft; ///< The initial speed of the left motor when the parcours starts (0-255).
  byte lineFollowInitialSpeedRight; ///< The initial speed of the left motor when the parcours starts (0-255).

  /** \brief Threshold value from black to white.
   * 
   * If the sensor measures a value below that threshold value than the software considers that sensors physical position to be on "white" ground.
   * This treshold value is calculated during the calibration of the sensors.<br>
   * The default value 156 is based on first measurements and set as long as no calibration is done.
   * @see LineFollow::calibrateSensors()
   */
  int lineFollowWhiteThreshold;

  /**
   * @todo write comment
   */
  int lineFollowKp;
  /**
   * @todo write comment
   */
  int lineFollowKd;

  byte liftCubePwmPin; ///< The pin where the servo motor of the hoist is connected to.
  byte liftCubeUpPosition; ///< Pwm value to set if the hoist has to be moved up fully.
  byte liftCubeDownPosition; ///< Pwm value to set if the hoist has to be moved up fully.
  int liftCubeDownUpDuration; ///< Time in milliseconds between the liftDown and the liftUp command of the method LiftCube::liftCube()

  byte curveSpeedSlowMotor; ///< The speed of the slower motor (in a left curve, left motor) during a 90° curve.
  byte curveSpeedFastMotor; ///< The speed of the faster motor (in a left curve, right motor) during a 90° curve.
  int curveInterval; ///< Interval in milliseconds in which the curve job shall be executed.
  /** \brief Time in milliseconds after which the end sensor (end sensor = sensor that will recognize when the curve is driven fully) will be activated.
   * In the case of the LeftCurve class the end sensor will be the left line sensor. Because of the fact that at the beginning of the "drive curve left"
   * procedure the left line sensor can still be on white ground we add a offset after which the check of this end sensor shall be started.
   */
  int curveActivateEndSensorOffset;

  byte cubeApproachLeftBottomSensor; ///< The pin where the left bottom distance sensor is connected to.
  byte cubeApproachLeftTopSensor; ///< The pin where the left top distance sensor is connected to.
  byte cubeApproachRightBottomSensor; ///< The pin where the right bottom distance sensor is connected to.
  byte cubeApproachRightTopSensor; ///< The pin where the right top distance sensor is connected to.

  int sensorDebugInterval; ///< Interval in milliseconds in which the debug sensors
  int sensorDebugReadGap; ///< Time in milliseconds between reading every sensor.

  void getCurrentConfiguration(long* spaceForConfigValues);
  void updateConfiguration(int* parameters);
  int getFreeMemory();

};

#endif












