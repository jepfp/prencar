﻿using System;
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
            #endregion

            #region 100-149 Information
            messages.Add(100, @"Parcours state changed to %%.");
            messages.Add(101, @"Debug delay of %% milliseconds started...");
            messages.Add(102, @"New configuration saved.");
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
lineFollowWhiteThreshold:%%");
            #endregion

            //messages.Add(51, @"");
        }

        public string getMessage(int id)
        {
            return messages[id];
        }
    }
}
