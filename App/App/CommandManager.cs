using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace App
{
    enum CommandType
    {
        Connection = 1
    }

    enum CommandStatus
    {
        OK = 0,
        Failed = 1,
        Ask = 2
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

        public override string ToString()
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
    }

    internal class CommandManager
    {
        public string SendCommand()
        {
            return "";
        }

        public void ParseCommand(string command)
        {

        }
    }
}
