using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Security.RightsManagement;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace App
{
    /// <summary>
    /// Logika interakcji dla klasy App.xaml
    /// </summary>
    public partial class App : Application
    {
        private static App instance;

        private CommunicationManager communicationManager;
        private CommandManager commandManager;
        
        public static bool Connected 
        { 
            get
            {
                return instance.communicationManager.IsOpen;
            } 
        } 

        public App()
        {
            InitializeComponent();
            instance = this;

            commandManager = new CommandManager();
            communicationManager = new CommunicationManager();
            communicationManager.OnReceived += commandManager.ParseCommand;
            commandManager.SetCommunicationManager(communicationManager);
        }


        public static void Connect(string PortName)
        {
            if(instance.communicationManager.Start(PortName))
            {
                instance.commandManager.ProcessConnectionComand();
            }
        }

        public static void OnConnected()
        {
            ControlWindow.UpdateAfterConnection();
            instance.commandManager.Start();
        }

        public static void Disconnect()
        {
            if(instance.communicationManager.IsOpen)
            {
                instance.communicationManager.Stop();
                ControlWindow.UpdateAfterDisconnect();
            }
        }

        public static void ControMotors(int[] keystates)
        {
            instance.commandManager.ProcessMotorControlComand(keystates);
        }

        public static void ShowErrorMessage(string Message)
        {
            ControlWindow.ShowErrorMessage(Message);
        }

        public static void SetTelemetry(float Temperature, float AngleX, float AngleY, float AngleZ)
        {
            ControlWindow.SetTelemetry(Temperature, AngleX, AngleY, AngleZ);
        }

    }
}
