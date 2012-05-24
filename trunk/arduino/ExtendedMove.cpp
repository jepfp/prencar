#include "ExtendedMove.h"

ExtendedMove ExtendedMove::_instance; ///< Static reference to the singleton object
boolean ExtendedMove::_instanceCreated = false;

/**
 * Get the ExtendedMove instance
 * @return Pointer to the ExtendedMove instance
 */
ExtendedMove* ExtendedMove::getInstance()
{
  if ( !_instanceCreated ){
    _instance = ExtendedMove();
    _instanceCreated = true;

    //do the configuration stuff so that a call to begin() from the outside of the
    //class is not necessary
    _instance.begin();
  }
  return &_instance;
}

ExtendedMove::ExtendedMove(){
  _currentCommandQueueId = 0;
  _queueExecutionInProgress = false;
}

/**
 * Prepares the ExtendedMove object to work together with other classes. The preparations done in here can not be
 * done from within the constructor because of dependencies such as Serial communication.
 * <b>This method needs to be called before this class can be fully used.</b>
 */
void ExtendedMove::begin(){
  _com = Communication::getInstance();
  _move = Move::getInstance();
  _conf = Configuration::getInstance();
}

/**
 * Does the job for which this class was written.<br>
 * This method has to be called in regular intervals.<br>
 * Executes the commands inside commandQueue one after the other.
 */
void ExtendedMove::doJob(){
  if(!_queueExecutionInProgress) return;

  //check the currently executed command if it's time to finish up and take the next one.
  MoveCommand* mc = getCurrentCommandInQueue();
  if(millis() > mc->startedMillis + mc->duration){
    //_com->send(--, millis());
    //Are no more commands available?
    if(_currentCommandIndexInQueue == _currentCommandQueueSize - 1){
      _com->send(80, _currentCommandQueueId);
      _move->performFastStop();
      _queueExecutionInProgress = false;
      return;
    }
    _currentCommandIndexInQueue++;
    executeCommand(getCurrentCommandInQueue());
  }
}

/**
 * Starts working on the command queue. Put the commands into the ExtendedMove::commandQueue and call this method afterwords.
 * @param commandQueueSize Amount of commands that shall be executed.
 * @return The id of that queue. An number that is increased every time when a call to startCurrentQueue() is done.
 */
int ExtendedMove::startCurrentQueue(int commandQueueSize){
  int parameters[2];
  _currentCommandQueueId++;
  parameters[0] = _currentCommandQueueId;
  _currentCommandQueueSize = commandQueueSize;
  parameters[1] = _currentCommandQueueSize;

  _currentCommandIndexInQueue = 0;
  _queueExecutionInProgress = true;
  //start with the first command
  _com->send(82, parameters, 2);
  executeCommand(getCurrentCommandInQueue());
  return _currentCommandQueueId;
}

/**
 * Executes the given command.
 */
void ExtendedMove::executeCommand(MoveCommand* mc){
  int parameters[4];
  parameters[0] = _currentCommandIndexInQueue;
  parameters[1] = _currentCommandQueueSize;
  parameters[2] = _currentCommandQueueId;
  parameters[3] = mc->duration;
  _com->send(81, parameters, 4);
  _move->controlMotors(mc->dirLeftMotor, mc->speedLeftMotor, mc->dirRightMotor, mc->speedRightMotor);
  mc->startedMillis = millis();
  //_com->send(--, millis());
}

/**
 * Checks if at the moment an execution is in progress and returns true if so and if the currentCommandQueueId matches the given id.
 */
boolean ExtendedMove::isExecutionInProcess(int queueId){
  if(queueId != _currentCommandQueueId) return false;
  return _queueExecutionInProgress;
}

/**
 * Returns true if at the moment an execution is in progress otherwise false.
 */
boolean ExtendedMove::isExecutionInProcess(){
  return _queueExecutionInProgress;
}

/**
 * Returns the id of the last executed or current executing command queue.
 */
int ExtendedMove::getCurrentCommandQueueId(){
  return _currentCommandQueueId;
}

/**
 * Returns the command that is currently in execution or was executed last.
 */
MoveCommand* ExtendedMove::getCurrentCommandInQueue(){
  return &commandQueue[_currentCommandIndexInQueue];
}
/**
 * Returns the index of the command that is currently in execution or was executed last.
 */
int ExtendedMove::getCurrentCommandIndexInQueue(){
  return _currentCommandIndexInQueue;
}

/**
 * Stops the currently executed queue if one is in execution at the moment.
 */
void ExtendedMove::stopCurrentQueue(){
  _queueExecutionInProgress = false;
}



