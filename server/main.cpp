/* ./server/main.cpp */


#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <enet/enet.h>

#include "console.hpp"
#include "gameserver.hpp"

int main (int argc, char ** argv)
{
    printf("Starting LD46 game server...\n");

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        usernames[i] = "";
    }
    

    if (enet_initialize () != 0)
    {
        fprintf (stderr, "ENet server failed to initialize!\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);


    ServerConsole console;

    //Create ENet objects
    ENetEvent event;
    ENetAddress address;
    

    address.host = ENET_HOST_ANY;
    address.port = SERVER_PORT;


    const int CHANNEL_COUNT = 2;
    server = enet_host_create (&address, MAX_PLAYERS, CHANNEL_COUNT, 0, 0);

    if (server == NULL)
    {
        std::cout << "Failed to create ENet server host!" << std::endl;
        return 1;
    }

    std::cout << "Awaiting connections..." << std::endl;
    while(console_is_running)
    {
        ENetEvent event;
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service (server, & event, EVENT_RATE) > 0)
        {
            printf("Parsing event!\n");
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected from " << event.peer -> address.host
                        << ":" << event.peer -> address.port << std::endl;
                break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "A packet of length " << event.packet -> dataLength << " containing "
                        << event.packet -> data << " was received from " << event.peer -> data << " on channel"
                        << event.channelID << std::endl;
                    if (event.channelID == 0)
                    {
                        char c [1];
                        c[0] = event.packet->data[0];

                        /* Determine which function to call based on
                            the value of the first character of packet */
                        std::cout << "Packet Type: " << c << std::endl;
                        switch (c[0])
                        {
                        case '1':
                            Authenticate(&event);
                            break;
                        case '2':
                            DataRequest(&event);
                            break;
                        case '3':
                            PlayerMove(&event);
                            break;
                        default:
                            std::cout << "Invalid packet recieved!" <<std::endl;
                            break;
                        }
                        enet_packet_destroy (event.packet);
                    }
                    else if(event.channelID == 1)
                    {
                        int peer_id = event.peer -> incomingPeerID;
                        if(usernames[peer_id] != "")
                        {
                            //Parse input string
                            std::stringstream ss((char*)event.packet->data);
                            std::string chat_message;
                            std::getline(ss, chat_message, '\n');
                            std::string resp = "<" + usernames[peer_id] + "> " + chat_message;
                            const char* data = resp.c_str();

                            std::cout << data << std::endl;

                            ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_host_broadcast(server, 1, packet);
                        }
                        enet_packet_destroy (event.packet);
                    }

                    
                break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << event.peer -> data << " disconnected." << std::endl;

                    //Clear username data and remove entity
                    std::string username = usernames[event.peer -> incomingPeerID];
                    if(username != "")
                    {
                        std::cout << "Removing '" << username << "'s player data!" << std::endl;
                        gamemap.remove_entity(username,"player");
                        usernames[event.peer -> incomingPeerID] = "";

                        std::string resp = "2|delete,player:" + username;
                        const char* data = resp.c_str();
                        ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
                        enet_host_broadcast(server, 0, packet);
                    }
                    
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
