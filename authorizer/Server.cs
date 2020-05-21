using System;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using System.Net;
using System.Net.Sockets;

using Amazon.ECS;
using Amazon.ECS.Model;
using System.Collections.Generic;
class AuthServer
{
    private bool running = false;
    private int port;
    private Thread thread;
    private IPAddress address;

    // Core objects
    private TcpListener server;
    private Redis redis;
    private AmazonECSClient ecs;


    public AuthServer(string addr = "0.0.0.0", int p = 7778)
    {
        port = p;
        address = IPAddress.Parse(addr);

        redis = new Redis(Environment.GetEnvironmentVariable("REDIS_HOSTNAME"));
        server = new TcpListener(address, port);
        ecs = new AmazonECSClient();
    }
    private void ServerLoop()
    {
        while(running)
        {
            try
            {
                //Wait for connection
                TcpClient client = server.AcceptTcpClient();
                //Get remote address
                IPEndPoint endPoint = (IPEndPoint) client.Client.RemoteEndPoint;
                Console.WriteLine(endPoint.Address.ToString() + " connected!");

                //Create streams
                NetworkStream stream = client.GetStream();
                StreamWriter writer = new StreamWriter(stream);

                Byte[] bytes = new byte[256];
                String data = null;


                writer.Write("Hey there bud!");
                writer.Flush();
                Console.WriteLine("HERE");

                byte[] sendBytes = System.Text.Encoding.ASCII.GetBytes("GET / HTTP/1.1");
                stream.Write(sendBytes, 0, sendBytes.Length);
                client.Client.Send(sendBytes);


                //BinaryWriter writer = new BinaryWriter(stream);
                //writer.Write("TEST");

                

                int i;

                while((i = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);
                    Console.WriteLine("Recieved: {0}", data);

    
                }
                
                client.Close();
                Console.WriteLine("Client disconnected");
            }
            catch(Exception e)
            {
                Console.WriteLine("Fatal exception: " + e.ToString());
            }
        }
    }

    public void Start()
    {
        server.Start();
        running = true;
        ThreadStart entrypoint = new ThreadStart(ServerLoop);
        thread = new Thread(entrypoint);
        thread.Start();
        Console.WriteLine("Waiting for a connection...");
    }
    public void Stop()
    {
        running = false;
    }
    public void ForceStop()
    {
        running = false;
        thread.Abort();
    }
}