#include "Arduino.h"
#include "StateMaschine.h"

StateMaschine StateMaschine::_instance; ///< Static reference to the singleton object
boolean StateMaschine::_instanceCreated = false;

/**
 * Get the StateMaschine instance
 * @return Pointer to the StateMaschine instance
 */
StateMaschine* StateMaschine::getInstance()
{
  if ( !_instanceCreated ){
    _instance = StateMaschine();
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
StateMaschine::StateMaschine()
{
  parcoursState = notStarted;
}

/**
 * Prepares the StateMaschine object to work.
 */
void StateMaschine::begin(){
  _com = Communication::getInstance();
  _conf = Configuration::getInstance();
  _move = Move::getInstance();
  _liftCube = LiftCube::getInstance();
  lineFollow.begin();
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible). It manages the parcours
 * with it's states.
 */
void StateMaschine::doJob(){

  checkCommands();

  if(parcoursState == followingFirstLine){
    lineFollow.doJob();

    //go to the next state if the car has reached the curve (see LineFollow::hasReachedCurve)
    if(lineFollow.hasReachedCurve){
      changeState(firstTurn);
    }
  }
}

/**
 * This method checks different possible arriving commands and performs the defined
 * action if necessary.
 */
void StateMaschine::checkCommands(){
  Command c;

  //the following commands are only available, if the parcours is not started
  if(parcoursState == notStarted || parcoursState == finished){
    //check for command 200
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 200)){
      lineFollow.calibrateSensors();
    }

    //check for command 300
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 300)){
      startParcours();
    }

    //check for command 101
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 101)){
      int* parameters = c.parameters;
      _conf->updateConfiguration(parameters);
      _com->send(102);
    }

    /*----------- offline control car -----------*/
    //check for command 400
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 400)){
      int* parameters = c.parameters;
      _move->controlMotors((TMotorDirection)parameters[0], parameters[1], (TMotorDirection)parameters[2], parameters[3]);
    }

    //check for command 401
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 401)){
      int* parameters = c.parameters;
      _liftCube->setHoistPosition(parameters[0]);
    }
  }

  //check for command 301
  if(_com->getAndRemoveCommandFromReadyCommands(&c, 301)){
    stopParcours();
  }
}

/**
 * Starts the parcours. The car will start moving autonomous.
 */
void StateMaschine::startParcours(){
  lineFollow.startIt();
  changeState(followingFirstLine);
}

/**
 * Stops the car if it is currently driving autonomously.
 */
void StateMaschine::stopParcours(){
  _move->performFastStop();
  changeState(notStarted);
}

/**
 * Changes the state of the Maschine to the given new state.
 * @param New state for the state maschine.
 */
void StateMaschine::changeState(TParcoursState newState){
  parcoursState = newState;
  _com->send(100, newState);
}









