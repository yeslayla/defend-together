using System;

namespace authorizer
{
    class Program
    {
        static AuthServer server;
        static void Main(string[] args)
        {
            server = new AuthServer();

            server.Start();

            string input;
            do
            {
                input = Console.ReadLine();
            }
            while(input != "stop");
            server.Stop();
        }
    }
}
