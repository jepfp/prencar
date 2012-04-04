#include "Configuration.h"
#include "Communication.h"
#include "StateMaschine.h"

Communication* _com;
StateMaschine* _stateMaschine;
Configuration* _conf;

void setup()
{
  //We get the instance the first time here. This also means that the constructor is called the first time from within
  //this setup method. This is necessary in order to make the serial communication work correctly.
  //For example Communication* com5 = Communication::getInstance(); outside of that function would cause that
  //the serial communication doesn't work.
  _conf = Configuration::getInstance();
  _com = Communication::getInstance();
  _stateMaschine = StateMaschine::getInstance();
}

void loop()
{
  if(_conf->doJobDelay > 0){
    _com->send(101, _conf->doJobDelay);
    delay(_conf->doJobDelay);
  }
  
  //several jobs that have to be performed always.
  _com->doJob();
  _stateMaschine->doJob();
}
















