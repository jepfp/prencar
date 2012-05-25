#include "Arduino.h"
#include "StateMachine.h"

StateMachine StateMachine::_instance; ///< Static reference to the singleton object
boolean StateMachine::_instanceCreated = false;

/**
 * Get the StateMachine instance
 * @return Pointer to the StateMachine instance
 */
StateMachine* StateMachine::getInstance()
{
  if ( !_instanceCreated ){
    _instance = StateMachine();
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
StateMachine::StateMachine()
{
  parcoursState = notStarted;
  forceChangeState = false;
}

/**
 * Prepares the StateMachine object to work.
 */
void StateMachine::begin(){
  _com = Communication::getInstance();
  _conf = Configuration::getInstance();
  _move = Move::getInstance();
  _liftCube = LiftCube::getInstance();
  _extMove = ExtendedMove::getInstance();
  lineFollow.begin();
  curveLeft.begin();
  _cubeApproach.begin();
  _liftCube->liftUp();
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible). It manages the parcours
 * with it's states.
 */
void StateMachine::doJob(){

  checkCommands();

  ///@todo this do job has to go to the right state.
  _liftCube->doJob();

  if(parcoursState == followingFirstLine){
    //go to the next state if the car has reached the curve (see LineFollow::hasReachedCurve)
    if(lineFollow.hasReachedCurve || forceChangeState){
      changeState(firstTurn);
      curveLeft.startIt();
    }
    else{
      lineFollow.doJob();
    }
  }
  else if(parcoursState == firstTurn){
    //go to the next state if the car has finished the curve (see CurveLeft::drivingCurveIsFinished)
    if(curveLeft.drivingCurveIsFinished || forceChangeState){
      changeState(followingSecondLine);
      lineFollow.startIt(_conf->lineFollowInitialSpeedLeft, _conf->lineFollowInitialSpeedRight,
      _conf->lineFollowReduceSpeedTimeSecondLine, _conf->lineFollowReducedSpeedLeft, _conf->lineFollowReducedSpeedRight);
    }
    else{
      curveLeft.doJob();
    }
  }
  else if(parcoursState == followingSecondLine){
    //go to the next state if the car has reached the curve (see LineFollow::hasReachedCurve)
    if(lineFollow.hasReachedCurve || forceChangeState){
      changeState(secondTurn);
      curveLeft.startIt();
    }
    else{
      lineFollow.doJob();
    }
  }
  else if(parcoursState == secondTurn){
    //go to the next state if the car has finished the curve (see CurveLeft::drivingCurveIsFinished)
    if(curveLeft.drivingCurveIsFinished || forceChangeState){
      changeState(followingThirdLineToCube);
      _cubeApproach.startIt();
    }
    else{
      curveLeft.doJob();
    }
  }
  else if(parcoursState == followingThirdLineToCube){
    //go to the next state if the car has detected the cube at least once
    if(_cubeApproach.amountOfCubeDetections > 0 || forceChangeState){
      changeState(cubeApproach);
      if(forceChangeState){
        //if we jump directly into the next state, we need to call the startIt method manually because that needs to be done every
        //parcours start and in the normal flow is done at the and of the previous state (secondTurn)
        _cubeApproach.startIt(); 
      }
    }
    else{
      _cubeApproach.doJob(true);
    }
  }
  else if(parcoursState == cubeApproach){
    //go to the next state if the car has detected the cube in the center
    if((_cubeApproach.amountOfCubeDetections > 0 && _cubeApproach.cubeDetections[_cubeApproach.amountOfCubeDetections - 1].cubeIsCentered == true) || forceChangeState){
      //changeState(liftCube);
      //for now just stop the car
      _move->performFastStop();
      changeState(finished);
      _com->send(209, (millis() - startParcoursTimestamp));
    }
    else{
      _cubeApproach.doJob(false);
    }
  }

  //set forceChangeState back to false in order to continue normally
  forceChangeState = false;
}

/**
 * This method checks different possible arriving commands and performs the defined
 * action if necessary.
 */
void StateMachine::checkCommands(){
  Command c;

  //the following commands are only available, if the parcours is not started
  if(parcoursState == notStarted || parcoursState == finished){
    //check for command 300
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 300)){
      int* parameters = c.parameters;
      int highestImplementedState = 6;
      if(parameters[0] > highestImplementedState){
        int p[2];
        p[0] = parameters[0];
        p[1] = highestImplementedState;
        _com->send(151, p, 2);
        stopParcours();
      }
      else{
        //if the desired state is > than 1, start with that state. If the parameter is 0 or 1 just start with
        //the first state.
        if(parameters[0] > 1){
          startParcoursAtState((TParcoursState)parameters[0]);
        }
        else{
          startParcours();
        }
        /// @todo the following line has to go to the begin of the liftCube state later on!
        //make sure that the cubeLifted bool is set to false
        _liftCube->cubeLifted = false;
      }
    }

    //check for command 101
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 101)){
      int* parameters = c.parameters;
      if(_conf->updateConfiguration(parameters)){
        _com->send(102);
        _com->sendCurrentConfiguration();
      }
      else{
        _com->send(152, _conf->getConfigurationVersion());
      }
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

  //check for command 102
  if(_com->getAndRemoveCommandFromReadyCommands(&c, 102)){
    int* parameters = c.parameters;
    if(parameters[0] == 1){
      changeActivateMessageFilter(true);
    }
    else{
      changeActivateMessageFilter(false);   
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
void StateMachine::startParcours(){
  changeState(followingFirstLine);
  //send the current configuration
  _com->sendCurrentConfiguration();
  startParcoursTimestamp = millis();
  lineFollow.startIt(_conf->lineFollowInitialSpeedLeft, _conf->lineFollowInitialSpeedRight,
  _conf->lineFollowReduceSpeedTimeFirstLine, _conf->lineFollowReducedSpeedLeft, _conf->lineFollowReducedSpeedRight);
}

/**
 * Starts the parcours at the given state. The car will start moving autonomous.
 * @param state Start state.
 */
void StateMachine::startParcoursAtState(TParcoursState state){
  //We go to the previous state of the state where we want to start
  //and perform the actions that are performed at the very end of this previous state by setting forceChangeState to true.
  state = (TParcoursState)((int)state - 1);
  parcoursState = state;
  forceChangeState = true;
  _com->send(205, state + 1);
  //send the current configuration
  _com->sendCurrentConfiguration();
  startParcoursTimestamp = millis();
}

/**
 * Stops the car if it is currently driving autonomously.
 */
void StateMachine::stopParcours(){
  _extMove->stopCurrentQueue();
  _move->performFastStop();
  changeState(notStarted);
}

/**
 * Changes the state of the Machine to the given new state.
 * @param New state for the state maschine.
 */
void StateMachine::changeState(TParcoursState newState){
  _com->send(100, newState);
  parcoursState = newState;
}

/**
 * Changes the state of the boolean Configuration::activateMessageFilter to the given state.
 * @param newState State to which activateMessageFilter shall be set.
 * @see Configuration::activateMessageFilter
 */
void StateMachine::changeActivateMessageFilter(boolean newState){
  _conf->activateMessageFilter = newState;
  if(newState){
    _com->send(208, 1);  
  }
  else{
    _com->send(208, 0); 
  }
}




































