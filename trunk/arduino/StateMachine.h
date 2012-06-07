#ifndef StateMachine_h
#define StateMachine_h

#include "Arduino.h"
#include "Communication.h"
#include "Configuration.h"
#include "LineFollow.h"
#include "CurveLeft.h"
#include "MoveBackToLine.h"
#include "LiftCube.h"
#include "CubeApproach.h"
#include "ExtendedMove.h"

/**
 * The enum with values about the current state.
 */
enum TParcoursState{ 
  notStarted,
  followingFirstLine,
  firstTurn,
  followingSecondLine,
  secondTurn,
  followingThirdLineToCube,
  cubeApproach,
  liftCube,
  moveBackToLine,
  followingThirdLineToFinish,
  finished
};

class StateMachine
{
private:
  StateMachine();
  static StateMachine _instance;
  static boolean _instanceCreated;
  void begin();

  Communication* _com; ///< The reference to the serial communication class.
  Configuration* _conf; ///< The reference to the configuration class.
  Move* _move; ///< The reference to the move class.
  LiftCube* _liftCube; ///< The reference to the LiftCube class.
  ExtendedMove* _extMove; ///< The reference to the ExtendedMove class.

  LineFollow lineFollow;
  CurveLeft curveLeft;
  CubeApproach _cubeApproach;
  MoveBackToLine _moveBackToLine;

  /** \brief Will be set to true if the stateMachine shall switch to the next state immediately.
   * 
   * In the state machine the work of every state is started at the end of the previous state. For example
   * the CurveLeft job starts at the end of the LineFollow job.<br>
   * This boolean is used when the command 300 is sent with a parameter that indicates at which state
   * the state machine shall start. The state machine will then start at the previous (!) state of
   * the given state and set forceChangeState to true. Doing so the state machine starts the job
   * of the next state and immediately switches to the next state.
   */
  boolean forceChangeState;
  
  /**
   * Set to true as soon as the ext move command has been started which moves the car over the finish line
   * and stops the car afterwords.
   */
  boolean extMoveCommandDriveOverFinishLineStarted;

  void checkCommands();
  void changeState(TParcoursState newState);
  void startDriveOverFinishLineExtMoveCommand();
public:
  static StateMachine* getInstance();
  TParcoursState parcoursState;
  unsigned long startParcoursTimestamp; ///< Holds the timestamp when the parcours starts.
  void doJob();
  void startParcours();
  void startParcoursAtState(TParcoursState state);
  void stopParcours();
  void changeActivateMessageFilter(boolean newState);
  int getCurrentBatteryVoltageLevel();
};
#endif






















