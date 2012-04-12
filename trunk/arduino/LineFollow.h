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
  void startIt();
private:
  int _calibrationSensorWhiteMin; ///< Holds the minimum value that was measured during the calibration for "white".
  int _calibrationSensorWhiteMax; ///< Holds the maximum value that was measured during the calibration for "white".
  int _calibrationSensorBlackMin; ///< Holds the minimum value that was measured during the calibration for "black".
  int _calibrationSensorBlackMax; ///< Holds the maximum value that was measured during the calibration for "black".

  unsigned long _timeLastLineFollowCheck; ///< Holds the timestamp of the time when the last line follow check was done. @see doJob()
  int _deltasensor; ///< @todo write comment
  int _sensoralt; ///< @todo write comment
  int _deltaPWM; ///< @todo write comment

  Communication* _com; ///< The reference to the serial communication class.
  Move* _move; ///< The reference to the move class to control the motors.
  Configuration* _conf; ///< The reference to the configuration class.

  /**
   * This variable holds the timestamp when a correction was started. At this time the sensors have detected
   * white ground which means that the car is about to leave the line.
   * @see doJob()
   */
  unsigned long _timeSinceCorrectionStarted;

  void readFrontLineSensors(int* resultArray);
  void readLineSensors(int* resultArray);
};
#endif









