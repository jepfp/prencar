#ifndef SensorDebug_h
#define SensorDebug_h

#include "Arduino.h"
#include "Communication.h"
#include "Configuration.h"

typedef struct { 
  boolean active; ///< Defines if the sensor shall be read and the data sent to the computer.
  byte pin;  ////< Pin to which the sensor is attached.
  byte messageCode; ///< Message code with which the measurements of that sensor shall be sent.
} 
TSensorJunctionPoint; 

#define AMOUNTOFSENSORJUNCTIONPOINTS 8

class SensorDebug
{
public:
  static SensorDebug* getInstance();
  void begin();
  void doJob();
  boolean hasReachedCurve; ///< Is set to true, if the left curve line is reached
  TSensorJunctionPoint sensorJunctionPoints[AMOUNTOFSENSORJUNCTIONPOINTS];
private:
  static SensorDebug _instance;
  static boolean _instanceCreated;

  Communication* _com; ///< The reference to the serial communication class.
  Configuration* _conf; ///< The reference to the configuration class.

  unsigned long _timeLastSensorDebugCheck; ///< Holds the timestamp of the time when the last sensor check check was done. @see doJob()
  boolean _debuggerActive; ///< Defines if the sensor debug modul is active or not.
  SensorDebug();

  void readSensorAndSendValue(TSensorJunctionPoint* sjp);
  
  /**
  * SensorDebug changes the value Configuration::activateMessageFilterState temporarily. The previous state is saved
  * in this private variable so that we can recover that state at the end again.
  */
  boolean _activateMessageFilterStateB4Start;
};
#endif









