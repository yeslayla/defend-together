using System;
using System.Threading;
using System.Net;
using System.Net.Sockets;
class AuthServer
{
    private int port;
    private IPAddress address;
    private TcpListener server;
    private Thread thread;
    private bool running = false;
    public AuthServer(string addr = "0.0.0.0", int p = 7778)
    {
        port = p;
        address = IPAddress.Parse(addr);

        server = new TcpListener(address, port);
    }
    private void ServerLoop()
    {
        while(running)
        {
            Console.WriteLine("Waiting for a connection...");

            TcpClient client = server.AcceptTcpClient();
            Console.WriteLine("Connected!");

            Byte[] bytes = new byte[256];
            String data = null;

            NetworkStream stream = client.GetStream();

            int i;

            while((i = stream.Read(bytes, 0, bytes.Length)) != 0)
            {
                data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);
                Console.WriteLine("Recieved: {0}", data);
            }
            
            client.Close();
        }
    }

    public void Start()
    {
        server.Start();
        running = true;
        ThreadStart entrypoint = new ThreadStart(ServerLoop);
        thread = new Thread(entrypoint);
        thread.Start();
    }
    public void Stop()
    {
        running = false;
    }
    public void ForceStop()
    {
        thread.Join();
    }
}