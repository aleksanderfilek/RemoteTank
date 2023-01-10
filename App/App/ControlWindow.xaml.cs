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
using System.Timers;


namespace App
{
    /// <summary>
    /// Logika interakcji dla klasy ControlWindow.xaml
    /// </summary>
    public partial class ControlWindow : Window
    {
        private static ControlWindow instance;
        public static ControlWindow Instance
        {
            get
            {
                return instance;
            }
        }

        private int[] keyboardState;
        private Button[] controlButtons;

        private Timer motorTimer;

        public ControlWindow()
        {
            InitializeComponent();
            instance = this;

            Closed += MainWindow_Closed;

            ChangeConnectButtonState(false);

            keyboardState = new int[4];
            for (int i = 0; i < 4; i++)
            {
                keyboardState[i] = 0;
            }
            controlButtons = new Button[4]
            {
                ForwardBtn, BackBtn, LeftBtn, RightBtn
            };

            motorTimer = new Timer(500);
            motorTimer.AutoReset = true;
            motorTimer.Elapsed += MotorTimer_Elapsed; ;
        }

        private void MainWindow_Closed(object sender, EventArgs e)
        {
            App.Disconnect();
        }

        private void MotorTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            App.ControMotors(keyboardState);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            int buttonId = 0;
            switch (e.Key)
            {
                case Key.W:
                    buttonId = 0;
                    break;
                case Key.S:
                    buttonId = 1;
                    break;
                case Key.A:
                    buttonId = 2;
                    break;
                case Key.D:
                    buttonId = 3;
                    break;
            }

            keyboardState[buttonId] = 1;

            if (!App.Connected)
                return;

            HighlightControlButton(buttonId, true);
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);
            int buttonId = 0;
            switch (e.Key)
            {
                case Key.W:
                    buttonId = 0;
                    break;
                case Key.S:
                    buttonId = 1;
                    break;
                case Key.A:
                    buttonId = 2;
                    break;
                case Key.D:
                    buttonId = 3;
                    break;
            }

            keyboardState[buttonId] = 0;

            if (!App.Connected)
                return;

            HighlightControlButton(buttonId, false);
        }

        private void ConnectBtn_Click(object sender, RoutedEventArgs e)
        {
            string portName = PortsCb.SelectedValue.ToString();
            App.Connect(portName);
        }

        public static void UpdateAfterConnection()
        {
            instance.Dispatcher.Invoke(() => {
                instance.ChangeConnectButtonState(true); });

            instance.motorTimer.Start();
        }

        public static void UpdateAfterDisconnect()
        {
            instance.Dispatcher.Invoke(() => {
                instance.ChangeConnectButtonState(false); });
        }

        private void ChangeConnectButtonState(bool Connected)
        {
            PortsCb.ItemsSource = CommunicationManager.GetPorts();
            ConnectTxt.Text = (Connected) ? "Connected" : "Disconnected";
            ConnectTxt.Foreground = (Connected) ? Brushes.Green : Brushes.Red;
        }

        private void HighlightControlButton(int ButtonId, bool Highlight)
        {

            controlButtons[ButtonId].Background = (Highlight) ? Brushes.Green : Brushes.LightGray;
        }

        public static void SetTelemetry(float Temperature, float AngleX, float AngleY, float AngleZ)
        {
            instance.Dispatcher.Invoke(() => {
                instance.TemperatureTxt.Text = Temperature.ToString();
                instance.OrientationTxt.Text = "X: " + AngleX.ToString() + ", Y: " + AngleY.ToString() + ", Z:" + AngleZ.ToString();
            });
        }

        public static void ShowErrorMessage(string Message)
        {
            instance.Dispatcher.Invoke(() => { 
                instance.ErrorTxt.Text = Message; });
        }
    }
}
