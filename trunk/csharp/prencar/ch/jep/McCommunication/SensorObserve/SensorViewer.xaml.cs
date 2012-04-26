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
            /*if (!plotter.Dispatcher.CheckAccess())
            {
                this.Dispatcher.Invoke(new NewMessageEventHandler(sc_NewMessage), new object[] { sender, message });
                return;
            }*/

            foreach (SensorJunctionPoint sjp in sensors)
            {
                if (message.MessageCode == sjp.MeasurementMessageCode)
                {
                    sjp.Source.AppendAsync(Dispatcher, new Point(getMillisSinceStart(), message.Parameters[sjp.MeasurementMessageParameterIndex]));
                }
            }
        }

        public void AddGraph(SensorJunctionPoint sjp){
            sensors.Add(sjp);
            sjp.Graph = plotter.AddLineGraph(sjp.Source, 2, sjp.Name);

        }

        private int getMillisSinceStart()
        {
            TimeSpan ts = new TimeSpan();
            ts = DateTime.Now - initTime;
            return (int)ts.TotalMilliseconds;
        }
    }
}
