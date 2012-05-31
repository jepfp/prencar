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
using System.Windows.Shapes;
using Microsoft.Research.DynamicDataDisplay;
using System.Windows.Controls.Primitives;
using System.Data;
using System.Globalization;

namespace ch.jep.McCommunication.SensorObserve
{
    /// <summary>
    /// Interaktionslogik für SensorViewer.xaml
    /// </summary>
    public partial class SensorViewer : Window
    {
        private List<SensorJunctionPoint> sensors = new List<SensorJunctionPoint>();
        private DateTime initTime;

        public SensorViewer(SerialCommunication sc)
        {
            InitializeComponent();

            initTime = DateTime.Now;
            sc.NewMessage += new NewMessageEventHandler(sc_NewMessage);

            plotter.Legend.LegendLeft = 20;

            dgSensors.ItemsSource = sensors;
            dgSensors.LoadingRow += new EventHandler<DataGridRowEventArgs>(dgSensors_LoadingRow);
        }

        public SensorViewer(SerialCommunication sc, List<SensorJunctionPoint> sensors) : this(sc)
        {
            sensors.ForEach(delegate(SensorJunctionPoint p)
            {
                this.AddGraph(p);
            });
        }

        void sc_NewMessage(object sender, Message message)
        {
            if (!plotter.Dispatcher.CheckAccess())
            {
                this.Dispatcher.Invoke(new NewMessageEventHandler(sc_NewMessage), new object[] { sender, message });
                return;
            }

            foreach (SensorJunctionPoint sjp in sensors)
            {
                if (message.MessageCode == sjp.MeasurementMessageCode)
                {
                    sjp.Source.AppendAsync(Dispatcher, new Point(getMillisSinceStart(), message.Parameters[sjp.MeasurementMessageParameterIndex]));
                }
            }
        }

        public void AddGraph(SensorJunctionPoint sjp){
            
            if (!sensors.Contains(sjp))
            {
                sensors.Add(sjp);
                //dgSensors.Rows[0].Cells[0].Style.BackColor = sjp.Brush;
                sjp.ActiveStateChanged += new ActiveStateChangedEventHandler(sjp_ActiveStateChanged);
            }

            if (sjp.Brush != null)
            {
                sjp.Graph = plotter.AddLineGraph(sjp.Source, new Pen(sjp.Brush, 2), new PenDescription(sjp.Name));
            }
            else
            {
                sjp.Graph = plotter.AddLineGraph(sjp.Source, 2, sjp.Name);
            }
        }

        void dgSensors_LoadingRow(object sender, DataGridRowEventArgs e)
        {
            DataGridCellInfo a = new DataGridCellInfo(e.Row, dgSensors.Columns[0]);
            dgSensors.CurrentCell = a;
            e.Row.Background = ((SensorJunctionPoint)e.Row.Item).Brush;
        }

        void sjp_ActiveStateChanged(object sender, bool newState, LineGraph lg)
        {
            if (newState)
            {
                SensorJunctionPoint sjp = (SensorJunctionPoint)sender;
                AddGraph(sjp);
            }
            else
            {
                plotter.Children.Remove(lg);
            }
        }

        private int getMillisSinceStart()
        {
            TimeSpan ts = new TimeSpan();
            ts = DateTime.Now - initTime;
            return (int)ts.TotalMilliseconds;
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            //Schliessen
            if (e.Key == Key.Escape)
            {
                Close();
            }
        }
    }  
}