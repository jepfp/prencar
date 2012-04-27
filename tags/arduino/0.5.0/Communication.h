#ifndef Communication_h
#define Communication_h

#include "Arduino.h"
#include "Configuration.h"
#include "Command.h"

class Communication
{
private:
  static Communication _instance;
  static boolean _instanceCreated;
  Communication();
  Configuration* _config;
  static int const _INCOMINGDATABUFFERSIZE = 128; ///< The buffer size for the incomingDataBuffer
  static int const _READYCOMMANDSSIZE = 15; ///< The amount of commands that can be stored into the readyCommands array.
  byte _incomingDataBuffer[_INCOMINGDATABUFFERSIZE]; ///< Buffer to store ONE incoming line which is not yet completely transmitted
  int _lastBufferCharacter; ///< The position of the last character in the buffer (lastBufferChar + 1 will be the next empty spot)
  void parseAndPutCommandOnList(byte* commandString);
  void checkCommands();
public:  
  static Communication* getInstance();
  void doJob();
  void send(byte messageId);
  void send(byte messageId, int param);
  void send(byte messageId, long const params[], byte paramSize);
  void send(byte messageId, int const params[], byte paramSize);
  void sendString(char* message);
  void sendCurrentConfiguration();
  /** \brief Holds all the commands that are ready for execution.
   * 
   * If a new command is transmitted over the serial interface and parsed as a Command, the object will be stored at the <b>first empty</b> position
   * of this array. Other classes can then check this array and see if there is command ready for themselves to execute. If such a command is found by a class
   * this class has to remove the command from that array by setting the space to null.
   */
  Command readyCommands[_READYCOMMANDSSIZE];
  boolean getAndRemoveCommandFromReadyCommands(Command* c, int commandCode);
  void sendFreeMemory(int placeOfMeasurement);
};

#endif





