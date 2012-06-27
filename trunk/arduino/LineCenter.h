#ifndef LineCenter_h
#define LineCenter_h

#include "Arduino.h"
#include "LineFollow.h"
#include "ExtendedMove.h"

/**
 * \brief The LineCenter class centers the car on the line.
 * 
 * Centering the car according to the line means, that the car at the end looks in the same direction as the line goes.
 * Depending on the parameter comingFromLeft while calling startIt() the car assumes that he is reaching the line from the
 * left or right side.
 * Assuming that the car reaches the line from the left side, the left sensor will at one point measure white ground. Then a
 * corrction is started which means that the car is driving back and turning. After that it starts moving straight forward again.
 * If the car reaches the line again, a new correction will be started again. If at any time both sensors measure black ground (which
 * means the line is between both sensors) the line center process has finished.
 */
class LineCenter : 
public LineFollow
{
public:
  LineCenter();
  virtual void begin();
  virtual void doJob();
  virtual void startIt(boolean comingFromLeft);

  /** \brief This variable is set to true as soon as the car is centered completely.
   * 
   * With other words this means that the difference between both sensors is below
   * the value defined in Configuration::lineCenterLineInMiddleDifference.
   */
  boolean lineCenteringIsFinished;
private:
  ExtendedMove* _extMove; ///< The reference to the extended move class.

  /**
   * This variable holds the timestamp when the line centering process was started.
   */
  unsigned long _timeLineCenterStarted;

  unsigned long _timeLastLineCenterCheck; ///< Holds the timestamp of the time when the last curve left check was done. @see doJob()

  /**
   * centeringFromLeft = true means that the car is reaching the line from the left hand side so the car has to constantly do a left curve until it is centered.
   * centeringFromLeft = false means that the car is reaching the line from the right hand side so the car has to constantly do a right curve until it is centered.
   */
  boolean _centeringFromLeft;

  void doFirstStopCorrection();
  void doCorrection();


};
#endif










