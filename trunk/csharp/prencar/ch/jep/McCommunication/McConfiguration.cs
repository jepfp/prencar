using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ch.jep.McCommunication
{
    public delegate void ConfigUpdatedEventHandler(object sender);

    public class McConfiguration
    {
        public event ConfigUpdatedEventHandler ConfigUpdated;

        public Dictionary<String, String> settings = new Dictionary<String, string>();
        private String title = "invalid configuration";

        public String Title
        {
            get { return title; }
            set { title = value; }
        }

        /// <summary>
        /// Parses the given configuration and makes it available in this object.
        /// </summary>
        /// <param name="configString"></param>
        public void parseConfiguration(String configString)
        {
            settings = new Dictionary<string, string>();

            String[] lines = configString.Replace("\r\n", "\n").Split('\n');
            foreach (String aLine in lines)
            {
                String[] propertyAndValue = aLine.Split(':');
                settings.Add(propertyAndValue[0], propertyAndValue[1]);
            }

            OnConfigUpdate();
        }

        /// <summary>
        /// Gets the value of a setting for the given key.
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public String get(String key)
        {
            return settings[key];
        }

        protected virtual void OnConfigUpdate()
        {
            if (ConfigUpdated != null)
            {
                ConfigUpdated(this);
            }
        }

        public override string ToString()
        {
            return title;
        }
    }
}
