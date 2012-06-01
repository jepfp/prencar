using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;

namespace ch.jep.McCommunication
{
    public delegate void NewMessageEventHandler(object sender, Message message);

    public class SerialCommunication
    {
        public event NewMessageEventHandler NewMessage;


        SerialPort sp;
        internal Queue<Message> messageQueue = new Queue<Message>();
        internal string linePart = "";

        private void dataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string received = null;
            received = sp.ReadExisting();

            createMessagesToQueue(received);

            while(messageQueue.Count > 0){
                OnNewMessage(messageQueue.Dequeue());
            }
        }

        internal void createMessagesToQueue(string receivedData)
        {
            receivedData = linePart + receivedData;

            //if the last line has not yet been transmitted fully, put it to linePart
            if (!receivedData.EndsWith("\n"))
            {
                linePart = receivedData.Substring(receivedData.LastIndexOf('\n') + 1);

                if (receivedData.LastIndexOf('\n') == -1)
                {
                    //if there is no \n, finish here, nothing needs to be added to the queue
                    return;
                }
                else
                {
                    receivedData = receivedData.Remove(receivedData.LastIndexOf('\n'));
                }
            }
            else
            {
                linePart = "";
            }

            if(receivedData.EndsWith("\n")){
                receivedData = receivedData.Remove(receivedData.LastIndexOf('\n'));
            }

            string[] lines = receivedData.Split('\n');
            foreach (string line in lines)
            {
                Message m = new Message(line);
                messageQueue.Enqueue(m);
            }
        }

        public void Connect(string comport)
        {
            try
            {
                if (sp == null)
                {
                    sp = new SerialPort(comport);
                    sp.NewLine = "\n";
                    sp.DataReceived += new SerialDataReceivedEventHandler(dataReceived);
                }

                if (!sp.IsOpen)
                {
                    sp.Open();
                }
            }
            catch (Exception ex)
            {
                sp = null;
                throw ex;
            }
        }

        public void Disconnect()
        {
            if (sp.IsOpen)
            {
                sp.Close();
                sp = null;
            }
        }

        protected virtual void OnNewMessage(Message message)
        {
            if (NewMessage != null)
            {
                NewMessage(this, message);
            }
        }

        public bool IsConnected()
        {
            if (sp != null)
            {
                return sp.IsOpen;
            }
            return false;
        }

        public void SendCommand(String command)
        {
            if (command.Length > 256)
            {
                throw new Exception("To many characters. Check the incoming data buffer of the board!!");
            }
            try
            {
                Console.WriteLine("Amount of characters that will be sent: " + command.Length.ToString());
                Console.WriteLine("Command to send:" + command.ToString());
                sp.WriteLine(command);
            }
            catch (Exception ex)
            {
                Console.WriteLine("SerialCommunication: Sending failed. " + ex.Message.ToString());
            }
        }

    }
}
