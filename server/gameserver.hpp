#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

#include "gamemap.hpp"

const int SERVER_PORT = 7777;
const int MAX_PLAYERS = 32;

const int EVENT_RATE = 1000;

//Game Data
static GameMap gamemap(256,256);
static std::string usernames[MAX_PLAYERS];

static ENetHost* server;

//===================
// Network Processes
//===================
void PlayerMove(ENetEvent* event)
{
    int peer_id = event->peer -> incomingPeerID;

    int move_x = 0;
    int move_y = 1;


    std::string data_input((char*)event->packet->data);
    data_input = data_input.substr(2);
    
    //Parse input string
    std::stringstream ss(data_input);
    std::string tempString;
    std::getline(ss, tempString, ',');
    move_x = std::stoi(tempString);
    std::getline(ss, tempString, '\n');
    move_y = std::stoi(tempString);

    //Update player position
    std::string resp = "2|" + gamemap.move_entity(usernames[peer_id], "player", move_x, move_y);
    const char* data = resp.c_str();
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);
}

void DataRequest(ENetEvent* event)
{
    //Create world data dump
    std::string map_dump_resp("2|");
    map_dump_resp += gamemap.get_tile_dump() + gamemap.get_entity_dump();
    const char* data = map_dump_resp.c_str();

    //Build and send reliable packet to requester
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(event->peer, 0, packet);
}

void Authenticate(ENetEvent* event)
{
    //Grab username from packet data
    std::string username((char*)event->packet->data);
    username = username.substr(2);

    //Determine if peer already has username
    int peer_id = event -> peer -> incomingPeerID;
    const char* data;
    if(usernames[peer_id] == "")
    {
        //Update username array
        usernames[peer_id] = username;

        //Spawn entity
        std::string spawn_data = gamemap.spawn_entity(usernames[peer_id], "player", 128, 129);

        //Tell peers about new player
        data = ("2|" + spawn_data).c_str();
        ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_host_broadcast(server, 0, packet);

        std::cout << usernames[peer_id] << " spawned!" << std::endl;
        std::cout << peer_id << " | " << username << std::endl;

        std::string okay_response("1|OK");
        data = (okay_response + username).c_str();
    
        //Return success
        packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(event -> peer, 0, packet);
    }
    else
    {
        std::string resp("1|Already logged in!");
        data = resp.c_str();
    
        //Return reliable error message
        ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(event -> peer, 0, packet);
    }
}

#endif