#include "Command.h"

/**
 * Creates a new command object.
 */
Command::Command(){
//  _com = Communication::getInstance();
  commandCode = 0;
}

/**
 * Parses a line and saves the content to the membervariables
 * @param commandString The start position of the command string.
 */
void Command::parse(byte* commandString){
  int commandIndex = 0;
  int tempStringSize = 6;  //6 which includes the biggest int char and a \0 character
  byte tempString[tempStringSize];
  commandIndex = copyBytesUntil(commandString, commandIndex, tempString, tempStringSize, '-');
  commandCode = atoi((char*)tempString);
  commandIndex++; //plus the "-" character
  commandIndex += copyBytesUntil(commandString, commandIndex, tempString, tempStringSize, ':');
  amountOfParameters = atoi((char*)tempString);
  //check if that's not to many parameters
  if(amountOfParameters > _COMMANDMAXPARAMETERAMOUNT){
    //this is an error and should not happen. However if it is the case, make sure no data will be overwritten accidently.
    return;
  }
  commandIndex++; //plus the ":" character
  //get all the parameters
  int i;
  for(i = 0; i < amountOfParameters - 1; i++){
    commandIndex += copyBytesUntil(commandString, commandIndex, tempString, tempStringSize, ',');
    parameters[i] = atoi((char*)tempString);
    commandIndex++; //plus the "," character
  }
  if(amountOfParameters > 0){
    commandIndex += copyBytesUntil(commandString, commandIndex, tempString, tempStringSize, '\n');
    parameters[i] = atoi((char*)tempString);
  }
  //_com->send(commandCode, parameters, amountOfParameters);
}

/*
 * Copies characters from the source to the target until the source character matches the stopCharacter or
 * the size of the target -1 (-1 because of the \\0 character) is reached.
 * @param source Source string from which the data is taken.
 * @param sourceStart From which character the copy process shall be started.
 * @param target Where the data shall be copied to.
 * @param stopCharacter Character that will stop the copy process.
 * @return How many bytes were copied (without the \\0 that will be added always at the end).
 */
int Command::copyBytesUntil(byte* const source, int sourceStart, byte* target, int targetSize, byte stopCharacter){
  //iterate through the characters. -2 because of the \0 character for that we will need some space as well at the end.
  int i;
  for(i = 0; i < targetSize - 1; i++){
    if(source[sourceStart + i] == stopCharacter){
      break;
    }
    target[i] = source[sourceStart + i];
  }
  target[i] = '\0';
  return i;
}

