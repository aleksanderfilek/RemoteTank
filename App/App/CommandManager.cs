using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.Timers;

namespace App
{
    enum CommandType
    {
        None = 0,
        Connection = 1,
        MotorControl = 2,
        GetData = 3
    }

    enum CommandStatus
    {
        Ask = 0,
        Success = 1,
        Failed = 2
    }

    class Command
    {
        public CommandType Type;
        public CommandStatus Status;
        public byte[] Data = new byte[29];

        public Command()
        {
            for(int i = 0; i < 29; i++)
            {
                Data[i] = 0;
            }
        }

        public Command(string command)
        {
            Type = (CommandType)command[0];
            Status = (CommandStatus)command[1];
            for (int i = 0; i < 29; i++)
            {
                Data[i] = (byte)command[2 + i];
            }
        }

        public string Parse()
        {
            StringBuilder builder = new StringBuilder();
            builder.Append((char)Type);
            builder.Append((char)Status);
            for (int i = 0; i < 29; i++)
            {
                char c = (char)(Data[i]);
                builder.Append(c);
            }
            builder.Append('\n');
            return builder.ToString();
        }

        public override string ToString()
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("{ Type: " + Type.ToString()+", ");
            builder.Append("{ Status: " + Status.ToString() + ", ");
            builder.Append("Data: ");
            for (int i = 0; i < 29; i++)
            {
                builder.Append((int)Data[i]);
                if(i < 28)
                {
                    builder.Append("-");
                }
            }
            builder.Append("}");
            return builder.ToString();
        }
    }

    internal class CommandManager
    {
        public static CommandManager Instance;
        private CommunicationManager communicationManager;

        private Queue<Command> sendQueue;
        private CommandType expectedResponseType = CommandType.None;

        private Timer telemeteryTimer;

        public delegate void Received(Command command);
        private Dictionary<CommandType, Received> responseDictionary;

        private const int responseTimeout = 2000;
        private Timer responseTimer;

        public CommandManager()
        {
            Instance = this;

            sendQueue = new Queue<Command>();

            responseDictionary = new Dictionary<CommandType, Received>();
            responseDictionary.Add(CommandType.Connection, ConnectionResponse);
            responseDictionary.Add(CommandType.GetData, GetDataResponse);

            responseTimer = new Timer(responseTimeout);
            responseTimer.Elapsed += ResponseTimer_Elapsed;

            telemeteryTimer = new Timer(1000);
            telemeteryTimer.AutoReset = true;
            telemeteryTimer.Elapsed += ProcessGetDataCommand;
        }

        public void Start()
        {
            telemeteryTimer.Start();
        }

        public void SetCommunicationManager(CommunicationManager manager)
        {
            communicationManager = manager;
        }

        private void Send(Command command)
        {
            if(expectedResponseType != CommandType.None)
            {
                sendQueue.Enqueue(command);
                return;
            }

            expectedResponseType = command.Type;
            communicationManager.Send(command.Parse());
            responseTimer.Start();
        }

        public void ParseCommand(string commandStr)
        {
            Command command = new Command(commandStr);
            if (command.Type != expectedResponseType)
            {
                expectedResponseType = CommandType.None;
                App.ShowErrorMessage("[Error] Command lost. " + command.ToString());
                return;
            }

            expectedResponseType = CommandType.None;
            responseTimer.Stop();
            if (responseDictionary.ContainsKey(command.Type))
            {
                responseDictionary[command.Type]?.Invoke(command);
            }

            if (sendQueue.Count > 0)
            {
                Command nextCommand = sendQueue.Dequeue();
                Send(nextCommand);
            }
        }

        private void ResponseTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            responseTimer.Stop();
            App.ShowErrorMessage("[Error] Command lost, type: " + expectedResponseType.ToString());
        }

        public void ProcessConnectionComand()
        {
            Command command = new Command();
            command.Type = CommandType.Connection;
            command.Status = CommandStatus.Ask;
            Send(command);
        }

        private void ConnectionResponse(Command command)
        {
            if(command.Status == CommandStatus.Success)
            {
                App.OnConnected();
            }
        }

        public bool ProcessMotorControlComand(int[] KeyboardState)
        {
            if (KeyboardState[0] == 1 && KeyboardState[1] == 1)
            {
                return false;
            }

            if (KeyboardState[2] == 1 && KeyboardState[3] == 1)
            {
                return false;
            }

            byte forward = (byte)((KeyboardState[0] == 1)? 1 : (KeyboardState[1] == 1)? 2 : 0);
            byte right = (byte)((KeyboardState[2] == 1) ? 1 : (KeyboardState[3] == 1) ? 2 : 0);

            Command command = new Command();
            command.Type = CommandType.MotorControl;
            command.Status = CommandStatus.Ask;
            command.Data[0] = forward;
            command.Data[1] = right;

            Instance.Send(command);

            return true;
        }
        private void GetDataResponse(Command command)
        {
            float temperature = BitConverter.ToSingle(command.Data, 0);
            float angleX = BitConverter.ToSingle(command.Data, 4);
            float angleY = BitConverter.ToSingle(command.Data, 8);
            float angleZ = BitConverter.ToSingle(command.Data, 12);

            App.SetTelemetry(temperature, angleX, angleY, angleZ);
        }

        private void ProcessGetDataCommand(object sender, ElapsedEventArgs e)
        {
            Command command = new Command();
            command.Type = CommandType.GetData;
            command.Status = CommandStatus.Ask;
            Instance.Send(command);
        }
    }
}
