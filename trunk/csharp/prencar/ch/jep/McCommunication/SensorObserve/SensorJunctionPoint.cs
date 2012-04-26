using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Research.DynamicDataDisplay;
using Microsoft.Research.DynamicDataDisplay.DataSources;
using System.Windows;

namespace ch.jep.McCommunication.SensorObserve
{
    /// <summary>
    /// Puts together all information that is needed to plot sensor measurement data. The name of the sensor, the message
    /// code sent from the microcontroller and the graph are bundeld in one class so that SensorViewer can plot the data.
    /// </summary>
    public class SensorJunctionPoint
    {
        ObservableDataSource<Point> source = new ObservableDataSource<Point>();

        private int measurementMessageCode;

        private int measurementMessageParameterIndex = 0;

        private LineGraph graph;

        private string name;

        /// <summary>
        /// Code of the message that is sent over the serial interface which contains the sensor measurement value of that sensor
        /// </summary>
        public int MeasurementMessageCode
        {
            get { return measurementMessageCode; }
            set { measurementMessageCode = value; }
        }

        public String Name
        {
            get
            {
                return this.name;
            }
            set
            {
                this.name = value;
            }
        }

        public LineGraph Graph
        {
            get { return graph; }
            set { this.graph = value; }
        }

        public ObservableDataSource<Point> Source
        {
            get { return this.source; }
        }

        /// <summary>
        /// A message can hold more than one parameter. This int defines which parameter from the message shall be taken as the
        /// sensor measurement value (0 based array index).
        /// </summary>
        public int MeasurementMessageParameterIndex
        {
            get { return measurementMessageParameterIndex; }
            set { measurementMessageParameterIndex = value; }
        }

        public SensorJunctionPoint()
        {
            source.SetXYMapping(p => p);
        }

        public SensorJunctionPoint(int measurementMessageCode, String sensorDescription)
            : this()
        {
            this.measurementMessageCode = measurementMessageCode;
            this.Name = sensorDescription;
        }

        public SensorJunctionPoint(int measurementMessageCode, int measurementMessageParameterIndex, String sensorDescription)
            : this(measurementMessageCode, sensorDescription)
        {
            this.measurementMessageParameterIndex = measurementMessageParameterIndex;
        }
    }
}
