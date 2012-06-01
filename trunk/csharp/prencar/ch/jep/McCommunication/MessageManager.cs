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
            messages.Add(18, @"Results from bottom distance sensors measurement (leftBottomSensor: %%, rightBottomSensor: %%)");
            messages.Add(19, @"Results from top distance sensors measurement (leftTopSensor: %%, rightTopSensor: %%)");
            #endregion

            #region 50-99 Debug
            messages.Add(50, @"");
            messages.Add(51, @"Direction of left motor set to: %%
0 - phaseOut
1 - forwards
2 - backwards
3 - fastStop");
            messages.Add(52, @"Direction of right motor set to: %%
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
            messages.Add(64, "Finishing lift cube by lifting up hoist...");
            messages.Add(65, "Set hoist position to %%.");
            messages.Add(66, @"Left line sensor detected white ground (Value: %%). Starting line centering.");
            messages.Add(67, @"Start driving curve left.");
            messages.Add(68, @"Free memory: %% (place of measurement: %%)");
            messages.Add(69, @"End switch has been set to one --> Lift cube.");
            messages.Add(70, @"Changed speed of left motor to %% and right motor to %%.");
            messages.Add(71, @"Line follow process switches after %% milliseconds to reduced base speed (left: %%, right: %%).");
            messages.Add(72, @"Cube not detected (left bottom: %%, right bottom: %%)");
            messages.Add(73, @"Cube detected with both sensors (left bottom: %%, right bottom: %%)");
            messages.Add(74, @"Cube approach: Verification of left bottom sensor failed (bottom: %%, top: %%).");
            messages.Add(75, @"Cube approach: Verification of right bottom sensor failed (bottom: %%, top: %%).");
            messages.Add(76, @"Cube with left sensor detected: (left: %%, right: %%).");
            messages.Add(77, @"Cube with right sensor detected: (left: %%, right: %%).");
            messages.Add(78, @"Cube detected with left sensor. Turning right.");
            messages.Add(79, @"Cube detected with right sensor. Turning left.");
            messages.Add(80, @"Execution of extended move command queue with id %% has finished.");
            messages.Add(81, @"Executing extended move command #%% (0 based index) out of %% commands from command queue #%% for %% milliseconds.");
            messages.Add(82, @"Started execution of extended move command queue #%% with %% commands.");
            messages.Add(83, @"No cube detection is done because turning is still in progress.");
            messages.Add(84, @"Starting line centering process from left.");
            messages.Add(85, @"Starting line centering process from right.");
            messages.Add(86, @"The car is aligned the line. Line Center process has finished.");
            messages.Add(87, @"Car centering: Sensor measured black --> driving straight (left sensor: %%, right sensor: %%, threshold: %%).");
            messages.Add(88, @"Car centering: Driving curve (left sensor: %%, right sensor: %%, threshold: %%).");
            messages.Add(89, @"Car centering: Correction in progress.");
            messages.Add(90, @"Cube approach: Starting 'stop car' action (ext move command).");
            messages.Add(91, @"Cube approach: 'stop car' action (ext move command) has finished after %%ms.");
            messages.Add(92, @"No state change from 'lift cube' to 'moveBackToLine' because extMove command 'stop car' from 'lift cube' is still in progress.");
            messages.Add(93, @"Move back to line: Line has been found (left sensor value: %%, right sensor value: %%).");
            #endregion

            #region 100-149 Information
            messages.Add(100, @"Parcours state changed to %%.");
            messages.Add(101, @"Debug delay of %% milliseconds started...");
            messages.Add(102, @"New configuration saved.");
            messages.Add(103, @"The cube end sensor detected the cube even though the program was not yet in the 'lift cube' state --> switching immediately to that state.");
            #endregion

            #region 150-199 Error
            messages.Add(150, "No empty spot found for new command. Discarding command!!");
            messages.Add(151, "The state %% does not exist or is not yet implemented. Highest implemented state at the moment: %%");
            messages.Add(152, @"Updating the configuration was not successful because the config versions don't match.
The microcontrollers current configuration version is %%.");
            messages.Add(153, "'Move back to line' process can not be started because there is no track back (cube approach recordings which have informations about how the line has been left are empty).");
            messages.Add(154, "'Move back to line' process can not be started because there is no track back. The first cube approach record already says, that the cube is in the center. We don't know for sure in which direction the car has left the line.");
            messages.Add(155, "'Move back to line' process can not be started because the track back is invalid: The first cube apporach record has neither set 'turnedRight' nor 'turnedLeft' to true.");
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
            messages.Add(204, @"CONFIGURATIONVERSION:%%
SERIALSPEED:%%
messageFilterLevel:%%
activateMessageFilter:%% 
doJobDelay:%%
whiteThresholdLineSensors:%%
whiteThresholdFrontSensors:%% 
lineFollowInterval:%%
lineFollowInitialSpeedLeft:%%
lineFollowInitialSpeedRight:%%
lineFollowKp:%%
lineFollowKd:%%
lineFollowReducedSpeedLeft:%%
lineFollowReducedSpeedRight:%%
lineFollowReduceSpeedTimeFirstLine:%%
lineFollowReduceSpeedTimeSecondLine:%%
lineFollowReduceSpeedTimeThirdLineToFinish:%%
lineFollowActivateFrontSensorOffset:%%
lineCenterInterval:%%
lineCenterLineInMiddleDifference:%%
lineCenterStraightSpeed:%%
lineCenterFromLeftMotorLeft:%%
lineCenterFromLeftMotorRight:%%
lineCenterFromRightMotorLeft:%%
lineCenterFromRightMotorRight:%%
lineCenterDriveBackDuration:%%
liftCubeUpPosition:%%
liftCubeDownPosition:%%
liftCubeDownUpDuration:%%
liftCubeStopDuration:%%
liftCubeStopSpeed:%%
curveStopDuration:%%
curveStopSpeed:%%
curveInterval:%%
curveSpeedSlowMotor:%%
curveSpeedFastMotor:%%
curveDriveStraightTime:%%
curveActivateEndSensorOffset:%%
cubeApproachUseTopSensors:%%
cubeApproachLineFollowReduceSpeedTime:%%
cubeApproachLineFollowReducedSpeedLeft:%%
cubeApproachLineFollowReducedSpeedRight:%%
cubeApproachDetectThreshold:%%
cubeApproachInterval:%%
cubeApproachTurnDuration:%%
cubeApproachTurnSpeedSlowMotor:%%
cubeApproachTurnSpeedFastMotor:%%
cubeApproachStraightSpeed:%%
moveBackToLineInterval:%%
moveBackToLine1stTurnDuration:%%
moveBackToLine1stLeftTurnLeftSpeed:%%
moveBackToLine1stLeftTurnRightSpeed:%%
moveBackToLine1stRightTurnLeftSpeed:%%
moveBackToLine1stRightTurnRightSpeed:%%
moveBackToLineStraightSpeed:%%
finishLineDriveOverDuration:%%
sensorDebugInterval:%%
sensorDebugReadGap:%%");
            messages.Add(205, "Starting parcours at state %%.");
            messages.Add(206, "Debug module activated.");
            messages.Add(207, "Debug module deactivated.");
            messages.Add(208, "Value of activateMessageFilter changed to %%.");
            messages.Add(209, @" ____|~\_
[____|_|-] Finished parcours after %% milliseconds.
 (_)   (_)");
            #endregion

            //messages.Add(51, @"");
        }

        public string getMessage(int id)
        {
            return messages[id];
        }
    }
}
