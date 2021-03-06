#ifndef Command_h
#define Command_h

#include "Arduino.h"
//#include "Communication.h"

/**
 * \brief A Command object represents a command sent from the host software.
 * 
 * The commands are transmitted as lines over the serial interface. Each command
 * is transmitted on one line (\\n). After a line is ready to be parsed a Command object is created out of it.
 * A command has always to be transmitted in the following format:<br>
 * <code>202-4:12,323,4399,2292\\n</code><br>
 * <code>[messageId]-[sizeOfParameters]:[parameter 1],[parameter 2],[parameter 3],[parameter n]\\n</code>
 *
 * If there arent any parameters the command would look like this:<br>
 * <code>205-0:\\n</code>
 * 
 * <h3>Possible Commands</h3>
 * Below all possible commands are listed.
 * <table>
 * <tr><th>Code</th><th>Name</th><th>Parameters</th><th>Description</th></tr>
 * <tr><td>100</td><td>Send current configuration</td><td>-</td><td>Tell the MC to send the whole configuration over the serial interface.</td></tr>
 * <tr><td>101</td><td>Update configuration</td><td>See Configuration::updateConfiguration()</td><td>Updates the configuration of the microcontroller with the given parameters.</td></tr>
 * <tr><td>200</td><td>Calibrate line sensors</td><td>-</td><td>Starts the calibration of the line sensors.</td></tr>
 * <tr><td>300</td><td>Start parcours</td><td>-</td><td>Starts the parcours</td></tr>
 * <tr><td>301</td><td>Stop parcours</td><td>-</td><td>Stops the parcours and the car performing a fast stop.</td></tr>
 * </table>
 */
class Command
{
private:
  //Communication* _com; ///< The reference to the serial communication class.
  static int const _COMMANDMAXPARAMETERAMOUNT = 20; ///< Defines, how many parameters a command can have.
  int copyBytesUntil(byte* const source, int sourceStart, byte* target, int targetSize, byte stopCharacter);
public:
  Command();
  int commandCode;
  int parameters[_COMMANDMAXPARAMETERAMOUNT];
  int amountOfParameters;
  void parse(byte* commandString);
};

#endif




