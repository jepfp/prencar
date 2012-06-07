using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Research.DynamicDataDisplay;
using Microsoft.Research.DynamicDataDisplay.DataSources;
using Microsoft.Research.DynamicDataDisplay.PointMarkers;
using ch.jep.McCommunication;
using ch.hslu.prencar;
using ch.jep.McCommunication.SensorObserve;
using ch.hslu.prencar.Properties;
using System.Windows.Threading;

namespace prencar
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Commands
        public static RoutedCommand OpenConfiguration = new RoutedCommand();
        public static RoutedCommand OpenLiveControl = new RoutedCommand();
        public static RoutedCommand OpenSensorDebug = new RoutedCommand();
        #endregion
        SerialCommunication sc = new SerialCommunication();
        McConfiguration conf = new McConfiguration();
        DebugOutputHandler debug;
        private DispatcherTimer stopwatch;
        McConfigurationView configurationView = null;
        List<Point> measuredVoltageLevels = new List<Point>();
        private DateTime initTime = DateTime.Now;

        enum ParcoursState
        {
            notStarted,
            followingFirstLine,
            firstTurn,
            followingSecondLine,
            secondTurn,
            followingThirdLineToCube,
            cubeApproach,
            liftCube,
            moveBackToLine,
            followingThirdLineToFinish,
            finished
        };

        ParcoursState parcoursState = ParcoursState.notStarted;

        public MainWindow()
        {
            InitializeComponent();

            OpenConfiguration.InputGestures.Add(new KeyGesture(Key.Enter, ModifierKeys.Alt));
            OpenLiveControl.InputGestures.Add(new KeyGesture(Key.L, ModifierKeys.Control));
            OpenSensorDebug.InputGestures.Add(new KeyGesture(Key.D, ModifierKeys.Control));

            debug = new DebugOutputHandler(Settings.Default.DebugFilesPath);

            conf.ConfigUpdated += new ConfigUpdatedEventHandler(conf_ConfigUpdated);
        }

        #region Communication
        private void workMessage(object sender, Message message)
        {
            if (!tbSerialInput.Dispatcher.CheckAccess())
            {
                this.Dispatcher.Invoke(new NewMessageEventHandler(workMessage), new object[] { sender, message });
                return;
            }
            //work out special messages
            if (message.MessageCode == 204)
            {
                conf.parseFileConfiguration(message.MessageCombined);
                conf.Title = "Current Configuration on Car";
                if (configurationView != null) configurationView.RefreshConfigurationList();
            }
            else if (message.MessageCode == 104)
            {
                measuredVoltageLevels.Add(new Point((double)getMillisSinceStart() / (1000*60), message.Parameters[0]));
                lblAccuVoltageLevel.Content = message.Parameters[0].ToString();
            }
            else if (message.MessageCode == 152)
            {
                if (configurationView != null) configurationView.DisplayConfigUpdateError(message);
            }
            else if (message.MessageCode == 100)
            {
                remoteChangedParcoursState((ParcoursState)message.Parameters[0]);
            }
            else if (message.MessageCode == 208)
            {
                if (message.Parameters[0] == 1)
                {
                    btnDisableMessageFilter.IsChecked = false;
                }
                else
                {
                    btnDisableMessageFilter.IsChecked = true;
                }
                
            }
            else if (message.MessageCode == 209)
            {
                lblStopwatchDescription.Content = "Finished:";
                TimeSpan ts = new TimeSpan(0, 0, 0, 0, (int)message.Parameters[0]);
                lblStopwatch.Content = ts.ToString(@"mm\:ss\.fff");

            }
            appendDebugText(message.MessageCombined);
        }

        private void appendDebugText(string p)
        {
            if (btnShowDebugOutput.IsChecked == true)
            {
                int substringStartindex = tbSerialInput.Text.Length - 1000;
                if (substringStartindex < 0) substringStartindex = 0;
                tbSerialInput.Text = (tbSerialInput.Text + "\n\n" + p).Substring(substringStartindex);
                tbSerialInput.ScrollToEnd();
            }
            debug.Append(p);
        }

        private void btnSerialConnectDisconnect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (sc.IsConnected())
                {
                    sc.NewMessage -= workMessage;
                    sc.Disconnect();
                    btnSerialConnectDisconnect.Content = "_Connect";
                }
                else
                {
                    sc.Connect(tbComport.Text);
                    btnSerialConnectDisconnect.Content = "_Disconnect";

                    //tell the microcontroller to send the current configuration
                    sendCommand("100-0:");

                    sc.NewMessage += new NewMessageEventHandler(workMessage);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString());
            }
        }
        #endregion

        #region GUI Events (Buttons etc.)
        private void ExecutedOpenConfigurationCommand(object sender, ExecutedRoutedEventArgs e)
        {
            configurationView = new McConfigurationView(sc, conf);
            configurationView.ShowDialog();
            configurationView = null;
        }

        private void ExecutedOpenSensorDebugCommand(object sender, ExecutedRoutedEventArgs e)
        {
            List<SensorJunctionPoint> sensorConfig = new List<SensorJunctionPoint>();

            sensorConfig.Add(new SensorJunctionPoint(10, "left line", Brushes.MidnightBlue));
            sensorConfig.Add(new SensorJunctionPoint(11, "right line", Brushes.SeaGreen));
            sensorConfig.Add(new SensorJunctionPoint(12, "left front line", Brushes.PowderBlue));
            sensorConfig.Add(new SensorJunctionPoint(13, "right front line", Brushes.Lime));
            sensorConfig.Add(new SensorJunctionPoint(14, "left bottom distance", Brushes.Orange));
            sensorConfig.Add(new SensorJunctionPoint(15, "left top distance", Brushes.Red));
            sensorConfig.Add(new SensorJunctionPoint(16, "right bottom distance", Brushes.OrangeRed));
            sensorConfig.Add(new SensorJunctionPoint(17, "right top distance", Brushes.DarkRed));

            sendCommand("201-1:1");
            SensorViewer sv = new SensorViewer(sc, sensorConfig);
            sv.ShowDialog();
            sendCommand("201-1:0");
        }

        private void ExecutedOpenLiveControlCommand(object sender, ExecutedRoutedEventArgs e)
        {
            LiveControl lc = new LiveControl(sc);
            lc.ShowDialog();
        }

        void conf_ConfigUpdated(object sender)
        {
            lblConfigurationVersion.Content = ((McConfiguration)sender).get("_CONFIGURATIONVERSION");
        }

        private void btnSendDebugSerialCommand_Click(object sender, RoutedEventArgs e)
        {
            sendCommand(tbDebugSerialCommand.Text);
        }

        private void btnStartStoppParcours_Click(object sender, RoutedEventArgs e)
        {
            if (parcoursState == ParcoursState.notStarted || parcoursState == ParcoursState.finished)
            {
                if (lbState.SelectedIndex == -1)
                {
                    MessageBox.Show("Fehler, kein Start-Status in der List-Box selektiert.");
                    return;
                }
                else if (((ListBoxItem)lbState.SelectedItem).Content.ToString() == "Finished")
                {
                    lbState.SelectedIndex = 0;
                }
                else if (lbState.SelectedIndex != 0)
                {
                    MessageBox.Show("The parcours will be started with the state " + ((ListBoxItem)lbState.SelectedItem).Content + ".");
                }
                newSession();
                startStopwatch();
                sendCommand("300-1:" + lbState.SelectedIndex);
            }
            else
            {
                sendCommand("301-0:");
            }
            btnStartStoppParcours.IsEnabled = false;
            btnStartStoppParcours.Content = "wait for mc...";
        }

        private void btnOpenLogfile_Click(object sender, RoutedEventArgs e)
        {
            String path = debug.LastAppendPath;
            if (path == "")
            {
                MessageBox.Show("Nothing has been logged yet.", "No Logfile", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            try
            {
                debug.FlushLog();
                System.Diagnostics.Process.Start(path);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Opening logfile failed (path: " + path + ")\n" + ex.Message.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void btnDisableMessageFilter_Click(object sender, RoutedEventArgs e)
        {
            if (btnDisableMessageFilter.IsChecked == true)
            {
                sc.SendCommand("102-1:0");
            }
            else
            {
                sc.SendCommand("102-1:1");
            }
        }

        private void btnOpenSensorViewer_Click(object sender, RoutedEventArgs e)
        {
            List<SensorJunctionPoint> sensorConfig = new List<SensorJunctionPoint>();

            sensorConfig.Add(new SensorJunctionPoint(2, "left line sensor"));
            sensorConfig.Add(new SensorJunctionPoint(2, 1, "right line sensor"));
            sensorConfig.Add(new SensorJunctionPoint(1, "left front line sensor"));
            sensorConfig.Add(new SensorJunctionPoint(1, 1, "right front line sensor"));
            sensorConfig.Add(new SensorJunctionPoint(3, "delta sensor"));
            sensorConfig.Add(new SensorJunctionPoint(4, "delta PWM"));

            SensorViewer sv = new SensorViewer(sc, sensorConfig);
            sv.Show();
        }

        private void lblAccuVoltageLevel_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            showAccuGraph();
        }

        private void btnUpdateAccuVoltageLevel_Click(object sender, RoutedEventArgs e)
        {
            sc.SendCommand("103");
        }

        private void btnShowAccuGraph_Click(object sender, RoutedEventArgs e)
        {
            showAccuGraph();
        }

        private void showAccuGraph()
        {
            List<SensorJunctionPoint> sensorConfig = new List<SensorJunctionPoint>();

            SensorJunctionPoint sjp = new SensorJunctionPoint(104, "Accumulator voltage level");
            sensorConfig.Add(sjp);
            sjp.AddPointsToSource(measuredVoltageLevels);

            SensorViewer sv = new SensorViewer(sc, sensorConfig);
            sv.Show();
            sv.Title = "Accumulator Voltage Level Graph Since " + initTime.TimeOfDay + "in decimal minutes.";
        }
        #endregion

        #region Parcours state machine
        private void remoteChangedParcoursState(ParcoursState newState)
        {
            parcoursState = newState;
            lbState.SelectedIndex = (int)newState;

            btnStartStoppParcours.IsEnabled = true;
            if (parcoursState == ParcoursState.notStarted || parcoursState == ParcoursState.finished)
            {
                btnStartStoppParcours.Content = "_Start";
                if (this.stopwatch != null) this.stopwatch.Stop();
            }
            else
            {
                btnStartStoppParcours.Content = "_Stop";
            }
        }

        private void startStopwatch()
        {
            if (this.stopwatch != null)
            {
                this.stopwatch.Stop();
            }

            TimeSpan start = new TimeSpan(DateTime.Now.Ticks);
            this.stopwatch = new DispatcherTimer(new TimeSpan(0, 0, 0, 1), DispatcherPriority.Normal, delegate
            {
                TimeSpan duration = (new TimeSpan(DateTime.Now.Ticks)).Subtract((TimeSpan)stopwatch.Tag);
                this.lblStopwatch.Content = duration.ToString(@"mm\:ss");
            }, this.Dispatcher);
            stopwatch.Tag = start;
            this.lblStopwatch.Content = "00:00";
            this.lblStopwatchDescription.Content = "Stopwatch:";
        }
        #endregion

        private void sendCommand(String cmd)
        {
            sc.SendCommand(cmd);
            appendDebugText("  (send to mc):" + cmd);
        }

        /// <summary>
        /// Deletes the text in the debug textbox and starts a new
        /// session which means that a new logfile will be created.
        /// </summary>
        private void newSession()
        {
            debug.NewSession();
            tbSerialInput.Text = "";
            debug.Append("New log session started...");
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            ch.hslu.prencar.Properties.Settings.Default.Save();
        }

        private int getMillisSinceStart()
        {
            TimeSpan ts = new TimeSpan();
            ts = DateTime.Now - initTime;
            return (int)ts.TotalMilliseconds;
        }
    }
}
