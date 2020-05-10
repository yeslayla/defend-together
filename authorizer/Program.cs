using System;
using System.Threading;

namespace authorizer
{
    class Program
    {
        static AuthServer server;
        static Redis redis;
        static void Main(string[] args)
        {
            redis = new Redis(Environment.GetEnvironmentVariable("REDIS_HOSTNAME"));

            redis.SetTest();

            Thread.Sleep(100);

            redis.GetTest();


            /* server = new AuthServer();

            server.Start();

            string input;
            do
            {
                input = Console.ReadLine();
            }
            while(input != "stop");
            server.Stop(); */
        }
    }
}
