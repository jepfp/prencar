using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ch.jep.McCommunication
{
    class MessageManager
    {
        private Dictionary<int, String> messages = new Dictionary<int, string>();

        static MessageManager instance = null;

        public static MessageManager getInstance()
        {
            if (instance == null)
            {
                instance = new MessageManager();
            }
            return instance;
        }

        private MessageManager()
        {

            #region 1-49 Sensormesswerte
            messages.Add(1, @"Results from front line sensors measurement:
leftFrontSensor: %%
rightFrontSensor: %%");
            messages.Add(2, @"Results from line sensors measurement:
leftSensor: %%
rightSensor: %%");
            messages.Add(3, @"Value of _deltasensor: %%");
            messages.Add(4, @"Value of _deltaPWM: %%");
            messages.Add(10, @"Left line sensor (debug): %%");
            messages.Add(11, @"Right line sensor (debug): %%");
            messages.Add(12, @"Left front line sensor (debug): %%");
            messages.Add(13, @"Right front line sensor (debug): %%");
            messages.Add(14, @"Cube approach left bottom sensor (debug): %%");
            messages.Add(15, @"Cube approach left top sensor (debug): %%");
            messages.Add(16, @"Cube approach right bottom sensor (debug): %%");
            messages.Add(17, @"Cube approach right top sensor (debug): %%");
            #endregion

            #region 50-99 Debug
            messages.Add(50, @"");
            messages.Add(51, @"Direction of left motor set to: %%
0 - phaseOut
1 - forwards
2 - backwards
3 - fastStop");
            messages.Add(52, @"Direction of left motor set to: %%
0 - phaseOut
1 - forwards
2 - backwards
3 - fastStop");
            messages.Add(53, @"Changed motor speed of left and right motor:
oldSpeedLeft: %%
changeLeft: %%
newSpeedLeft: %%
oldSpeedRight: %%
changeRight: %%
newSpeedRight: %%");
            messages.Add(54, @"Motor speed has been equalized.
oldSpeedLeft: %%
oldSpeedRight: %%
newAverageSpeed: %%");
            messages.Add(55, @"Left front sensor detected white ground (Value: %%).");
            messages.Add(56, @"Left line sensor detected white ground (Value: %%). Moving right.");
            messages.Add(57, @"Right line sensor detected white ground (Value: %%). Moving left.");
            messages.Add(58, "Motor controlled: speed left: %% (direction: %%), speed right: %% (direction: %%)");
            messages.Add(59, "");
            messages.Add(60, "Perform fast stop.");
            messages.Add(61, "Put command with code %% to spot number %% out of %% spots in readyCommands.");
            messages.Add(62, "Took command with code %% from the list. Spot %% is empty again.");
            messages.Add(63, "Lift cube started...");
            messages.Add(64, "Finishing lift cube by lifting up host...");
            messages.Add(65, "Set hoist position to %%.");
            messages.Add(66, @"Left line sensor detected white ground (Value: %%).");
            messages.Add(67, @"Start driving curve left.");
            messages.Add(68, @"Free memory: %% (place of measurement: %%)");
            #endregion

            #region 100-149 Information
            messages.Add(100, @"Parcours state changed to %%.");
            messages.Add(101, @"Debug delay of %% milliseconds started...");
            messages.Add(102, @"New configuration saved.");
            #endregion

            #region 150-199 Error
            messages.Add(150, "No empty spot found for new command. Discarding command!!");
            messages.Add(151, "The state %% does not exist or is not yet implemented. Highest implemented state at the moment: %%");
            #endregion

            #region 200-255 User Information
            messages.Add(200, @"Calibrating the line sensors...
Place the front left sensor on black ground. The calibration starts in 10 seconds.");
            messages.Add(201, @"Starting the calibration…
50 measurements in 100ms intervals are taken.");
            messages.Add(202, @"Finished.
Place the front left sensor on white ground. The calibration starts in 10 seconds.");
            messages.Add(203, @"Calibration finished. The measured values are:
_calibrationSensorBlackMin: %%
_calibrationSensorBlackMax: %%
_calibrationSensorWhiteMin: %%
_calibrationSensorWhiteMax: %%
oneThirdOfRange: %% ((BlackMin - WhiteMax) / 3)
lineFollowWhiteThreshold: %% (WhiteMax + oneThirdOfRange)");
            messages.Add(204, @"_CONFIGURATIONVERSION:%%
_SERIALSPEED:%%
messageFilterLevel:%%
doJobDelay:%%
lineFollowInitialSpeedLeft:%%
lineFollowInitialSpeedRight:%%
lineFollowInterval:%%
lineFollowKp:%%
lineFollowKd:%%
lineFollowWhiteThreshold:%%
curveSpeedSlowMotor:%%
curveSpeedFastMotor:%%
curveInterval:%%
curveActivateEndSensorOffset:%%
sensorDebugInterval:%%
sensorDebugReadGap:%%");
            messages.Add(205, "Starting parcours at state %%.");
            messages.Add(206, "Debug module activated.");
            messages.Add(207, "Debug module deactivated.");
            #endregion

            //messages.Add(51, @"");
        }

        public string getMessage(int id)
        {
            return messages[id];
        }
    }
}
