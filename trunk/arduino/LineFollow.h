#ifndef LineFollow_h
#define LineFollow_h

#include "Arduino.h"
#include "Communication.h"
#include "Move.h"

class LineFollow
{
public:
  LineFollow();
  void begin();
  void doJob();
  void calibrateSensors();

  boolean hasReachedCurve; ///< Is set to true, if the left curve line is reached
  boolean frontLineSensorsEnabled; ///< Defines if the line follow class shall check the front line sensors (default's true after every new start!)

  void startIt(int initSpeedLeft, int initSpeedRight);
  void startIt(int initSpeedLeft, int initSpeedRight, int reduceSpeedTime, byte reducedSpeedLeftMotor, byte reducedSpeedRightMotor);
protected:
  int _calibrationSensorWhiteMin; ///< Holds the minimum value that was measured during the calibration for "white".
  int _calibrationSensorWhiteMax; ///< Holds the maximum value that was measured during the calibration for "white".
  int _calibrationSensorBlackMin; ///< Holds the minimum value that was measured during the calibration for "black".
  int _calibrationSensorBlackMax; ///< Holds the maximum value that was measured during the calibration for "black".

  unsigned long _timeLineFollowStarted; ///< Holds the timestamp of when the line follow process started with startIt();

  byte _reducedSpeedLeftMotor; ///< The reduced speed (0-255) of the left motor that can be assigned after _reduceSpeedTime has been reached.
  byte _reducedSpeedRightMotor; ///< The reduced speed (0-255) of the right motor that can be assigned after _reduceSpeedTime has been reached.
  /**
   * Time after which the speed of the motors shall be reduced to _reducedSpeedLeftMotor respectively _reducedSpeedRightMotor. Set 0
   * to not use this.
   */
  int _reduceSpeedTime;

  byte _currentSpeedLeftMotor; ///< The current base speed of the left motor. The regulation of the motor speed is done based on that value.
  byte _currentSpeedRightMotor; ///< The current base speed of the right motor. The regulation of the motor speed is done based on that value.

  Communication* _com; ///< The reference to the serial communication class.
  Move* _move; ///< The reference to the move class to control the motors.
  Configuration* _conf; ///< The reference to the configuration class.

  void readFrontLineSensors(int* resultArray);
  void readLineSensors(int* resultArray);

private:
  unsigned long _timeLastLineFollowCheck; ///< Holds the timestamp of the time when the last line follow check was done. @see doJob()
  int _deltasensor; ///< @todo write comment
  int _sensoralt; ///< @todo write comment
  int _deltaPWM; ///< @todo write comment
};
#endif













