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
using System.Diagnostics;
using Microsoft.Win32;

namespace ch.jep.McCommunication
{
    /// <summary>
    /// Interaktionslogik für McConfiguration.xaml
    /// </summary>
    public partial class McConfigurationView : Window
    {
        #region Commands
        public static RoutedCommand UploadConfiguration = new RoutedCommand();
        #endregion

        McConfiguration currentConf;
        SerialCommunication sc;

        public McConfigurationView(SerialCommunication sc, McConfiguration currentConf)
        {
            InitializeComponent();

            UploadConfiguration.InputGestures.Add(new KeyGesture(Key.U, ModifierKeys.Control));

            this.currentConf = currentConf;
            this.sc = sc;

            RefreshConfigurationList();
        }

        private void lbAvailableConfigurations_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                if (lbAvailableConfigurations.SelectedIndex != -1)
                {
                    dgSettings.ItemsSource = ((McConfiguration)lbAvailableConfigurations.SelectedItem).settings;
                }
            }
            catch
            {
                MessageBox.Show("Diese Datei scheint keine gültige Konfigurationsdatei zu sein.", "Fehler");
            }
        }

        public void RefreshConfigurationList()
        {
            this.lbAvailableConfigurations.Items.Clear();
            this.lbAvailableConfigurations.Items.Add(currentConf);
            this.lbAvailableConfigurations.SelectedItem = currentConf;

            //list all available config files
            String path = Settings.Default.ConfigurationFilesPath;
            if (!Directory.Exists(path))
            {
                MessageBox.Show("Konfigurationsverzeichnis kann nicht gefunden werden.\n" + path + "\nEs wird erstellt.", "Fehler", MessageBoxButton.OK, MessageBoxImage.Error);
                Directory.CreateDirectory(path);
                return;
            }
            String[] filenames = Directory.GetFiles(path);
            foreach(string aFilename in filenames){
                McConfiguration aConfig = new McConfiguration();
                aConfig.Title = System.IO.Path.GetFileNameWithoutExtension(aFilename);
                aConfig.parseFileConfiguration(File.ReadAllText(aFilename));
                this.lbAvailableConfigurations.Items.Add(aConfig);
            }
        }

        private void ExecutedUploadConfigurationCommand(object sender, ExecutedRoutedEventArgs e)
        {
            dgSettings.CommitEdit(DataGridEditingUnit.Row, true);

            McConfiguration confToUpload = (McConfiguration)this.lbAvailableConfigurations.SelectedItem;
            String command = "101-" + confToUpload.settings.Count.ToString() + ":" + confToUpload.GetMcConfigurationDump();
            Debug.Print(command);
            sc.SendCommand(command);
        }

        private void ExecutedSaveCommand(object sender, ExecutedRoutedEventArgs e)
        {
            if (lbAvailableConfigurations.SelectedIndex == 0)
            {
                //the running configuration can not be saved directly. ==> create a duplication
                btnDuplicateConfiguration_Click(sender, e);
                return;
            }
            dgSettings.CommitEdit(DataGridEditingUnit.Row, true);
            String path = Settings.Default.ConfigurationFilesPath;
            McConfiguration confToSave = (McConfiguration)this.lbAvailableConfigurations.SelectedItem;
            File.WriteAllText(System.IO.Path.Combine(new string[] { path, confToSave.Title + ".txt" }), confToSave.GetFileConfigurationDump()); 
        }

        private void btnDuplicateConfiguration_Click(object sender, RoutedEventArgs e)
        {
            McConfiguration confToDuplicate = (McConfiguration)this.lbAvailableConfigurations.SelectedItem;

            SaveFileDialog dlg = new SaveFileDialog();
            dlg.InitialDirectory = System.IO.Path.GetFullPath(Settings.Default.ConfigurationFilesPath);
            dlg.FileName = "DuplicationOf_" + confToDuplicate.Title;
            dlg.DefaultExt = ".txt";
            dlg.Filter = "Text documents (.txt)|*.txt"; // Filter files by extension

            bool? dialogRes = dlg.ShowDialog();

            if (dialogRes == true)
            {
                File.WriteAllText(dlg.FileName, confToDuplicate.GetFileConfigurationDump());
                RefreshConfigurationList();
            }
        }

        internal void DisplayConfigUpdateError(Message message)
        {
            MessageBox.Show(message.MessageCombined, "Error while updating the configuration", MessageBoxButton.OK, MessageBoxImage.Error);
            this.RefreshConfigurationList();
        }

        private void btnOpenSourceFolder_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                String path = Settings.Default.ConfigurationFilesPath;
                string windir = Environment.GetEnvironmentVariable("WINDIR");
                System.Diagnostics.Process prc = new System.Diagnostics.Process();
                prc.StartInfo.FileName = windir + @"\explorer.exe";
                prc.StartInfo.Arguments = path;
                prc.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
            {
                Close();
            }
        }
    }
}
