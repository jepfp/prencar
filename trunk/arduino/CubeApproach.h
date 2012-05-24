#ifndef CubeApproach_h
#define CubeApproach_h

#include "Arduino.h"
#include "LineFollow.h"
#include "Communication.h"
#include "ExtendedMove.h"

struct CubeDetection{
  int leftBottom;
  int leftTop;
  int rightBottom;
  int rightTop;
  boolean turnedRight; ///< Set to true if the car has turned right after this detection.
  boolean turnedLeft; ///< Set to true if the car has turned left after this detection.
  boolean cubeIsCentered; ///< Set to true if the cube has been detected with both bottom sensors at this detection.
};

class CubeApproach
{
private:
  static int const _MAXRECORDABLECUBEDETECTIONS = 20; ///< Defines, how many cube detections can be recorded.

  unsigned long _timeLastCubeApproachCheck; ///< Holds the timestamp of the time when the last cube approach check was done. @see doJob()

  Communication* _com; ///< The reference to the serial communication class.
  Move* _move; ///< The reference to the move class to control the motors.
  Configuration* _conf; ///< The reference to the configuration class.
  ExtendedMove* _extMove; ///< The reference to the extended move class.
  
  LineFollow _lineFollow; ///< LineFollow instance for the cube approach.

  void readBottomSensors(int* resultArray);
  void readTopSensors(int* resultArray);
  CubeDetection* tryToDetectCube();
  void turn(int direction);
public:
  CubeDetection cubeDetections[_MAXRECORDABLECUBEDETECTIONS];
  int amountOfCubeDetections; ///< Counter that is increased each time the cube is detected by either the left or the right sensor.
  
  CubeApproach();
  void begin();
  void doJob(boolean followLine);
  void startIt();
};
#endif











