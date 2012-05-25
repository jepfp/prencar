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
 * <tr><td>102</td><td>Activate / Deactivate "activateMessageFilter"</td><td>1 to activate the filter; 0 to deactivate it</td><td>Defines if the messageFilterLevel shall be taken into account during sending a message over the serial communication interface. See Configuration::activateMessageFilter</td></tr>
 * <tr><td>201</td><td>Set state of SensorDebug module</td><td>1 --> set module on</td><td>If enabled (parameter[0] = 1) the sensor debug module sends different sensor measurements in regular intervals.</td></tr>
 * <tr><td>300</td><td>Start parcours</td><td>State to start from as int number (first state = 0). @see TParcoursState</td><td>Starts the parcours</td></tr>
 * <tr><td>301</td><td>Stop parcours</td><td>-</td><td>Stops the parcours and the car performing a fast stop.</td></tr>
 * <tr><td>400</td><td>Control car</td><td>speedLeft, direction Left, speedRight, direction Right</td><td>Speed between 0-255; direction: 0 - phaseOut, 1 - forwards, 2 - backwards, 3 - fastStop</td></tr>
 * <tr><td>401</td><td>Control hoist</td><td>hoist position (pwm value between 0-255)</td>This can control the servo directly and should only be used for debug reasons (can destroy the servo).<td></td></tr>
 * <tr><td>402</td><td>Move hoist down or up</td><td>1 = move hoist down; 0 = move hoist up</td><td>Sets the hoist to the configured up / down position to crab the cube.</td></tr>
 * </table>
 */
class Command
{
private:
  //Communication* _com; ///< The reference to the serial communication class.
  static int const _COMMANDMAXPARAMETERAMOUNT = 50; ///< Defines, how many parameters a command can have.
  int copyBytesUntil(byte* const source, int sourceStart, byte* target, int targetSize, byte stopCharacter);
public:
  Command();
  int commandCode;
  int parameters[_COMMANDMAXPARAMETERAMOUNT];
  int amountOfParameters;
  void parse(byte* commandString);
};

#endif





