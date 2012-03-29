using ch.jep.McCommunication;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace prencarTest
{
    
    
    /// <summary>
    ///Dies ist eine Testklasse für "MessageTest" und soll
    ///alle MessageTest Komponententests enthalten.
    ///</summary>
    [TestClass()]
    public class MessageTest
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
        ///Ein Test für "RawMessage"
        ///</summary>
        [TestMethod()]
        public void RawMessageTest_WithoutParameter()
        {
            Message target = new Message();
            string rawInput = "200";
            target.RawMessage = rawInput;
            Assert.AreEqual(target.MessageCode, 200);
            Assert.AreEqual("Calibrating the line sensors...\nPlace the front left sensor on black ground. The calibration starts in 10 seconds.", target.MessageCombined);
            Assert.AreEqual(target.Parameters.Count, 0);
        }

        /// <summary>
        ///Ein Test für "RawMessage"
        ///</summary>
        [TestMethod()]
        public void RawMessageTest_With6Parameter()
        {
            Message target = new Message();
            string rawInput = "53-6:10,10,20,20,20,40";
            target.RawMessage = rawInput;
            Assert.AreEqual(target.MessageCode, 53);
            Assert.AreEqual(@"Changed motor speed of left and right motor:
oldSpeedLeft: 10
changeLeft: 10
newSpeedLeft: 20
oldSpeedRight: 20
changeRight: 20
newSpeedRight: 40", target.MessageCombined);
            Assert.AreEqual(target.Parameters.Count, 6);
        }

        /// <summary>
        ///Ein Test für "RawMessage"
        ///</summary>
        [TestMethod()]
        public void RawMessageTest_WithOneParameter()
        {
            Message target = new Message();
            string rawInput = "55:98.99";
            target.RawMessage = rawInput;
            Assert.AreEqual(target.MessageCode, 55);
            Assert.AreEqual(@"Left front sensor detected white ground (Value: 98.99).", target.MessageCombined);
            Assert.AreEqual(target.Parameters.Count, 1);
            Assert.AreEqual(target.Parameters[0], 98.99);
        }

        /// <summary>
        ///Ein Test für "RawMessage"
        ///</summary>
        [TestMethod()]
        public void RawMessageTest_WithSyntaxError()
        {
            Message target = new Message();
            string rawInput = "This just is a plain message with a : :-)";
            target.RawMessage = rawInput;
            Assert.AreEqual("(combining Error, Raw message:) This just is a plain message with a : :-)", target.MessageCombined);
        }
    }
}
