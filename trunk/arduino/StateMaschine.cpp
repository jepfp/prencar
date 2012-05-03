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
  forceChangeSate = false;
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
  curveLeft.begin();
  _liftCube->liftUp();
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible). It manages the parcours
 * with it's states.
 */
void StateMaschine::doJob(){

  checkCommands();

  if(parcoursState == followingFirstLine){
    //go to the next state if the car has reached the curve (see LineFollow::hasReachedCurve)
    if(lineFollow.hasReachedCurve || forceChangeSate){
      changeState(firstTurn);
      curveLeft.startIt();
    }
    else{
      lineFollow.doJob();
    }
  }
  else if(parcoursState == firstTurn){
    //go to the next state if the car has finished the curve (see CurveLeft::drivingCurveIsFinished)
    if(curveLeft.drivingCurveIsFinished || forceChangeSate){
      changeState(followingSecondLine);
      lineFollow.startIt();
    }
    else{
      curveLeft.doJob();
    }
  }
  else if(parcoursState == followingSecondLine){
    //go to the next state if the car has reached the curve (see LineFollow::hasReachedCurve)
    if(lineFollow.hasReachedCurve || forceChangeSate){
      changeState(secondTurn);
      curveLeft.startIt();
    }
    else{
      lineFollow.doJob();
    }
  }
  else if(parcoursState == secondTurn){
    //go to the next state if the car has finished the curve (see CurveLeft::drivingCurveIsFinished)
    if(curveLeft.drivingCurveIsFinished || forceChangeSate){
      //changeState(followingThirdLineToCube);
      //for now just stop the car
      _move->performFastStop();
      changeState(finished);
    }
    else{
      curveLeft.doJob();
    }
  }

  //set forceChangeSate back to false in order to continue normally
  forceChangeSate = false;
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
      int* parameters = c.parameters;
      int highestImplementedState = 4;
      if(parameters[0] > highestImplementedState){
        int p[2];
        p[0] = parameters[0];
        p[1] = highestImplementedState;
        _com->send(151, p, 2);
        stopParcours();
      }
      else{
        //send the current configuration
        _com->sendCurrentConfiguration();
        //if the desired state is > than 1, start with that state. If the parameter is 0 or 1 just start with
        //the first state.
        if(parameters[0] > 1){
          parcoursState = (TParcoursState)(parameters[0] - 1);
          forceChangeSate = true;
          _com->send(205, parcoursState + 1);
        }
        else{
          startParcours();
        }
      }
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

    //check for command 402
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 402)){
      int* parameters = c.parameters;
      if(parameters[0] == 1){
        _liftCube->liftDown();
      }
      else{
        _liftCube->liftUp();
      }
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
  changeState(followingFirstLine);
  lineFollow.startIt();
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























