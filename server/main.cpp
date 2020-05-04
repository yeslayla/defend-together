/* ./server/main.cpp */


#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <enet/enet.h>

#include "console.hpp"
#include "gameserver.hpp"
using namespace std;

int main (int argc, char ** argv)
{
    //Intialize Enet
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "ENet server failed to initialize!\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);

    //Intialize game data
    gameserver::Intialize();

    //Create console
    ServerConsole console;

    //Create ENet objects
    ENetEvent event;
    ENetAddress address;
    
    //Build Enet Host
    address.host = ENET_HOST_ANY;
    address.port = SERVER_PORT;
    const int CHANNEL_COUNT = 2;
    server = enet_host_create (&address, MAX_PLAYERS, CHANNEL_COUNT, 0, 0);
    if (server == NULL)
    {
        std::cout << "Failed to create ENet server host!" << std::endl;
        return 1;
    }

    //Start ticker
    gameserver::StartTicker();

    //Run main game server loop
    std::cout << "Awaiting connections..." << std::endl;
    while(game_is_running)
    {
        ENetEvent event;
        while (enet_host_service (server, & event, EVENT_RATE) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected from " << event.peer -> address.host
                        << ":" << event.peer -> address.port << std::endl;
                break;

                case ENET_EVENT_TYPE_RECEIVE:
                    //Channel #0 processes authentication, world updates,
                    // and player movement
                    if (event.channelID == 0)
                    {
                        gameserver::ProcessGeneralInput(&event);
                    }
                    //Channel #1 proccesses chat messages
                    else if(event.channelID == 1)
                    {
                        gameserver::ProcessChatMessage(&event);
                        
                    }
                enet_packet_destroy (event.packet);
                    
                break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    gameserver::HandlePlayerDisconnect(&event);
                    
                    //Open peer for new connection
                    event.peer -> data = NULL;
            }
        }
    }

    // Exit process
    console.stop();
    enet_host_destroy(server);
    return 0;
}
