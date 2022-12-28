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

        public MainWindow()
        {
            InitializeComponent();
        }

        ~MainWindow()
        {
        }

        private void SendMsg(object sender, RoutedEventArgs e)
        {
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            switch(e.Key)
            {
                case Key.W:
                    //_serialPort.Write("W");
                    break;
                case Key.S:
                    //_serialPort.Write("S");
                    break;
                case Key.A:
                    //_serialPort.Write("A");
                    break;
                case Key.D:
                    //_serialPort.Write("D");
                    break;
            }
        }
    }
}
