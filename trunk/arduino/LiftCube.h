#ifndef LiftCube_h
#define LiftCube_h

#include "Arduino.h"
#include "Communication.h"
#include <Servo.h>

class LiftCube
{
private:
  LiftCube();
  static LiftCube _instance;
  static boolean _instanceCreated;
  void begin();
  byte _currentPosition; ///< Last position of the servo to which it was set (pwm value 0-255)
  unsigned long _liftDownStartTimestamp; ///< Saves the timestamp when the method liftCube is called to know when it's time to lift up again.

  Communication* _com; ///< The reference to the serial communication class.
  Configuration* _conf; ///< The reference to the configuration class.

  boolean _liftingStarted; ///< Will be set to true as soon as the cube lift process starts and will therefore prevent a second call to liftCube().

  Servo _hoistServo;
public:
  static LiftCube* getInstance();

  boolean cubeLifted; ///< Will be set to true as soon as the cube is lifted. If set to true the cube will not be lifted again automatically (a parcours restart is needed first).

  void doJob();
  void liftUp();
  void liftDown();
  void liftCube();
  void setHoistPosition(byte pos);

};
#endif


















