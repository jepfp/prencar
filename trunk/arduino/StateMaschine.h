#ifndef StateMaschine_h
#define StateMaschine_h

#include "Arduino.h"
#include "Communication.h"
#include "Configuration.h"
#include "LineFollow.h"
#include "CurveLeft.h"
#include "LiftCube.h"

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

class StateMaschine
{
private:
  StateMaschine();
  static StateMaschine _instance;
  static boolean _instanceCreated;
  void begin();

  Communication* _com; ///< The reference to the serial communication class.
  Configuration* _conf; ///< The reference to the configuration class.
  Move* _move; ///< The reference to the move class.
  LiftCube* _liftCube; ///< The reference to the LiftCube class.

  LineFollow lineFollow;
  CurveLeft curveLeft;

  /** \brief Will be set to true if the stateMaschine shall switch to the next state immediately.
   * 
   * In the state machine the work of every state is started at the end of the previous state. For example
   * the CurveLeft job starts at the end of the LineFollow job.<br>
   * This boolean is used when the command 300 is sent with a parameter that indicates at which state
   * the state machine shall start. The state machine will then start at the previous (!) state of
   * the given state and set forceChangeState to true. Doing so the state machine starts the job
   * of the next state and immediately switches to the next state.
   */
  boolean forceChangeSate;

  void checkCommands();
  void changeState(TParcoursState newState);
public:
  static StateMaschine* getInstance();
  TParcoursState parcoursState;
  void doJob();
  void startParcours();
  void stopParcours();
};
#endif





















