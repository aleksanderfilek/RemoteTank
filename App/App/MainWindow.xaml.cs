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

        SerialPort _serialPort;

        public MainWindow()
        {
            InitializeComponent();

            _serialPort = new SerialPort("COM7", 9600);
            _serialPort.Open();
        }

        ~MainWindow()
        {
            _serialPort.Close();
        }

        private void SendMsg(object sender, RoutedEventArgs e)
        {
            _serialPort.Write("A");
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            if (e.Key == Key.W)
            {
                _serialPort.Write("A");

            }
        }
    }
}
