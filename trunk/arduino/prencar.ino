#include "LineFollow.h"
#include "Configuration.h"
#include "Move.h"
#include "Communication.h"

Communication* com;
LineFollow lineFollow;

void setup()
{
  //We get the instance the first time here. This also means that the constructor is called the first time from within
  //this setup method. This is necessary in order to make the serial communication work correctly.
  //For example Communication* com5 = Communication::getInstance(); outside of that function would cause that
  //the serial communication doesn't work.
  com = Communication::getInstance();
  lineFollow.begin();
}

void loop()
{
  /*com->sendString("Kalibrierung der Sensoren (Methode) startet in 2 Sekunden");
   delay(2000);
   lineFollow.calibrateSensors();*/

//  delay(3000);
  //com->sendCurrentConfiguration();
  com->doJob();
  /*com->sendString("LineFollow startet in 5 Sekunden.");
   delay(5000);
   lineFollow.startIt();
   while(!lineFollow.hasReachedLine){
   //com->sendString("Do job now!");
   lineFollow.doJob();
   delay(1000);
   }
   com->sendString("Beendet, warte 10 Sekunden.");
   lineFollow.hasReachedLine = false;
   delay(10000);*/
}














