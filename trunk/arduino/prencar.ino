#include "Configuration.h"
#include "Communication.h"
#include "StateMaschine.h"

Communication* com;
StateMaschine* stateMaschine;

void setup()
{
  //We get the instance the first time here. This also means that the constructor is called the first time from within
  //this setup method. This is necessary in order to make the serial communication work correctly.
  //For example Communication* com5 = Communication::getInstance(); outside of that function would cause that
  //the serial communication doesn't work.
  com = Communication::getInstance();
  stateMaschine = StateMaschine::getInstance();
}

void loop()
{
  //several jobs that have to be performed always.
  com->doJob();
  stateMaschine->doJob();
}














