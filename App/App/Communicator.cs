using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Navigation;

namespace App
{
    internal class Communicator
    {
        private SerialPort serialPort = null;

        public bool IsOpen 
        { 
            get 
            {
                if (serialPort == null)
                    return false;
                return serialPort.IsOpen; 
            } 
        }

        public Communicator()
        {

        }

        ~Communicator()
        {
            serialPort.Close();
        }

        public void OpenPort(string portName)
        {
            serialPort = new SerialPort(portName, 9600);
            serialPort.Open();
        }
    }
}
