using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Navigation;
using System.Threading;

namespace App
{
    internal class CommunicationManager
    {
        public static CommunicationManager instance;
        private static SerialPort serialPort = null;
        private Thread readThread = null;

        public bool IsOpen 
        { 
            get 
            {
                if (serialPort == null)
                    return false;
                return serialPort.IsOpen; 
            } 
        }

        public delegate void Received(string msg);

        public event Received OnReceived;

        public CommunicationManager()
        {
            instance = this;
        }

        public bool Start(string portName)
        {
            try
            {
                serialPort = new SerialPort(portName, 9600);
                serialPort.Open();
                serialPort.DataReceived += SerialPort_DataReceived;
            }
            catch(ObjectDisposedException)
            {
                Stop();
                return false;
            }
            catch(UnauthorizedAccessException)
            {
                Stop();
                return false;
            }

            return true;
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string result = serialPort.ReadLine();
            if (result.Length < 31)
            {
                return;
            }
            OnReceived?.Invoke(result);
        }

        public void Stop()
        {
            serialPort.Close();
            serialPort = null;
        }

        public void Send(string msg)
        {
            serialPort.Write(msg);
        }

        public static string[] GetPorts()
        {
            return SerialPort.GetPortNames();
        }
    }
}
