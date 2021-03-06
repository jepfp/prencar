#include "Arduino.h"
#include "LiftCube.h"
#include "Configuration.h"
#include "Communication.h"

LiftCube LiftCube::_instance; ///< Static reference to the singleton object
boolean LiftCube::_instanceCreated = false;

/**
 * Get the LiftCube instance
 * @return Pointer to the LiftCube instance
 */
LiftCube* LiftCube::getInstance()
{
  if ( !_instanceCreated ){
    _instance = LiftCube();
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
LiftCube::LiftCube()
{

}

/**
 * Prepares the LiftCube object to work according to the values defined in the configuration.
 * The preparations done in here can not be done from within the constructor because of
 * dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b> In this class it is done
 * when getting an instance of the class the first time.
 * @see getInstance()
 */
void LiftCube::begin(){
  _conf = Configuration::getInstance();
  _com = Communication::getInstance();
  _extMove = ExtendedMove::getInstance();

  pinMode(_conf->liftCubePwmPin, OUTPUT);
  pinMode(_conf->liftCubeSwitchPin, INPUT);

  //make sure the pull up resistor is on
  //switching on the pull up resistor means that the signal will be high as long as
  //the chord attached to that pin is connected to the ground
  digitalWrite(_conf->liftCubeSwitchPin, HIGH);

  liftingStarted = false;
  cubeLifted = false;

  _hoistServo.attach(_conf->liftCubePwmPin);
}

/**
 * Checks if it's already time to lift up the hoist again after liftCube() was called.
 * This method has to be called in regular intervals (as short as possible).<br>
 */
void LiftCube::doJob(){
  int switchState = digitalRead(_conf->liftCubeSwitchPin);
  if(switchState == 0 && liftingStarted == false && cubeLifted == false){
    _com->send(69);
    liftCube();
    liftingStarted = true;

    //stop the moving car to be sure not to drive over the parcours boarders.
    MoveCommand* mc = _extMove->commandQueue;
    mc[0].duration = _conf->liftCubeStopDuration;
    mc[0].dirLeftMotor = backwards;
    mc[0].speedLeftMotor = _conf->liftCubeStopSpeed;
    mc[0].dirRightMotor = backwards;
    mc[0].speedRightMotor = _conf->liftCubeStopSpeed;

    _extMove->startCurrentQueue(1);
  }

  if(_liftDownStartTimestamp != 0 && millis() > _liftDownStartTimestamp + _conf->liftCubeDownUpDuration){
    //move the hoist up
    _com->send(64);
    _liftDownStartTimestamp = 0;
    liftUp();
    cubeLifted = true;
    liftingStarted = false;
  }
}

/**
 * Moves the hoist to the top position according to the configuration.
 * @see Configuration::liftCubeUpPosition
 */
void LiftCube::liftUp()
{
  setHoistPosition(_conf->liftCubeUpPosition);
}

/**
 * Moves the hoist to the bottom position according to the configuration.
 * @see Configuration::liftCubeDownPosition
 */
void LiftCube::liftDown()
{
  setHoistPosition(_conf->liftCubeDownPosition);
}

/**
 * Moves the hoist to the bottom position (according to the configuration), waits for the time defined in
 * Configuration::liftCubeDownUpDuration and then moves the hoist to the top position again.<br>
 * When this method is called, also the car will be stopped (see Configuration::liftCubeStopDuration, Configuration::liftCubeStopSpeed).
 * <b>Make sure the doJob method is called in regular intervals.</b>
 * @see Configuration::liftCubeDownPosition
 * @see Configuration::liftCubeUpPosition
 * @see Configuration::liftCubeDownUpDuration
 */
void LiftCube::liftCube()
{
  _com->send(63);
  _liftDownStartTimestamp = millis();
  liftDown();
}

/**
 * Moves the hoist to the given position (pwm value between 0-255).
 */
void LiftCube::setHoistPosition(byte pos){
  _com->send(65, pos);
  _hoistServo.write(pos);
}




