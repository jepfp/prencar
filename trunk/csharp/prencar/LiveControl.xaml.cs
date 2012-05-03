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
using ch.jep.McCommunication;

namespace ch.hslu.prencar
{
    enum MotorDirection
    {
        phaseOut, ///< Phase out the motor. The power is taken away from the motor but the car can roll a couple of centimers still.  
        forward, ///< The motor rotates forward.
        backwards, ///< The motor rotates backward.
        fastStop ///< The motor is stopped immediately. This is done by shortly giving power in the other direction.
    };

    /// <summary>
    /// Interaktionslogik für LiveControl.xaml
    /// </summary>
    public partial class LiveControl : Window
    {

        SerialCommunication sc;

        string lastCommand = "";

        /*
         * the variable direction will hold a combination off direction keys.
         * Remember that the car behaves like a panzer!
         * The following combinations are possible (8 diffrent):
         *  - e:    forward     turns right forwardRight
         *  - ei:   forward     -           forward
         *  - i:    forward     turns left  forwardLeft
         *  - f:    backward    turns right backwardRight
         *  - fj:   backward    -           backward
         *  - j:    backward    turns left  backwardLeft
         *  - ej:   rotate      right       roatateRight
         *  - fi:   rotate      left        rotateLeft
         */
        string direction = "";
        char forwardToRightKey = 'e'; //variable stands for the LEFT axis which makes the car turning (TO) RIGHT; forwards
        char forwardToLeftKey = 'i'; //variable stands for the RIGHT axis which makes the car turning (TO) LEFT; forwards 
        char backwardToRightKey = 'f'; //variable stands for the LEFT axis which makes the car turning (TO) RIGHT; backwards
        char backwardToLeftKey = 'j'; //variable stands for the RIGHT axis which makes the car turning (TO) LEFT; backwards

        //image-references. if needed, the picture will be loaded
        BitmapImage notMoving;
        BitmapImage forwardRight;
        BitmapImage forward;
        BitmapImage forwardLeft;
        BitmapImage backwardRight;
        BitmapImage backward;
        BitmapImage backwardLeft;
        BitmapImage roatateRight;
        BitmapImage rotateLeft;
        BitmapImage notAllowed;

        public LiveControl(SerialCommunication sc)
        {
            InitializeComponent();
            sHoist.ValueChanged += new RoutedPropertyChangedEventHandler<double>(sHoist_ValueChanged);

            this.sc = sc;
        }

        private void tbSpeedLeftMotor_TextChanged(object sender, TextChangedEventArgs e)
        {
            try
            {
                sSpeedLeftMotor.Value = int.Parse(tbSpeedLeftMotor.Text);
            }
            catch
            {
                sSpeedLeftMotor.Value = 0;
                tbSpeedLeftMotor.SelectAll();
            }
        }

        private void tbSpeedRightMotor_TextChanged(object sender, TextChangedEventArgs e)
        {
            try
            {
                sSpeedRightMotor.Value = int.Parse(tbSpeedRightMotor.Text);
            }
            catch
            {
                sSpeedRightMotor.Value = 0;
                tbSpeedRightMotor.SelectAll();
            }
        }

        private void tbHoist_TextChanged(object sender, TextChangedEventArgs e)
        {
            try
            {
                sHoist.Value = int.Parse(tbHoist.Text);
            }
            catch
            {
                sHoist.Value = 0;
                tbHoist.SelectAll();
            }
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {

            //Nothalt
            if (e.Key == Key.Escape)
            {
                direction = "";
                move();
            }

            string key = e.Key.ToString().ToLower(); //not case sensitive

            //if the key already is down since the last keyDown-event don't do anything (if you hold a key, it fires the event over and over again)
            if (e.IsRepeat)
                return;

            if (key == forwardToRightKey.ToString().ToLower())
                if (!direction.Contains("e")) direction += "e";

            if (key == forwardToLeftKey.ToString().ToLower())
                if (!direction.Contains("i")) direction += "i";

            if (key == backwardToRightKey.ToString().ToLower())
                if (!direction.Contains("f")) direction += "f";

            if (key == backwardToLeftKey.ToString().ToLower())
                if (!direction.Contains("j")) direction += "j";


            move();
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
            string key = e.Key.ToString().ToLower(); //not case sensitive

            if (key == forwardToRightKey.ToString().ToLower())
                direction = direction.Replace("e", "");

            if (key == forwardToLeftKey.ToString().ToLower())
                direction = direction.Replace("i", "");

            if (key == backwardToRightKey.ToString().ToLower())
                direction = direction.Replace("f", "");

            if (key == backwardToLeftKey.ToString().ToLower())
                direction = direction.Replace("j", "");

            move();
        }

        private void move()
        {
            lblDirection.Content = direction;
            string directionDescription = "";

            /* check for the different possibilities
             * The following combinations are possible (8 diffrent):
             *  - e:    forward     turns right forwardRight
             *  - ei:   forward     -           forward
             *  - i:    forward     turns left  forwardLeft
             *  - f:    backward    turns right backwardRight
             *  - fj:   backward    -           backward
             *  - j:    backward    turns left  backwardLeft
             *  - ej:   rotate      right       roatateRight
             *  - fi:   rotate      left        rotateLeft
             */
            if (direction == "e")
            {
                if (forwardRight == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/forwardRight.png", UriKind.Relative);
                    forwardRight = new BitmapImage(src);
                }
                imgDirection.Source = forwardRight;
                directionDescription = "Vorwärts nach rechts";
                drive(MotorDirection.forward, MotorDirection.phaseOut);
            }
            else if (direction == "ei" || direction == "ie")
            {
                if (forward == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/forward.png", UriKind.Relative);
                    forward = new BitmapImage(src);
                }
                imgDirection.Source = forward;
                directionDescription = "Geradeaus";
                drive(MotorDirection.forward, MotorDirection.forward);
            }
            else if (direction == "i")
            {
                if (forwardLeft == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/forwardLeft.png", UriKind.Relative);
                    forwardLeft = new BitmapImage(src);
                }
                imgDirection.Source = forwardLeft;
                directionDescription = "Vorwärts nach links";
                drive(MotorDirection.phaseOut, MotorDirection.forward);
            }
            else if (direction == "f")
            {
                if (backwardRight == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/backwardRight.png", UriKind.Relative);
                    backwardRight = new BitmapImage(src);
                }
                imgDirection.Source = backwardRight;
                directionDescription = "Rückwärts nach rechts";
                drive(MotorDirection.backwards, MotorDirection.phaseOut);
            }
            else if (direction == "fj" || direction == "jf")
            {
                if (backward == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/backward.png", UriKind.Relative);
                    backward = new BitmapImage(src);
                }
                imgDirection.Source = backward;
                directionDescription = "Rückwärts";
                drive(MotorDirection.backwards, MotorDirection.backwards);
            }
            else if (direction == "j")
            {
                if (backwardLeft == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/backwardLeft.png", UriKind.Relative);
                    backwardLeft = new BitmapImage(src);
                }
                imgDirection.Source = backwardLeft;
                directionDescription = "Rückwärts nach links";
                drive(MotorDirection.phaseOut, MotorDirection.backwards);
            }
            else if (direction == "ej" || direction == "je")
            {
                if (roatateRight == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/rotateRight.png", UriKind.Relative);
                    roatateRight = new BitmapImage(src);
                }
                imgDirection.Source = roatateRight;
                directionDescription = "Rotieren nach rechts";
                drive(MotorDirection.forward, MotorDirection.backwards);
            }
            else if (direction == "fi" || direction == "if")
            {
                if (rotateLeft == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/rotateLeft.png", UriKind.Relative);
                    rotateLeft = new BitmapImage(src);
                }
                imgDirection.Source = rotateLeft;
                directionDescription = "Rotieren nach links";
                drive(MotorDirection.backwards, MotorDirection.forward);
            }
            else if (direction == "")
            {
                if (notMoving == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/notMoving.png", UriKind.Relative);
                    notMoving = new BitmapImage(src);
                }
                imgDirection.Source = notMoving;
                directionDescription = "keine Bewegung";
                drive(MotorDirection.fastStop, MotorDirection.fastStop);
            }
            else
            {
                if (notAllowed == null)
                {
                    Uri src = new Uri(@"/prencar;component/Images/notAllowed.png", UriKind.Relative);
                    notAllowed = new BitmapImage(src);
                }
                imgDirection.Source = notAllowed;
                directionDescription = "unerlaubte Aktion: " + direction;
            }

            lblDirection.Content = directionDescription;
        }

        private void drive(MotorDirection directionLeft, MotorDirection directionRight)
        {
            drive(directionLeft, int.Parse(tbSpeedLeftMotor.Text), directionRight, int.Parse(tbSpeedRightMotor.Text));
        }

        private void drive(MotorDirection directionLeft, int speedLeft, MotorDirection directionRight, int speedRight)
        {
            string command = "400-4:";
            command += (int)directionLeft;
            command += "," + speedLeft.ToString();
            command += "," + (int)directionRight;
            command += "," + speedRight.ToString();
            
            //don't send the same command twice
            if (command != lastCommand)
            {
                //Console.WriteLine(command);
                sc.SendCommand(command);

                lastCommand = command;
            }
        }

        private void sHoist_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            string command = "401-1:" + sHoist.Value.ToString();
            //Console.WriteLine(command);
            sc.SendCommand(command);
        }

        private void btnToggleHoist_Click(object sender, RoutedEventArgs e)
        {
            if (btnToggleHoist.IsChecked == true)
            {
                sc.SendCommand("402-1:1");
            }
            else
            {
                sc.SendCommand("402-1:0");
            }
        }
    }
}
