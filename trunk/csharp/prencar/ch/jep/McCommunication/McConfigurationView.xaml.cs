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
using ch.hslu.prencar.Properties;
using System.IO;

namespace ch.jep.McCommunication
{
    /// <summary>
    /// Interaktionslogik für McConfiguration.xaml
    /// </summary>
    public partial class McConfigurationView : Window
    {
        McConfiguration currentConf;
        SerialCommunication sc;

        public McConfigurationView(SerialCommunication sc, McConfiguration currentConf)
        {
            InitializeComponent();
            this.currentConf = currentConf;
            this.sc = sc;

            refreshConfigurationList();
        }

        private void displayConfiguration(McConfiguration conf)
        {

        }

        private void lbAvailableConfigurations_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                dgSettings.ItemsSource = ((McConfiguration)lbAvailableConfigurations.SelectedItem).settings;
            }
            catch
            {
                MessageBox.Show("Diese Datei scheint keine gültige Konfigurationsdatei zu sein.", "Fehler");
            }
        }

        private void refreshConfigurationList()
        {
            this.lbAvailableConfigurations.Items.Clear();
            this.lbAvailableConfigurations.Items.Add(currentConf);
            this.lbAvailableConfigurations.SelectedItem = currentConf;

            //list all available config files
            String path = Settings.Default.ConfigurationFilesPath;
            String[] filenames = Directory.GetFiles(path);
            foreach(string aFilename in filenames){
                McConfiguration aConfig = new McConfiguration();
                aConfig.Title = System.IO.Path.GetFileNameWithoutExtension(aFilename);
                aConfig.parseConfiguration(File.ReadAllText(aFilename));
                this.lbAvailableConfigurations.Items.Add(aConfig);
            }
        }
    }
}
