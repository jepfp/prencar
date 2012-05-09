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

        //public Dictionary<String, String> settings = new Dictionary<String, string>();
        public List<ConfigProperty> settings = new List<ConfigProperty>();
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
        public void parseFileConfiguration(String configString)
        {
            settings = new List<ConfigProperty>();

            String[] lines = configString.Replace("\r\n", "\n").Split('\n');
            foreach (String aLine in lines)
            {
                String[] propertyAndValue = aLine.Split(':');
                settings.Add(new ConfigProperty(propertyAndValue[0], propertyAndValue[1]));
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
            foreach (ConfigProperty p in settings)
            {
                if (p.Key == key)
                {
                    return p.Value;
                }
            }
            return "";
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

        public String GetMcConfigurationDump()
        {
            String dump = "";
            foreach (ConfigProperty cf in settings)
            {
                dump += cf.Value + ",";
            }
            dump = dump.Substring(0, dump.Length - 1);
            return dump;
        }

        public String GetFileConfigurationDump()
        {
            String dump = "";
            foreach (ConfigProperty cf in settings)
            {
                dump += cf.Key + ":" + cf.Value + "\r\n";
            }
            dump = dump.Substring(0, dump.Length - 2);
            return dump;
        }
    }
}
