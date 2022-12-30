using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using System.Windows.Media.Animation;

namespace App
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        CommunicationManager communicationManager;
        CommandManager commandManager;
        public MainWindow()
        {
            InitializeComponent();
            communicationManager = new CommunicationManager();

            PortsCB.ItemsSource = CommunicationManager.GetPorts();
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            switch(e.Key)
            {
                case Key.W:
                    communicationManager.Send("w");
                    break;
                case Key.S:
                    Command command = new Command();
                    command.Type = CommandType.Connection;
                    command.Status = CommandStatus.Ask;

                    communicationManager.Send(command.ToString());
                    break;
                case Key.A:
                    //_serialPort.Write("A");
                    break;
                case Key.D:
                    //_serialPort.Write("D");
                    break;
            }
        }

        private void ConnectBtn_Click(object sender, RoutedEventArgs e)
        {
            if(PortsCB.SelectedItem == null)
            {
                return;
            }

            if (communicationManager.IsOpen)
            {
                communicationManager.Stop();
            }

            if (!communicationManager.Start((string)PortsCB.SelectedValue))
            {
                Console.WriteLine("Error");
            }else
            {
                Console.WriteLine("Connected");
            }
        }
    }
}
