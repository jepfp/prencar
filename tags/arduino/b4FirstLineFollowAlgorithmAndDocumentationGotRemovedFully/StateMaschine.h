#ifndef StateMaschine_h
#define StateMaschine_h

#include "Arduino.h"
#include "Communication.h"
#include "Configuration.h"
#include "LineFollow.h"

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

  LineFollow lineFollow;

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


















