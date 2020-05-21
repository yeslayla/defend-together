using System;
using StackExchange.Redis;

class Redis
{
    private ConnectionMultiplexer muxer;
    public IDatabase conn;
    private string hostname;
    private int port;
    public Redis(string host = "127.0.0.1", int p = 6379)
    {
        if(host == "")
        {
            throw new Exception("Must provide a redis hostname!");
        }

        //Set private variables
        hostname = host;
        port = p;

        //Connect to redis cluster
        Console.WriteLine("Attempting to connect to: " + host + ":" + p.ToString());
        muxer = ConnectionMultiplexer.Connect(hostname + ":" + port.ToString());
        conn = muxer.GetDatabase();
        Console.WriteLine("Connected to redis server!");
    }

    ~Redis()
    {
        muxer.Close();
    }
}