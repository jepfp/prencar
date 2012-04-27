using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ch.jep.McCommunication
{
    public class Message
    {
        private String rawMessage;
        private DateTime creationDate;
        private int messageCode;
        private String messageCombined;
        private List<Double> parameters = new List<double>();

        public Message()
        {
            creationDate = DateTime.Now;
        }

        public Message(String rawMessage)
        {
            creationDate = DateTime.Now;
            this.RawMessage = rawMessage;
        }

        #region Getters / Setters
        public String RawMessage
        {
            get { return rawMessage; }
            set {
                rawMessage = value;

                try
                {
                    //first split by :
                    string[] splitByColon = rawMessage.Split(':');

                    //get the message code
                    string[] splitByDash = splitByColon[0].Split('-');
                    this.messageCode = int.Parse(splitByDash[0]);

                    //get the parameters
                    if (splitByColon.Length > 1)
                    {
                        string[] splitByComma = splitByColon[1].Split(',');
                        foreach (string parameter in splitByComma)
                        {
                            parameters.Add(Double.Parse(parameter));
                        }
                    }

                    //combine the message
                    this.messageCombined = MessageManager.getInstance().getMessage(this.messageCode);
                    for (int i = 0; i < parameters.Count; i++)
                    {
                        string s = this.messageCombined;
                        int percentIndex = s.IndexOf("%%");
                        s = s.Remove(percentIndex, 2);
                        s = s.Insert(percentIndex, parameters[i].ToString());
                        this.messageCombined = s;
                    }
                }
                catch
                {
                    this.messageCombined = "(combining Error, Raw message:) " + this.rawMessage;
                }
            }
        }

        public DateTime CreationDate
        {
            get { return creationDate; }
            set { creationDate = value; }
        }

        public int MessageCode
        {
            get { return messageCode; }
            set { messageCode = value; }
        }

        public String MessageCombined
        {
            get { return messageCombined; }
            set { messageCombined = value; }
        }

        public List<Double> Parameters
        {
            get { return parameters; }
            set { parameters = value; }
        }
        #endregion

    }
}
