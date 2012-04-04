#include "Arduino.h"
#include "LineFollow.h"
#include "Move.h"
#include "Configuration.h"

/**
 * Constructur for the class with the hardware pins to use.
 */
LineFollow::LineFollow()
{

}

/**
 * Prepares the LineFollow object to work together with other classes. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void LineFollow::begin(){
  _com = Communication::getInstance();
  _move = Move::getInstance();
  _conf = Configuration::getInstance();

  pinMode(_conf->lineFollowLeftSensorPin, INPUT);
  pinMode(_conf->lineFollowRightSensorPin, INPUT);
  pinMode(_conf->lineFollowLeftFrontSensorPin, INPUT);
  pinMode(_conf->lineFollowRightFrontSensorPin, INPUT);

  //make sure, the pull up resistors are off
  digitalWrite(_conf->lineFollowLeftSensorPin, LOW);
  digitalWrite(_conf->lineFollowRightSensorPin, LOW);
  digitalWrite(_conf->lineFollowLeftFrontSensorPin, LOW);
  digitalWrite(_conf->lineFollowRightFrontSensorPin, LOW);

  hasReachedLine = false;
}

/**
 * Starts the line following process.
 */
void LineFollow::startIt(){
  _deltasensor = 0;
  _sensoralt = 20;
  _deltaPWM = 0;
  _timeSinceCorrectionStarted = 0;

  _move->controlMotors(forward, _conf->lineFollowInitialSpeedLeft, forward, _conf->lineFollowInitialSpeedRight);
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible).<br>
 * The sensor data of the sensors is captured and based on the result
 * the motor speed is adjusted.
 * 
 * What's done in detail:
 * <ol>
 * <li>Read the front sensors to check, if a 90° courve is needed.</li>
 * <li>Check - if a correction (driving a courve to get back to the line) is in progress - if it's already time to drive straight forward again.</li>
 * <li>Read the line sensor values to check if one line sensor has detected white ground which means that a correction is needed. If so a correction will
 * be started.</li>
 * </ol>
 *
 * Not every task is done every time this function gets called.
 * <ul>
 * <li>The two line sensors are not read if a correction is in progress.</li>
 * </ul>
 */
void LineFollow::doJob(){
  int sensorValues[2];
  readFrontLineSensors(sensorValues);

  /*
  //This statements are just to see if we can make the car moving straight forward.
   _move->controlMotors(forward, _conf->lineFollowInitialSpeed, forward, _conf->lineFollowInitialSpeed + 5);
   delay(5000);
   _move->performFastStop();*/

  //check if a 90° curve is needed
  if(sensorValues[0] < _conf->lineFollowWhiteThreshold){
    //_com->sendString("fast stop should be done now.");
    ///@todo Implement what needs to be done when the 90° curve needs to be driven. For now just perform a fast stop
    /*_com->send(55, sensorValues[0]);
     _move->performFastStop();
     hasReachedLine = true;
     return;*/
  }

  //Check - if a correction (driving a courve to get back to the line) is in progress - if it's already time to drive
  //straight forward again.
  /*if(_timeSinceCorrectionStarted != 0 && millis() >= _timeSinceCorrectionStarted + _conf->lineCorrectionDuration){
   //    _com->sendString("after correction - moving forward again");
   _move->controlMotors(forward, _conf->lineFollowInitialSpeed, forward, _conf->lineFollowInitialSpeed);
   _timeSinceCorrectionStarted = 0;
   }*/

  //Read the line sensor values to check if one line sensor has detected white ground which means that a
  //correction is needed. If so a correction will be started.
  //We do this only if no correction is in progress
  //_com->send(_timeSinceCorrectionStarted);
  if(_timeSinceCorrectionStarted == 0){
    readLineSensors(sensorValues);

    _deltasensor = sensorValues[0]-sensorValues[1];
    _com->send(3,_deltasensor);

    _deltaPWM = _conf->lineFollowKp * (_deltasensor/100) + (_conf->lineFollowKd/100) * (_deltasensor-_sensoralt);
    _sensoralt = _deltasensor;

    _move->changeMotorSpeedBasedOnInitialSpeed(_deltaPWM,(-1)*_deltaPWM); 
    //_timeSinceCorrectionStarted = millis(); 



    /*{
     //    _com->sendString("check line sensors");
     readLineSensors(sensorValues);
     
     //left sensor
     if(sensorValues[0] < _conf->lineFollowWhiteThreshold){
     _com->sendString("korrigiere nach rechts.");
     _move->changeMotorSpeed(((-1)*((int)_conf->lineFollowCorrectionMinus)), ((int)_conf->lineFollowCorrectionPlus));
     _timeSinceCorrectionStarted = millis();
     _com->send(56, sensorValues[0]);
     }
     
     //right sensor
     if(sensorValues[1] < _conf->lineFollowWhiteThreshold){
     _com->sendString("korrigiere nach links.");
     _move->changeMotorSpeed(((int)_conf->lineFollowCorrectionPlus), ((-1)*((int)_conf->lineFollowCorrectionMinus)));
     _timeSinceCorrectionStarted = millis();
     _com->send(57, sensorValues[1]);
     }*/
  }


}

/**
 * Reads the two front line sensors and returns their values.
 * @param Pointer to an int array where the two result values will be filled in. [0] front left sensor; [1] front right sensor
 */
void LineFollow::readFrontLineSensors(int* resultArray){

  resultArray[0] = analogRead(_conf->lineFollowLeftFrontSensorPin);
  ///@todo Maybe a delay is needed between the readings.
  resultArray[1] = analogRead(_conf->lineFollowRightFrontSensorPin);

  _com->send(1, resultArray, 2);
}

/**
 * Reads the line sensors (but not the front sensors) and returns their values.
 * @param Pointer to an int array where the two result values will be filled in. [0] left sensor; [1] right sensor
 */
void LineFollow::readLineSensors(int* resultArray){

  resultArray[0] = analogRead(_conf->lineFollowLeftSensorPin);
  ///@todo Maybe a delay is needed between the readings.
  resultArray[1] = analogRead(_conf->lineFollowRightSensorPin);

  _com->send(2, resultArray, 2);
}

/**
 * \brief Calibrates the sensors.
 *
 * This method can be called before the car starts with it's parcours. It evaluates the max and min values
 * for the sensor when it's physical position is on black or white ground. For every position 50 measurements are taken and
 * stored afterwords.<br>
 * This method uses serial communication to instruct the person where to place the car.<br><br>
 * <b>Important: All measurements are done using to front left sensor. The other sensors are not measured separately
 * since we have equivalent sensors.</b>
 * <ol>
 * <li>Calibration function is called. Place the front left sensor on black ground. The microcontroller sleeps for 10 seconds.</li>
 * <li>50 measurements are taken (time between measurements: 100ms).</li>
 * <li>The minimum and maximum measured value are evaluated and stored into _calibrationSensorBlackMin and _calibrationSensorBlackMax.</li>
 * <li>Place the front left sensor on white ground. The microcontroller sleeps for 10 seconds.</li>
 * <li>50 measurements are taken (time between measurements: 100ms).</li>
 * <li>The minimum and maximum measured value are evaluated and stored into _calibrationSensorWhiteMin and _calibrationSensorWhiteMax.</li>
 * <li>Out of this measurements the Configuration::lineFollowWhiteThreshold value gets calculated and stored. It will be +100 than _calibrationSensorWhiteMax.</li> 
 * </ol>
 * @see Configuration::lineFollowWhiteThreshold
 * @todo The calculation of Configuration::lineFollowWhiteThreshold can be done using some config value as well so that the behavior can be influenced by the configuration.
 */
void LineFollow::calibrateSensors(){
  float sensorValue;
  _com->send(200);
  delay(10000);
  _com->send(201);

  //sensor measurement on black ground
  _calibrationSensorBlackMin = 1023;
  _calibrationSensorBlackMax = 0;
  for(byte i = 0; i < 50; i++){
    sensorValue = analogRead(_conf->lineFollowLeftFrontSensorPin);
    if(sensorValue < _calibrationSensorBlackMin){
      _calibrationSensorBlackMin = sensorValue;
    }
    if(sensorValue > _calibrationSensorBlackMax){
      _calibrationSensorBlackMax = sensorValue;
    }
    delay(100);
  }

  _com->send(202);
  delay(10000);
  _com->send(201);

  //sensor measurement on white ground
  _calibrationSensorWhiteMin = 1023;
  _calibrationSensorWhiteMax = 0;
  for(byte i = 0; i < 50; i++){
    sensorValue = analogRead(_conf->lineFollowLeftFrontSensorPin);
    if(sensorValue < _calibrationSensorWhiteMin){
      _calibrationSensorWhiteMin = sensorValue;
    }
    if(sensorValue > _calibrationSensorWhiteMax){
      _calibrationSensorWhiteMax = sensorValue;
    }
    delay(100);
  }

  //calculate the white threshold value
  int rangeWhiteMaxToBlackMin = _calibrationSensorBlackMin - _calibrationSensorWhiteMax;
  //calculate 1/3 out of the range and add it to the whiteMax. This will be the new whiteThreshold value.
  //(We know that the calculation doesn't use float even though a division is made here.)
  int oneThirdOfRange = rangeWhiteMaxToBlackMin/3;
  _conf->lineFollowWhiteThreshold = _calibrationSensorWhiteMax + oneThirdOfRange;

  int measurements[] = {
    _calibrationSensorBlackMin, _calibrationSensorBlackMax, _calibrationSensorWhiteMin, _calibrationSensorWhiteMax,
    oneThirdOfRange, _conf->lineFollowWhiteThreshold
  };

  _com->send(203, measurements, 6);
}




























