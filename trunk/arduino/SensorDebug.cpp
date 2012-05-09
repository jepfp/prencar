#include "Arduino.h"
#include "SensorDebug.h"

SensorDebug SensorDebug::_instance; ///< Static reference to the singleton object
boolean SensorDebug::_instanceCreated = false;

/**
 * Get the SensorDebug instance
 * @return Pointer to the sensorDebug instance
 */
SensorDebug* SensorDebug::getInstance()
{
  if ( !_instanceCreated ){
    _instance = SensorDebug();
    _instanceCreated = true;

    //do the configuration stuff so that a call to begin() from the outside of the
    //class is not necessary
    _instance.begin();
  }
  return &_instance;
}

/**
 * Constructur for the class.
 */
SensorDebug::SensorDebug()
{

}

/**
 * Prepares the SensorDebug object to work together with other classes. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void SensorDebug::begin(){
  _com = Communication::getInstance();
  _conf = Configuration::getInstance();

  //left line sensor pin
  sensorJunctionPoints[0].active = true;
  sensorJunctionPoints[0].pin = _conf->lineFollowLeftSensorPin;
  sensorJunctionPoints[0].messageCode = 10;

  //right line sensor pin
  sensorJunctionPoints[1].active = true;
  sensorJunctionPoints[1].pin = _conf->lineFollowRightSensorPin;
  sensorJunctionPoints[1].messageCode = 11;
  
  //left front line sensor pin
  sensorJunctionPoints[2].active = true;
  sensorJunctionPoints[2].pin = _conf->lineFollowLeftFrontSensorPin;
  sensorJunctionPoints[2].messageCode = 12;
  
  //right front line sensor pin
  sensorJunctionPoints[3].active = true;
  sensorJunctionPoints[3].pin = _conf->lineFollowRightFrontSensorPin;
  sensorJunctionPoints[3].messageCode = 13;
  
  //cube approach left top sensor
  sensorJunctionPoints[4].active = true;
  sensorJunctionPoints[4].pin = _conf->cubeApproachLeftBottomSensor;
  sensorJunctionPoints[4].messageCode = 14;
  
  //cube approach left bottom sensor
  sensorJunctionPoints[5].active = true;
  sensorJunctionPoints[5].pin = _conf->cubeApproachLeftTopSensor;
  sensorJunctionPoints[5].messageCode = 15;
  
  //cube approach right bottom sensor
  sensorJunctionPoints[6].active = true;
  sensorJunctionPoints[6].pin = _conf->cubeApproachRightBottomSensor;
  sensorJunctionPoints[6].messageCode = 16;
  
  //cube approach right top sensor
  sensorJunctionPoints[7].active = true;
  sensorJunctionPoints[7].pin = _conf->cubeApproachRightTopSensor;
  sensorJunctionPoints[7].messageCode = 17;

  for(int i = 0; i < AMOUNTOFSENSORJUNCTIONPOINTS; i++){
    byte pin = sensorJunctionPoints[i].pin;

    pinMode(pin, INPUT);

    //make sure, the pull up resistors are off
    digitalWrite(pin, LOW);
  }

  _timeLastSensorDebugCheck = millis();
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible).<br>
 * The sensor data of the sensors is captured and sent over the serial interface.
 */
void SensorDebug::doJob(){
  Command c;

  //check for command 201
  if(_com->getAndRemoveCommandFromReadyCommands(&c, 201)){
    int* parameters = c.parameters;
    if(parameters[0] == 1){
      _debuggerActive = true;
      //backup the current state of _conf->activateMessageFilter
      _activateMessageFilterStateB4Start = _conf->activateMessageFilter;
      _conf->activateMessageFilter = false;
      _com->send(208, 0);
      _com->send(206);
    }
    else{
      _debuggerActive = false;
      //rescue the state of _conf->activateMessageFilter
      _conf->activateMessageFilter = _activateMessageFilterStateB4Start;
      _com->send(208, _conf->activateMessageFilter ? 1 : 0);
      _com->send(207);
    }
  }

  if(_debuggerActive){
    if(_conf->sensorDebugInterval == 0 || millis() > _timeLastSensorDebugCheck + _conf->sensorDebugInterval){
      _timeLastSensorDebugCheck = millis();

      for(int i = 0; i < AMOUNTOFSENSORJUNCTIONPOINTS; i++){
        readSensorAndSendValue(&sensorJunctionPoints[i]);
        delay(_conf->sensorDebugReadGap);
      }
    }
  }
}

void SensorDebug::readSensorAndSendValue(TSensorJunctionPoint* sjp){
  /*_com->send(240, sjp->active);
  _com->send(241, sjp->pin);
  _com->send(242, sjp->messageCode);*/
  
  if(!sjp->active) return;

  int value = analogRead(sjp->pin);
  _com->send(sjp->messageCode, value);
}








