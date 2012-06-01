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
  Configuration();
public:
  /** \brief Defines the size of the array in which all dynamic configuration values are saved.
   * 
   * Update this, if you want to add new configuration values to the list of dynamic configuration values.<br>
   * <b>Please make sure that the constant Command::_COMMANDMAXPARAMETERAMOUNT is big enough.</b>
   */
  static byte const SIZEOFDYNAMICCONFIGURATION = 58;

  //-----------------------------------
  //PIN ASSIGNMENT
  byte movePwmLeftPin;
  byte movePwmRightPin;
  byte moveModeFirstLeftPin;
  byte moveModeSecondLeftPin;
  byte moveModeFirstRightPin;
  byte moveModeSecondRightPin;

  byte lineFollowLeftSensorPin;
  byte lineFollowRightSensorPin;
  byte lineFollowLeftFrontSensorPin;
  byte lineFollowRightFrontSensorPin;

  byte liftCubePwmPin; ///< The pin where the servo motor of the hoist is connected to.

  byte cubeApproachLeftBottomSensor; ///< The pin where the left bottom distance sensor is connected to.
  byte cubeApproachLeftTopSensor; ///< The pin where the left top distance sensor is connected to.
  byte cubeApproachRightBottomSensor; ///< The pin where the right bottom distance sensor is connected to.
  byte cubeApproachRightTopSensor; ///< The pin where the right top distance sensor is connected to.

  //-----------------------------------
  //GENERAL CONFIG VALUES
  /**
   * \brief Filter that shall be applied to all outgoing messages (over serial interface).
   * 
   * Messages with a message code below or equals to the value set in here won't be sent over the
   * serial interface.
   */
  byte messageFilterLevel;
  boolean activateMessageFilter; ///< True if _messageFilterLevel shall be taken into account.
  int doJobDelay; ///< Time in milliseconds to wait before each main loop starts. Can be used for debug purposes.
  /** \brief Threshold value from black to white.
   * 
   * If the sensor measures a value below that threshold value than the software considers that sensors physical position to be on "white" ground.
   */
  int whiteThresholdLineSensors;
  int whiteThresholdFrontSensors; ///< See lineFollowWhiteThresholdLineSensors

    //-----------------------------------
  //LINE FOLLOW
  int lineFollowInterval; ///< Interval in milliseconds in which the line follow job shall be executed.
  byte lineFollowInitialSpeedLeft; ///< The initial speed of the left motor when the parcours starts (0-255).
  byte lineFollowInitialSpeedRight; ///< The initial speed of the left motor when the parcours starts (0-255).
  /**
   * P factor for the PD controller
   */
  int lineFollowKp;
  /**
   * D factor for the PD controller
   */
  int lineFollowKd;
  /**
   * \brief The reduced speed of the left motor that can be assigned after a defined amount of time (0-255).
   * 
   * This value is used in combination with lineFollowReduceSpeedTimeFirstLine and lineFollowReduceSpeedTimeSecondLine.
   */
  byte lineFollowReducedSpeedLeft;
  /**
   * \brief See lineFollowReducedSpeedLeft
   */
  byte lineFollowReducedSpeedRight;
  /**
   * \brief The time in milliseconds (after a call to LineFollow::startIt()) on the first line after which the line follow shall continue with the
   * defined reduced speed (0 to disable this function).
   */
  int lineFollowReduceSpeedTimeFirstLine;
  /**
   * \brief The time in milliseconds (after a call to LineFollow::startIt()) on the second line after which the line follow shall continue with the
   * defined reduced speed (0 to disable this function).
   */
  int lineFollowReduceSpeedTimeSecondLine;
  /**
   * \brief The time in milliseconds (after a call to LineFollow::startIt()) on the third line and after the cube has been lifted
   * after which the line follow shall continue with the
   * defined reduced speed (0 to disable this function).
   */
  int lineFollowReduceSpeedTimeThirdLineToFinish;
  /**
   * \brief Milliseconds (after a call to LineFollow::startIt()) after which the front line sensors shall be activated. 
   *
   * This offset is helpful at the beginning of the parours in order to be able to start the parcours <b>behind</b> the start line
   * and after driving a curve if at the end of the curve one of the front line sensors still is on top of the 90°-line. If
   * set to 0 the front line sensors are always on.
   */
  int lineFollowActivateFrontSensorOffset;

  //-----------------------------------
  //LINE CENTER
  int lineCenterInterval; ///< Interval in milliseconds in which the line centering job shall be executed.
  /**
   * \brief During line centering a value below this difference will mean, that the line centering process is finished.
   */
  int lineCenterLineInMiddleDifference;
  int lineCenterStraightSpeed; ///< The speed of both motors while driving straight during the line centering.
  int lineCenterFromLeftMotorLeft; ///< The speed of the left motor during the line centering process if the car is reaching the line from the left hand side.
  int lineCenterFromLeftMotorRight; ///< The speed of the right motor during the line centering process if the car is reaching the line from the left hand side.
  int lineCenterFromRightMotorLeft; ///< The speed of the left motor during the line centering process if the car is reaching the line from the right hand side.
  int lineCenterFromRightMotorRight; ///< The speed of the right motor during the line centering process if the car is reaching the line from the right hand side.
  int lineCenterDriveBackDuration; ///< The duration while the car during the correction is driving backwards.

  //-----------------------------------
  //LIFT CUBE
  byte liftCubeUpPosition; ///< Pwm value to set if the hoist has to be moved up fully.
  byte liftCubeDownPosition; ///< Pwm value to set if the hoist has to be moved up fully.
  int liftCubeDownUpDuration; ///< Time in milliseconds between the liftDown and the liftUp command of the method LiftCube::liftCube()
  byte liftCubeSwitchPin; ///< Hardware pin to which the end switch is attached.
  /**
   * \brief Time for which the motors shall "rotate" backwards with the speed set in liftCubeStopSpeed.
   *
   * This stop action is performed right after the car has started lifting the cube. The goal of this action is to stop the car to prevent
   * it from driving across the parcours boarders.
   */
  int liftCubeStopDuration;
  int liftCubeStopSpeed; ///< The speed for the stop action which is performed after the cube lifting process has started. @see liftCubeStopDuration

  //-----------------------------------
  //CURVE
  /**
   * \brief Duration of the stop action.
   *
   * At the beginning of a curve the car has to stop because it's maybe still driving (cinetic energy).
   * This value defines, how long the stop shall be. "Stopping" means that the motors are turning in the opposite direction for the
   * time set in this config value.
   */
  int curveStopDuration;
  /**
   * \brief This value defines the speed that shall be set to both motors during a stop that has to stop the car which is currently in
   * motion (note: the direction will be set to backwards).
   *
   * @see curveStopDuration
   */
  int curveStopSpeed;
  int curveInterval; ///< Interval in milliseconds in which the curve job shall be executed.
  int curveSpeedSlowMotor; ///< The speed of the slower motor (in a left curve, left motor) during a 90° curve. This value can go from -255 to +255.
  int curveSpeedFastMotor; ///< The speed of the faster motor (in a left curve, right motor) during a 90° curve. This value can go from -255 to +255.
  int curveDriveStraightTime; ///< The time after which the car shall drive straight again to find the line (no more curve is done).
  /** \brief Time in milliseconds after which the end sensor (end sensor = sensor that will recognize when the curve is driven fully) will be activated.
   *
   * In the case of the LeftCurve class the end sensor will be the left line sensor. Because of the fact that at the beginning of the "drive curve left"
   * procedure the left line sensor can still be on white ground we add a offset after which the check of this end sensor shall be started.
   */
  int curveActivateEndSensorOffset;

  //-----------------------------------
  //CUBE APPROACH
  boolean cubeApproachUseTopSensors; ///< If set to true the top sensors are taken into account during the cube approach process.
  /**
   * \brief The time in milliseconds (after a call to LineFollow::startIt()) on the third line after which the line follow shall continue with the
   * defined reduced speed cubeApproachLineFollowReducedSpeedLeft and cubeApproachLineFollowReducedSpeedRight (0 to disable this function).
   */
  int cubeApproachLineFollowReduceSpeedTime;
  /**
   * \brief The reduced speed of the left motor that can be assigned after a defined amount of time (see cubeApproachLineFollowReduceSpeedTime)
   * during the cube approach line following process (speed 0-255).
   */
  byte cubeApproachLineFollowReducedSpeedLeft;
  byte cubeApproachLineFollowReducedSpeedRight; ///< See cubeApproachLineFollowReducedSpeedLeft
  /**
   * \brief Threshold for the cube detection.
   *
   * Measuring a value higher than this threshold means that the cube has been detected with this sensor.
   */
  int cubeApproachDetectThreshold;
  int cubeApproachInterval; ///< Interval in milliseconds in which the distance sensors shall be checked.
  int cubeApproachTurnDuration; ///< Duration in milliseconds of a turn when the cube has been detected.
  int cubeApproachTurnSpeedSlowMotor; ///< Speed of slow motor during a turn when the cube has been detected. This value can be nagative.
  int cubeApproachTurnSpeedFastMotor; ///< Speed of fast motor during a turn when the cube has been detected. This value can be nagative.
  int cubeApproachStraightSpeed; ///< Speed during cube approach while the car is driving straight.

  //-----------------------------------
  //MOVE BACK TO LINE
  int moveBackToLineInterval; ///< Interval in which during driving back to the line the line sensors shall be checked.
  int moveBackToLine1stTurnDuration; ///< Duration of the 1st turn to move / rotate the car in direction where the line is.
  int moveBackToLine1stLeftTurnLeftSpeed; ///< Speed of the left motor during the 1st turn (can be a negative) in case of a left turn. @see moveBackToLine1stTurnDuration
  int moveBackToLine1stLeftTurnRightSpeed; ///< Speed of the right motor during the 1st turn (can be a negative) in case of a left turn. @see moveBackToLine1stTurnDuration
  int moveBackToLine1stRightTurnLeftSpeed; ///< Speed of the left motor during the 1st turn (can be a negative) in case of a left turn. @see moveBackToLine1stTurnDuration
  int moveBackToLine1stRightTurnRightSpeed; ///< Speed of the right motor during the 1st turn (can be a negative) in case of a left turn. @see moveBackToLine1stTurnDuration
  int moveBackToLineStraightSpeed; ///< Speed of the car when it drives straight back to the line.

  //-----------------------------------
  //FINISH LINE
  /**
   * \brief Time in milliseconds of how long the car shall drive straight to cross the finish line.
   *
   * The car is driving with lineFollowInitialSpeedLeft and lineFollowInitialSpeedRight.
   */
  int finishLineDriveOverDuration;

  //-----------------------------------
  //SENSOR DEBUG
  int sensorDebugInterval; ///< Interval in milliseconds in which the debug sensors
  int sensorDebugReadGap; ///< Time in milliseconds between reading every sensor.

  static Configuration* getInstance();
  byte getConfigurationVersion();
  long getSerialSpeed();
  void getCurrentConfiguration(long* spaceForConfigValues);
  boolean updateConfiguration(int* parameters);
  int getFreeMemory();
};

#endif


