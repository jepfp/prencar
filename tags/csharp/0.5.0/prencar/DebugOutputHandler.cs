using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ch.hslu.prencar
{
    class DebugOutputHandler
    {
        String persistencePath;
        String lastAppendPath = "";
        const int LOGBUFFERCAPACITY = 10000;
        StringBuilder logBuffer = new StringBuilder(LOGBUFFERCAPACITY, LOGBUFFERCAPACITY);

        /// <summary>
        /// Crates a new handler.
        /// </summary>
        /// <param name="persistencePath">Path where the output shall be logged.</param>
        public DebugOutputHandler(String persistencePath)
        {
            this.persistencePath = persistencePath;
            if(!Directory.Exists(persistencePath)){
                Console.WriteLine("Created path " + persistencePath);
                Directory.CreateDirectory(persistencePath);
            }

            NewSession();
        }

        /// <summary>
        /// Destructor.
        /// </summary>
        ~DebugOutputHandler()
        {
            CloseSession();
        }

        /// <summary>
        /// Holds the path to the file where the last execution of append() was saved to.
        /// </summary>
        public String LastAppendPath
        {
            get { return Path.GetFullPath(lastAppendPath); }
        }

        /// <summary>
        /// Creates a new logfile session. New entries will be saved in a new file.
        /// </summary>
        public void NewSession()
        {
            FlushLog();
            lastAppendPath = createAndGetPathWithDateAndTime();
        }

        public void Append(string text){
            text = "\r\n\r\n" + text;
            if (logBuffer.Length + text.Length >= logBuffer.MaxCapacity)
            {
                Console.WriteLine("Flushed, because buffer was full (" + LOGBUFFERCAPACITY.ToString() + " characters).");
                FlushLog();
            }
            logBuffer.Append(text);
        }

        /// <summary>
        /// Makes sure, that all data is saved in the log file.
        /// </summary>
        public void FlushLog()
        {
            if (logBuffer.Length == 0) return;

            try
            {
                Console.WriteLine("Flushing " + logBuffer.Length.ToString() + " characters...");
                File.AppendAllText(lastAppendPath, logBuffer.ToString());
                logBuffer.Clear();
                Console.WriteLine("Flushing done");
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("Append text to logfile failed:\n" + ex.Message.ToString());
            }
        }

        /// <summary>
        /// Closes the session by flushing the log buffer.
        /// </summary>
        public void CloseSession()
        {
            FlushLog();
        }

        /// <summary>
        /// Combines the full path to a file based on the given persistencePath with an additional folder
        /// for the current day and a file name with hour, minute, second. Extension: .log
        /// The method also makes sure, that this path gets created and is available.
        /// </summary>
        /// <returns>Created path</returns>
        private string createAndGetPathWithDateAndTime()
        {
            String completePath = persistencePath;
            completePath = Path.Combine(completePath, DateTime.Now.ToString("yyyy_MM_dd"));
            if (!Directory.Exists(completePath))
            {
                Console.WriteLine("Created path " + completePath);
                Directory.CreateDirectory(completePath);
            }
            completePath = Path.Combine(completePath, DateTime.Now.ToString("hh_mm_ss") + ".log");
            return completePath;
        }

        /// <summary>
        /// Returns the whole content of the current / latest logfile.
        /// Calling this method will cause a flush!
        /// </summary>
        /// <returns>Log content</returns>
        public string getCurrentContent()
        {
            FlushLog();
            try
            {
                return File.ReadAllText(lastAppendPath);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("Reading the logfile failed:\n" + ex.Message.ToString());
                return "";
            }
        }
    }
}
