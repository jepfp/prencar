#ifndef ExtendedMove_h
#define ExtendedMove_h

#include "Arduino.h"
#include "Move.h"

struct MoveCommand{
  unsigned long startedMillis; ///< Will be set as soon as the execution of this command starts.
  int duration; ///< Defines for how long this command shall be executed (milliseconds).
  TMotorDirection dirLeftMotor;
  byte speedLeftMotor;
  TMotorDirection dirRightMotor;
  byte speedRightMotor;
};

/**
 * \brief Allows to define extended car control instuctions.
 * 
 * What can be done with ExtendedMove:
 * <ul>
 * <li>The car can be controlled for a defined amount of time. ExtendedMove takes care of stoping the car after this amount of time.</li>
 * <li>A queue of commands can be executed.</li>
 * <li></li>
 * </ul>
 */
class ExtendedMove
{
private:
  static ExtendedMove _instance;
  static boolean _instanceCreated;
  static int const _MAXAMOUNTOFQUEUEDCOMMANDS = 10; ///< The max amount of commands the command queue can hold.

  Communication* _com; ///< The reference to the serial communication class.
  Move* _move; ///< The reference to the move class to control the motors.
  Configuration* _conf; ///< The reference to the configuration class.

  boolean _queueExecutionInProgress; ///< Set to false if the last queue has been executed fully.
  int _currentCommandQueueId; ///< Id of the current executed command queue (will be increased each time a new command arrives)
  int _currentCommandQueueSize;
  int _currentCommandIndexInQueue; ///< Index of command that is currently in execution.
  void executeCommand(MoveCommand* mc);

  ExtendedMove();
  void begin();
public:
  static ExtendedMove* getInstance();
  MoveCommand commandQueue[_MAXAMOUNTOFQUEUEDCOMMANDS]; ///< Current queue with commands that are executed.

  int startCurrentQueue(int commandQueueSize);
  void doJob();

  boolean isExecutionInProcess(int queueId);
  boolean isExecutionInProcess();

  int getCurrentCommandQueueId();
  MoveCommand* getCurrentCommandInQueue();
  int getCurrentCommandIndexInQueue();
  void stopCurrentQueue();

};

#endif
















