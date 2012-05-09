using ch.jep.McCommunication;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace prencarTest
{
    
    
    /// <summary>
    ///Dies ist eine Testklasse für "SerialCommunicationTest" und soll
    ///alle SerialCommunicationTest Komponententests enthalten.
    ///</summary>
    [TestClass()]
    public class SerialCommunicationTest
    {


        private TestContext testContextInstance;

        /// <summary>
        ///Ruft den Testkontext auf, der Informationen
        ///über und Funktionalität für den aktuellen Testlauf bietet, oder legt diesen fest.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Zusätzliche Testattribute
        // 
        //Sie können beim Verfassen Ihrer Tests die folgenden zusätzlichen Attribute verwenden:
        //
        //Mit ClassInitialize führen Sie Code aus, bevor Sie den ersten Test in der Klasse ausführen.
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext)
        //{
        //}
        //
        //Mit ClassCleanup führen Sie Code aus, nachdem alle Tests in einer Klasse ausgeführt wurden.
        //[ClassCleanup()]
        //public static void MyClassCleanup()
        //{
        //}
        //
        //Mit TestInitialize können Sie vor jedem einzelnen Test Code ausführen.
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Mit TestCleanup können Sie nach jedem einzelnen Test Code ausführen.
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///Ein Test für "createMessagesToQueue"
        ///</summary>
        [TestMethod()]
        [DeploymentItem("prencar.exe")]
        public void createMessagesToQueueTest_justOneCharacterAndNewLine()
        {
            SerialCommunication target = new SerialCommunication();
            string receivedData = "a\n";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(1, target.messageQueue.Count);
            Message m = target.messageQueue.Dequeue();
            Assert.AreEqual(m.RawMessage, "a");
        }

        /// <summary>
        ///Ein Test für "createMessagesToQueue"
        ///</summary>
        [TestMethod()]
        [DeploymentItem("prencar.exe")]
        public void createMessagesToQueueTest_justOneCharacter()
        {
            SerialCommunication target = new SerialCommunication();
            string receivedData = "a";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(0, target.messageQueue.Count);
            Assert.AreEqual(target.linePart, "a");

            //again one more character. Still not a new line
            receivedData = "b";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(0, target.messageQueue.Count);
            Assert.AreEqual(target.linePart, "ab");

            //first time were to whole line is present
            //again one more character. Still not a new line
            receivedData = "c\n";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(1, target.messageQueue.Count);
            Assert.AreEqual("abc", target.messageQueue.Dequeue().RawMessage);
            Assert.AreEqual(target.linePart, "");
        }

        /// <summary>
        ///Ein Test für "createMessagesToQueue"
        ///</summary>
        [TestMethod()]
        [DeploymentItem("prencar.exe")]
        public void createMessagesToQueueTest_ThreeMessagesWithoutRests()
        {
            SerialCommunication target = new SerialCommunication();
            string receivedData = "200\n53-6:10,10,20,20,20,40\n55:98.99\n";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(3, target.messageQueue.Count);
            Assert.AreEqual("200", target.messageQueue.Dequeue().RawMessage);
            Assert.AreEqual("53-6:10,10,20,20,20,40", target.messageQueue.Dequeue().RawMessage);
            Assert.AreEqual(98.99, target.messageQueue.Dequeue().Parameters[0]);
            Assert.AreEqual(target.linePart, "");
        }

        /// <summary>
        ///Ein Test für "createMessagesToQueue"
        ///</summary>
        [TestMethod()]
        [DeploymentItem("prencar.exe")]
        public void createMessagesToQueueTest_ThreeMessagesWithOneNotCompleteAtFirst()
        {
            SerialCommunication target = new SerialCommunication();
            string receivedData = "200\n53-6:10,10,20,20,20,40\n55:98.";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(2, target.messageQueue.Count);
            Assert.AreEqual("200", target.messageQueue.Dequeue().RawMessage);
            Assert.AreEqual("53-6:10,10,20,20,20,40", target.messageQueue.Dequeue().RawMessage);
            Assert.AreEqual(target.linePart, "55:98.");

            receivedData = "99\n";
            target.createMessagesToQueue(receivedData);
            Assert.AreEqual(1, target.messageQueue.Count);
            Assert.AreEqual(target.linePart, "");
            Assert.AreEqual(98.99, target.messageQueue.Dequeue().Parameters[0]);
        }
    }
}
