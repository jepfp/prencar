#include "Communication.h"
#include "Configuration.h"
#include "Command.h"

Communication Communication::_instance; ///< Static reference to the singleton object
boolean Communication::_instanceCreated = false;

#define sc Serial

/**
 * Get the Communication instance
 * @return the Communication instance
 */
Communication* Communication::getInstance()
{
  if ( !_instanceCreated ){
    _instance = Communication();
    _instanceCreated = true;
  }
  return &_instance;
}
/**
 * Constructor
 */
Communication::Communication(){
  //get a reference to the configuration
  _config = Configuration::getInstance();

  _lastBufferCharacter = 0;

  sc.begin(_config->getSerialSpeed());
}

/**
 * Does the job which this class has to do in regular intervals.
 * Therefore this method has to be called in regular intervals (as short as possible).<br>
 *
 * It checks if data is available to read. If so, the data is added to an incoming buffer and as soon
 * as one command (one line) is complete, a Command object is created.
 */
void Communication::doJob(){
  int amountAvailableBytes = sc.available();
  while(amountAvailableBytes > 0){
    _incomingDataBuffer[_lastBufferCharacter] = sc.read();
    //if one line is completely transmitted create a command
    if(_incomingDataBuffer[_lastBufferCharacter] == '\n'){
      parseAndPutCommandOnList(_incomingDataBuffer);
      _lastBufferCharacter = -1;
    }
    _lastBufferCharacter++;
    amountAvailableBytes--;
  }

  checkCommands();
}

/**
 * Returns a pointer to the first free spot in readyCommands (free = command is set to 0)
 */
void Communication::parseAndPutCommandOnList(byte* commandString){
  sendFreeMemory(1);
  //find the first free spot
  int i;
  boolean spotFound = false;
  for(i = 0; i < _READYCOMMANDSSIZE; i++){
    if(readyCommands[i].commandCode == 0){
      spotFound = true;
      break;
    } 
  }
  if(spotFound == false){
    send(150);
    return;
  }
  Command c;
  c.parse(commandString);

  readyCommands[i] = c;

  int parameters[3];
  parameters[0] = c.commandCode;
  parameters[1] = i;
  parameters[2] = _READYCOMMANDSSIZE;
  send(61, parameters, 3);
}

/**
 * Checks the array readyCommands for commands that have to be exectued here and executes them.
 */
void Communication::checkCommands(){
  Command c;

  //check for command 100
  if(getAndRemoveCommandFromReadyCommands(&c, 100)){
    sendCurrentConfiguration();
  }

}

/**
 * Checks the array readyCommands if the given commandCode is in the list. If so it copies the values
 * from the command in the list to the space of the given reference and removes the command from the
 * readyCommands array.
 * @param c Command that will be filled, if the command is in the list. "c.commandCode" will be set to 0 if the command is not in the list
 * @param commandCode The code of the command to look for inside the readyCommands array.
 * @return Returns true if the command was found and removed from the list otherwise false.
 */
boolean Communication::getAndRemoveCommandFromReadyCommands(Command* c, int commandCode){
  for(int i = 0; i < _READYCOMMANDSSIZE; i++){
    if(readyCommands[i].commandCode == commandCode){
      *c = readyCommands[i];

      //remove command from the list
      readyCommands[i].commandCode = 0;

      int parameters[3];
      parameters[0] = c->commandCode;
      parameters[1] = i;
      send(62, parameters, 2);
      return true;
    } 
  }

  //if the command was not found set the commandCode of the given object to 0
  c->commandCode = 0;
  return false;
}

/**
 * Sends the message code over the configured serial interface.<br>
 * The message will be filtered according to the configured filter level in the configuration.
 * @see Configuration::getMessageFilterLevel()
 * @param messageId Id of the message that will be sent.
 */
void Communication::send(byte messageId){
  if(messageId > _config->getMessageFilterLevel()){
    sc.println(messageId);
  }
}

/**
 * \overload
 * @param messageId Id of the message that will be sent.
 * @param param Parameter that will be sent with this message.
 */
void Communication::send(byte messageId, int param){
  if(messageId > _config->getMessageFilterLevel()){
    sc.print(messageId);
    sc.print(":");
    sc.println(param);
  }
}

/**
 * Sends the message code with the unsigned long parameter. The param will be
 * sent as binary ascii characters and decimal number.
 * @param messageId Id of the message that will be sent.
 * @param param Parameter that will be sent with this message.
 */
void Communication::sendBinary(byte messageId, unsigned long param){
  if(messageId > _config->getMessageFilterLevel()){
    sc.print(messageId);
    sc.print(":");
    sc.print(param, DEC);
    sc.print(";");
    sc.println(param, BIN);
  }
}

/**
 * Sends the message code over the configured serial interface.<br>
 * The message will be filtered according to the configured filter level in the configuration.<br><br>
 * The message will be sent in the following format:<br>
 * <code>202-4:12,323,4399,2292\\n</code><br>
 * <code>[messageId]-[sizeOfParameters]:[parameter 1],[parameter 2],[parameter 3],[parameter n]\\n</code>
 * @param messageId Id of the message that will be sent.
 * @param params[] Parameters that will be sent with this message.
 * @param paramSize Size of params[].
 */
void Communication::send(byte messageId, const long params[], byte paramSize){
  if(messageId > _config->getMessageFilterLevel()){
    sc.print(messageId);
    sc.print("-");
    sc.print(paramSize);
    sc.print(":");
    byte i;
    for(i = 0; i < (paramSize - 1); i++){
      ///@todo: make sure the receiver of the serial message can work with negative parameters
      sc.print(params[i], DEC);
      sc.print(",");
    }
    //the last one is sent without a comma but with a new line
    sc.println(params[i]);
  }
}
/**
 * \overload
 * @param messageId Id of the message that will be sent.
 * @param params[] Parameters that will be sent with this message.
 * @param paramSize Size of params[].
 * @todo Find a better implementation so that we don't have a copy of code.
 */
void Communication::send(byte messageId, const int params[], byte paramSize){
  if(messageId > _config->getMessageFilterLevel()){
    sc.print(messageId);
    sc.print("-");
    sc.print(paramSize);
    sc.print(":");
    byte i;
    for(i = 0; i < (paramSize - 1); i++){
      ///@todo: make sure the receiver of the serial message can work with negative parameters
      sc.print(params[i]);
      sc.print(",");
    }
    //the last one is sent without a comma but with a new line
    sc.println(params[i]);
  }
}

/**
 * Sends a plain string message for debug reasons.
 * 
 * Note: This method should only temporarly be used to test the library.
 * @param message Message to send.
 */
void Communication::sendString(char* message){
  sc.println(message);
}

/**
 * Gets the current configuration from the Configuraton instance and sends it over the serial port.
 */
void Communication::sendCurrentConfiguration(){
  long currentConfiguration[Configuration::SIZEOFDYNAMICCONFIGURATION];
  _config->getCurrentConfiguration(currentConfiguration);
  send(204, currentConfiguration, Configuration::SIZEOFDYNAMICCONFIGURATION);
}

/**
 * Sends the currently free memory over the serial interface with the given placeOfMeasurement
 * value as a identifier from where this method was called.
 * @param placeOfMeasurement Identifier from where this method was called.
 */
void Communication::sendFreeMemory(int placeOfMeasurement){
  int freeMemory = _config->getFreeMemory();
  int params[2];
  params[0] = freeMemory;
  params[1] = placeOfMeasurement;
  send(68, params, 2);
}


























