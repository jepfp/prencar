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

namespace prencar
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialCommunication sc = new SerialCommunication();
        McConfiguration conf = new McConfiguration();

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

            conf.ConfigUpdated += new ConfigUpdatedEventHandler(conf_ConfigUpdated);
        }

        void conf_ConfigUpdated(object sender)
        {
            lblConfigurationVersion.Content = ((McConfiguration)sender).get("_CONFIGURATIONVERSION");
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Prepare data in arrays
            const int N = 180;
            double[] x = new double[N];
            double[] y = new double[N];

            for (int i = 0; i < N; i++)
            {
                x[i] = i;
                y[i] = Math.Sin(i);
            }

            // Create data sources:
            var xDataSource = x.AsXDataSource();
            var yDataSource = y.AsYDataSource();

            CompositeDataSource compositeDataSource = xDataSource.Join(yDataSource);
            // adding graph to plotter
            plotter.AddLineGraph(compositeDataSource,
                Colors.Goldenrod,
                3,
                "Distance");

            // Force evertyhing plotted to be visible
            plotter.FitToView();
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
            }
            tbSerialInput.Text += "\n\n" + message.MessageCombined;
            tbSerialInput.ScrollToEnd();
        }

        private void btnSerialConnectDisconnect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (sc.IsConnected())
                {
                    sc.NewMessage -= workMessage;
                    sc.Disconnect();
                    btnSerialConnectDisconnect.Content = "Connect";
                }
                else
                {
                    sc.Connect(tbComport.Text);
                    btnSerialConnectDisconnect.Content = "Disconnect";

                    //tell the microcontroller to send the current configuration
                    sc.SendCommand("100-0:");

                    sc.NewMessage += new NewMessageEventHandler(workMessage);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString());
            }
        }
        #endregion

        private void btnOpenMcConfigurationView_Click(object sender, RoutedEventArgs e)
        {
            McConfigurationView cv = new McConfigurationView(sc, conf);
            cv.ShowDialog();

            //after closing, tell the MC to send the new configuration again.
            sc.SendCommand("100");
        }

        private void btnSendDebugSerialCommand_Click(object sender, RoutedEventArgs e)
        {
            sc.SendCommand(tbDebugSerialCommand.Text);
            tbSerialInput.Text += "\n\n  (send to mc):" + tbDebugSerialCommand.Text;
            tbSerialInput.ScrollToEnd();
        }

        private void btnCalibrateLineSensors_Click(object sender, RoutedEventArgs e)
        {
            sc.SendCommand("200-0:");
        }

        private void btnStartStoppParcours_Click(object sender, RoutedEventArgs e)
        {
            if (parcoursState == ParcoursState.notStarted)
            {
                sc.SendCommand("300-0:");
                changeParcoursState(ParcoursState.followingFirstLine);
                btnStartStoppParcours.Content = "Stop";
            }
            else
            {
                sc.SendCommand("301-0:");
                changeParcoursState(ParcoursState.notStarted);
                btnStartStoppParcours.Content = "Start";
            }
        }

        private void changeParcoursState(ParcoursState newState)
        {
            parcoursState = newState;
            lbState.SelectedIndex = (int)newState;
        }

    }
}
