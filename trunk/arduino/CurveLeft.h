#ifndef CurveLeft_h
#define CurveLeft_h

#include "Arduino.h"
#include "LineFollow.h"

/**
 * \brief The CurveLeft class will control the car during the 90° left curve.
 * 
 * This class will be started by the state maschine when the 90° angle of the white line is reached (with other words when the left front line sensor
 * has detected white ground).
 *
 * <h3>How the curve is driven</h3>
 * <ol>
 * <li>The car will start driving a left curve. The angle / speed of how fast this curve is driven is defined in Configuration::curveSpeedSlowMotor and
 * Configuration::curveSpeedFastMotor.</li>
 * <li>After Configuration::curveActivateEndSensorOffset milliseconds the left line sensor will be activated.</li>
 * <li>The car continues driving a left curve until the left line sensor measures white ground again.</li>
 * <li>As soon as white ground has been detected white ground the drivingCurveIsFinished boolean will be set to true indicating that the "drive
 * left curve" procedure has finished.</li>
 * </ol>
 * @see Configuration::curveSpeedSlowMotor
 * @see Configuration::curveSpeedFastMotor
 * @see Configuration::curveActivateEndSensorOffset
 */
class CurveLeft : public LineFollow
{
public:
  CurveLeft();
  virtual void begin();
  virtual void doJob();
  virtual void startIt();
  
  /** \brief This variable is set to true as soon as the curve is driven fully.
   * 
   * With other words this means that the left line sensor has detected white
   * ground so we can continue with the normal line following (this will start with a
   * correction to the left side of course).
   */
  boolean drivingCurveIsFinished;
private:

  /**
   * This variable holds the timestamp when the curve left was started.
   */
  unsigned long _timeCurveLeftStarted;
  
  unsigned long _timeLastCurveLeftCheck; ///< Holds the timestamp of the time when the last curve left check was done. @see doJob()

  
};
#endif









