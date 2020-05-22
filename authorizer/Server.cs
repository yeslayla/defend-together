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

    private Amazon.ECS.Model.Task GetTask(string task_arn)
    {
        // Builds ECS Request
        DescribeTasksRequest r = new DescribeTasksRequest();
        List<string> tasks = new List<string>();
        tasks.Add(task_arn);
        r.Tasks = tasks;
        
        //Send Describe Tasks Request
        var t = ecs.DescribeTasksAsync(r);
        t.RunSynchronously();

        //Return result
        return t.Result.Tasks[0];
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

                long num_of_tasks = redis.conn.ListLength("tasks");

                if(num_of_tasks > 0)
                {
                    
                    var task = GetTask(redis.conn.ListGetByIndex("tasks",0));
                    int port = task.Containers[0].NetworkBindings[0].HostPort;
                    string hostname = task.Containers[0].NetworkBindings[0].BindIP;

                    writer.Write("server:" + hostname + ":" + port.ToString());
                    writer.Flush();
                    Console.WriteLine("Routed client to " + hostname + ":" + port.ToString());
                }
                else
                {
                    string msg = "ERROR: No valid game server found!";
                    Console.WriteLine(msg);
                    writer.Write(msg);
                    writer.Flush();
                }

                //Read any client response
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