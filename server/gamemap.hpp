#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <string>
#include <iostream>
#include <vector>

#include "gameentity.hpp"

class GameMap
{
    public:
        GameMap(int,int);
        int get_tile(int,int);
        void set_tile(int,int,int);
        std::string get_tile_dump(void);
        std::string get_entity_dump(void);
        std::string spawn_entity(std::string,std::string,int,int);
        std::string move_entity(std::string,std::string,int,int);
        int get_size_x(void);
        int get_size_y(void);
    private:
        int ** map_data;
        int size_x;
        int size_y;
        bool valid_x_y(int,int);
        std::vector<GameEntity> entities;

};

GameMap::GameMap(int x, int y)
{
    size_x = x;
    size_y = y;
    map_data = new int * [x];
    for (int i = 0; i < x; i++)
    {
        map_data[i] = new int [y];
        for(int q = 0; q < y; q++)
        {
            if(q == 0 || i == 0 || i == x-1 || q == y-1)
            {
                map_data[i][q] = 0;
            }
            else
            {
                map_data[i][q] = 1;
            }
        }
    }

    this -> spawn_entity("spawn","crystal",size_x/2, size_y/2);

}

std::string GameMap::spawn_entity(std::string entity_id, std::string entity_type, int x = 0, int y = 0)
{
    GameEntity entity = GameEntity(entity_id, entity_type, x, y);
    entities.push_back(entity);
    return entity.get_dump();
}

std::string GameMap::move_entity(std::string entity_id, std::string entity_type, int x = 0, int y = 0)
{
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].get_id() == entity_id && entities[i].get_type() == entity_type)
        {
            if(map_data[entities[i].get_x() + x][entities[i].get_y() + y] % 2 == 1)
            {
                entities[i].set_x(entities[i].get_x() + x);
                entities[i].set_y(entities[i].get_y() + y);
            }
            return entities[i].get_dump();
        }
    }
    return "";
}

bool GameMap::valid_x_y(int x, int y)
{
    if(x < size_x && x >= 0)
    {
        if(y < size_y && y >= 0)
        {
            return true;
        }
    }
    return false;
}

int GameMap::get_tile(int x, int y)
{
    if(this->valid_x_y(x,y))
    {
        return map_data[x][y];
    }
    return -1;
}
void GameMap::set_tile(int x, int y, int tile_type)
{
    if(this->valid_x_y(x,y))
    {
        map_data[x][y] = tile_type;
    }
}
std::string GameMap::get_tile_dump(void)
{
    std::string data_string("");
    for(int x = 0; x < size_x; x++)
    {
        for(int y = 0; y < size_y; y++)
        {
            data_string += std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(map_data[x][y]) + '\n';
        }
    }
    return data_string;
}

std::string GameMap::get_entity_dump(void)
{
    std::string data_string("");
    for(int i = 0; i < entities.size(); i++)
    {
        data_string += entities[i].get_dump();
    }
    return data_string;
}

int GameMap::get_size_x(void)
{
    return size_x;
}

int GameMap::get_size_y(void)
{
    return size_y;
}

#endif