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
leftFrontSensor: %%0%%
rightFrontSensor: %%1%%");
            messages.Add(2, @"Results from line sensors measurement:
leftSensor: %%0%%
rightSensor: %%1%%");
            messages.Add(3, @"Value of _deltasensor: %%0%%");
            #endregion

            #region 50-99 Debug
            messages.Add(50, @"");
            messages.Add(51, @"Direction of left motor set to: %%0%%
0 - phaseOut
1 - forwards
2 - backwards
3 - fastStop");
            messages.Add(52, @"Direction of left motor set to: %%0%%
0 - phaseOut
1 - forwards
2 - backwards
3 - fastStop");
            messages.Add(53, @"Changed motor speed of left and right motor:
oldSpeedLeft: %%0%%
changeLeft: %%1%%
newSpeedLeft: %%2%%
oldSpeedRight: %%3%%
changeRight: %%4%%
newSpeedRight: %%5%%");
            messages.Add(54, @"Motor speed has been equalized.
oldSpeedLeft: %%0%%
oldSpeedRight: %%1%%
newAverageSpeed: %%2%%");
            messages.Add(55, @"Left front sensor detected white ground (Value: %%0%%).");
            messages.Add(56, @"Left line sensor detected white ground (Value: %%0%%). Moving right.");
            messages.Add(57, @"Right line sensor detected white ground (Value: %%0%%). Moving left.");
            messages.Add(58, "Motor controlled: speed left: %%0%%, direction left: %%1%%, speed right: %%2%%, direction right: %%3%%");
            messages.Add(59, "");
            messages.Add(60, "Perform fast stop.");
            messages.Add(61, "Put command with code %%0%% to spot number %%1%% out of %%2%% spots in readyCommands.");
            messages.Add(62, "Took command with code %%0%% from the list. Spot %%1%% is empty again.");
            #endregion

            #region 100-149 Information
            messages.Add(100, @"Parcours state changed to %%0%%.");
            #endregion

            #region 150-199 Error
            messages.Add(150, "No empty spot found for new command. Discarding command!!");
            #endregion

            #region 200-255 User Information
            messages.Add(200, @"Calibrating the line sensors...
Place the front left sensor on black ground. The calibration starts in 10 seconds.");
            messages.Add(201, @"Starting the calibration…
50 measurements in 100ms intervals are taken.");
            messages.Add(202, @"Finished.
Place the front left sensor on white ground. The calibration starts in 10 seconds.");
            messages.Add(203, @"Calibration finished. The measured values are:
_calibrationSensorBlackMin: %%0%%
_calibrationSensorBlackMax: %%1%%
_calibrationSensorWhiteMin: %%2%%
_calibrationSensorWhiteMax: %%3%%
oneThirdOfRange: %%4%% ((BlackMin - WhiteMax) / 3)
lineFollowWhiteThreshold: %%5%% (WhiteMax + oneThirdOfRange)");
            messages.Add(204, @"_CONFIGURATIONVERSION:%%0%%
_SERIALSPEED:%%1%%
messageFilterLevel:%%2%%
lineFollowInitialSpeedLeft:%%3%%
lineFollowInitialSpeedRight:%%4%%
lineCorrectionDuration:%%5%%
lineFollowKp:%%6%%
lineFollowKd:%%7%%
lineFollowWhiteThreshold:%%8%%");
            #endregion

            //messages.Add(51, @"");
        }

        public string getMessage(int id)
        {
            return messages[id];
        }
    }
}
