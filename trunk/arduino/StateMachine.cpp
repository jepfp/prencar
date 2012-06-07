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
  extMoveCommandDriveOverFinishLineStarted = false;
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

  pinMode(_conf->accuPin, INPUT);
  //make sure, the pull up resistors are off
  digitalWrite(_conf->accuPin, LOW);

  lineFollow.begin();
  curveLeft.begin();
  _cubeApproach.begin();
  _moveBackToLine.begin();
  _liftCube->liftUp();
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals (as short as possible). It manages the parcours
 * with it's states.
 */
void StateMachine::doJob(){

  checkCommands();

  //The lift cube job is done always so that the car can lift the cube in every possible state (fall back).
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
    /**
     * @todo add an else if statement that also checks if lineFollow.hasReachedCurve for the case if the cube was not detected but the
     * car is at the end of the parcours.
     */
    if(_cubeApproach.amountOfCubeDetections > 0 || forceChangeState){
      changeState(cubeApproach);
      if(forceChangeState){
        //if we jump directly into the next state, we need to call the startIt method manually because that needs to be done every
        //parcours start and in the normal flow is done at the and of the previous state (secondTurn)
        _cubeApproach.startIt(); 
      }
    }
    else if(_liftCube->liftingStarted){
      //fallback: If the cube would switch the end sensor even though the car is not yet in the "lift cube" state --> switch to that state
      _com->send(103);
      changeState(liftCube);
    }
    else{
      _cubeApproach.doJob(true);
    }
  }
  else if(parcoursState == cubeApproach){
    //go to the next state if the car has detected the cube in the center
    if((_cubeApproach.amountOfCubeDetections > 0 && _cubeApproach.cubeDetections[_cubeApproach.amountOfCubeDetections - 1].cubeIsCentered == true) || forceChangeState){
      changeState(liftCube);
      //drive until the cube is lifted
      _move->controlMotors(forward, _conf->cubeApproachStraightSpeed, forward, _conf->cubeApproachStraightSpeed);
    }
    else if(_liftCube->liftingStarted){
      //fallback: If the cube would switch the end sensor even though the car is not yet in the "lift cube" state --> switch to that state
      _com->send(103);
      changeState(liftCube);
    }
    else{
      _cubeApproach.doJob(false);
    }
  }
  else if(parcoursState == liftCube){
    //go to the next state if the car has lifted the cube
    if(_liftCube->cubeLifted || forceChangeState){
      //don't go to the next state if still a car stopping process is being executed.
      if(!_extMove->isExecutionInProcess()){
        changeState(moveBackToLine);
        _moveBackToLine.startIt(&_cubeApproach);
      }
      else{
        //send a message that the state won't get changed yet because the execution of the stop command is still in progress.
        _com->send(92);
      }
    }
    else{
      //the lift cube job is done always so nothing needs to be done in here...
    }
  }
  else if(parcoursState == moveBackToLine){
    //go to the next state if the car is centered on the line again or if an error has occured.
    if(_moveBackToLine.isBackOnLine || _moveBackToLine.errorNoCubeApproachInformation || forceChangeState){
      _extMove->stopCurrentQueue(); //just, if there are still any executions in progress.
      changeState(followingThirdLineToFinish);
      lineFollow.startIt(_conf->lineFollowInitialSpeedLeft, _conf->lineFollowInitialSpeedRight,
      _conf->lineFollowReduceSpeedTimeThirdLineToFinish, _conf->lineFollowReducedSpeedLeft, _conf->lineFollowReducedSpeedRight);
    }
    else{
      _moveBackToLine.doJob();
    }
  }
  else if(parcoursState == followingThirdLineToFinish){
    //go to the next state if the car has detected the cube in the center
    if(lineFollow.hasReachedCurve || forceChangeState){
      if(!extMoveCommandDriveOverFinishLineStarted){
        startDriveOverFinishLineExtMoveCommand();
      }
      else if(extMoveCommandDriveOverFinishLineStarted && !_extMove->isExecutionInProcess()){
        changeState(finished);
        _com->send(209, (millis() - startParcoursTimestamp));
      }
    }
    else{
      lineFollow.doJob();
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

    //check for command 103
    if(_com->getAndRemoveCommandFromReadyCommands(&c, 103)){
      int voltageLevel = getCurrentBatteryVoltageLevel();
      _com->send(104, voltageLevel);
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
  int voltageLevel = getCurrentBatteryVoltageLevel();
  _com->send(104, voltageLevel);
  startParcoursTimestamp = millis();
  extMoveCommandDriveOverFinishLineStarted = false;
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
  extMoveCommandDriveOverFinishLineStarted = false;
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

/**
 * Creates an Extended Move command which moves the car over the finish line and stops it afterwords.
 * It also sets extMoveCommandDriveOverFinishLineStarted to true as soon as this method is called.
 */
void StateMachine::startDriveOverFinishLineExtMoveCommand(){
  extMoveCommandDriveOverFinishLineStarted = true;

  //create an extMove command and add start it's execution
  MoveCommand* mc = _extMove->commandQueue;
  mc[0].duration = _conf->finishLineDriveOverDuration;
  mc[0].dirLeftMotor = forward;
  mc[0].speedLeftMotor = _conf->lineFollowInitialSpeedLeft;
  mc[0].dirRightMotor = forward;
  mc[0].speedRightMotor = _conf->lineFollowInitialSpeedRight;

  mc[1].duration = 250;
  mc[1].dirLeftMotor = backwards;
  mc[1].speedLeftMotor = 130;
  mc[1].dirRightMotor = backwards;
  mc[1].speedRightMotor = 130;

  _extMove->startCurrentQueue(2);
}

int StateMachine::getCurrentBatteryVoltageLevel(){
  return analogRead(_conf->accuPin);
}













































