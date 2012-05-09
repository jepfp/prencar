using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.IO;

namespace ch.jep.McCommunication
{
    public class ConfigProperty
    {
        string key;

        public string Key
        {
            get { return key; }
            set { key = value; }
        }
        string value;

        public string Value
        {
            get { return this.value; }
            set { this.value = value; }
        }

        public ConfigProperty(string key, string value)
        {
            this.key = key;
            this.value = value;
        }
    }

}
