#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "gameentity.hpp"

class GameMap
{
    public:
        GameMap(int,int);
        int get_tile(int,int);
        void set_tile(int,int,int);
        std::string get_tile_dump(void);
        std::string get_entity_dump(void);
        std::string spawn_entity(std::string,std::string,float,float);
        std::string move_entity(std::string,std::string,float,float);
        void set_entity_velocity(std::string,std::string,float,float);
        std::string move_enitity_relatively(std::string,std::string,float,float);
        int get_entity_pos_x(std::string,std::string);
        int get_entity_pos_y(std::string,std::string);
        int get_size_x(void);
        int get_size_y(void);
        bool entity_exists(std::string, std::string);
        bool remove_entity(std::string, std::string);
        std::string world_tick(void);

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

std::string GameMap::spawn_entity(std::string entity_id, std::string entity_type, float x = 0, float y = 0)
{
    GameEntity entity = GameEntity(entity_id, entity_type, x, y);
    entities.push_back(entity);
    return entity.get_dump();
}

std::string GameMap::move_enitity_relatively(std::string entity_id, std::string entity_type, float x = 0, float y = 0)
{
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].get_id() == entity_id && entities[i].get_type() == entity_type)
        {
            return move_entity(entity_id, entity_type, x - entities[i].get_x(), y - entities[i].get_y());
        }
    }
    return "";
}

std::string GameMap::move_entity(std::string entity_id, std::string entity_type, float x = 0, float y = 0)
{
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].get_id() == entity_id && entities[i].get_type() == entity_type)
        {
            GameEntity* entity = &entities[i];
            //if(map_data[entities[i].get_x() + x][entities[i].get_y() + y] % 2 == 1)
            //{
            if(entity->get_x() + x < 0)
            {
                entity->set_x(0);
            }
            else if(entity->get_x() + x >= this->get_size_x())
            {
                entity->set_x(this->get_size_x() - 1);
            }
            else
            {
                entity->set_x(entities[i].get_x() + x);
            }
            
            if(entity->get_y() + y < 0)
            {
                entity->set_y(0);
            }
            else if(entity->get_y() + y >= this->get_size_y())
            {
                entity->set_y(this->get_size_y() - 1);
            }
            else
            {
                entity->set_y(entities[i].get_y() + y);
            }

            return entity->get_dump();
        }
    }
    return "";
}

void GameMap::set_entity_velocity(std::string entity_id, std::string entity_type, float x = 0, float y = 0)
{
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].get_id() == entity_id && entities[i].get_type() == entity_type)
        {
            GameEntity* entity = &entities[i];
            entity->set_velocity(x,y);   
        }
    }
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

std::string GameMap::world_tick(void)
{
    std::string dump = "";
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].movement_tick())
        {
            dump += entities[i].get_dump();
        }
    }
    if(dump != "")
    {
    std::cout << dump << std::endl;
    }
    return dump;
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

bool GameMap::entity_exists(std::string entity_id, std::string entity_type)
{
    for(int i = 0; i < entities.size(); i++)
    {
        GameEntity* entity = &entities[i];
        if(entity->get_id() == entity_id)
        {
            if(entity->get_type() == entity_type)
            {
                return true;
            }
        }
    }

    return false;
}

bool erase(std::vector<GameEntity> &v, GameEntity key)
{
    for(auto it = v.begin(); it != v.end();)
    {
        if (it->get_id() == key.get_id())
        {
            it = v.erase(it);
            return true;
        }
        else
        {
            ++it;
        }
    }
    return false;
}

bool GameMap::remove_entity(std::string entity_id, std::string entity_type)
{

    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].get_id() == entity_id && entities[i].get_type() == entity_type)
        {
            return erase(entities, entities[i]);
        }
    }

    return false;
}

int GameMap::get_entity_pos_x(std::string entity_id, std::string entity_type)
{
    for(int i = 0; i < entities.size(); i++)
    {
        GameEntity* entity = &entities[i];
        if(entity->get_id() == entity_id)
        {
            if(entity->get_type() == entity_type)
            {
                return entity->get_x();
            }
        }
    }
    return -1;
}
int GameMap::get_entity_pos_y(std::string entity_id, std::string entity_type)
{
    for(int i = 0; i < entities.size(); i++)
    {
        GameEntity* entity = &entities[i];
        if(entity->get_id() == entity_id)
        {
            if(entity->get_type() == entity_type)
            {
                return entity->get_y();
            }
        }
    }
    return -1;
}

#endif