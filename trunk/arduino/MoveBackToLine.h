#ifndef MoveBackToLine_h
#define MoveBackToLine_h

#include "Arduino.h"
#include "LineFollow.h"
#include "ExtendedMove.h"
#include "LineCenter.h"
#include "CubeApproach.h"

/**
 * \brief The MoveBackToLine class will navigate car back to the line according to the stored information from the given cubeApproach object
 * 
 * @todo write a detailed description
 */
class MoveBackToLine : public LineFollow
{
public:
  MoveBackToLine();
  virtual void begin();
  virtual void doJob();
  virtual void startIt(CubeApproach* cubeApproach);
  
  /** \brief This variable is set to true as soon as the line has been reached but the car is not yet centered.
   * 
   * With other words this means that the left (or right) line sensor has detected white
   * ground so we can continue with the normal line following.
   */
  boolean lineHasBeenReached;
  
  /**
   * \brief Will be set to true as soon as the car is back on the line and the line centering process has finished.
   */
  boolean isBackOnLine;
  
  /**
   * \brief Set to true if no cube approach information has been found.
   *
   * The 'move back to line' process can not be started if there is no track back
   * (cube approach recordings which have informations about how the line has been left are empty).
   * In that case, the error flag is set to true.
   */
  boolean errorNoCubeApproachInformation;
private:
  ExtendedMove* _extMove; ///< The reference to the extended move class.
  CubeApproach* _cubeApproach; ///< Reference to a cube approach class with information about how to get back to the line. 
  
  LineCenter lineCenter;

  /**
   * This variable holds the timestamp when the "move back to line" process was started.
   */
  unsigned long _timeMoveBackToLineStarted;
  
  unsigned long _timeLastMoveBackToLineCheck; ///< Holds the timestamp of the time when the last "move back to line" check was done. @see doJob()
  
  /**
   * reachingLineFromLeft = true means that the car is reaching the line from the left hand side so the car has to check the left line sensor.
   * reachingLineFromLeft = false means that the car is reaching the line from the right hand side so the car has to check the right line sensor.
   */
  boolean _reachingLineFromLeft;

  
};
#endif









