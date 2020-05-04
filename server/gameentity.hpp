#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <string>
#include <iostream>

class GameEntity
{
    public:
        GameEntity(std::string,std::string,float,float);
        std::string get_id(void);
        std::string get_type(void);
        float get_x(void);
        float get_y(void);
        std::string set_x(float);
        std::string set_y(float);
        void set_velocity(float, float);
        std::string get_dump(void);
        bool movement_tick(void);
    private:
        float pos_x;
        float pos_y;
        float velocity_x;
        float velocity_y;
        std::string id;
        std::string type;

};

GameEntity::GameEntity(std::string entity_id, std::string entity_type, float x = 0, float y = 0)
{
    id = entity_id;
    type = entity_type;
    velocity_x = 0;
    velocity_y = 0;
    this->set_x(x);
    this->set_y(y);
    std::cout << "Entity created with id: '" << id << "' at " << x << "," << y << std::endl;
}

bool GameEntity::movement_tick()
{
    if(velocity_x != 0 || velocity_y != 0)
    {
        set_x(pos_x + velocity_x);
        set_y(pos_y + velocity_y);
        return true;
    }
    return false;
}

void GameEntity::set_velocity(float x, float y)
{
    velocity_x = x;
    velocity_y = y;
}

float GameEntity::get_x(void)
{
    return pos_x;
}

float GameEntity::get_y(void)
{
    return pos_y;
}

std::string GameEntity::get_dump(void)
{
    return std::to_string(pos_x) + "," + std::to_string(pos_y) + "," + type + ":" + id + '\n';
}

std::string GameEntity::set_x(float x)
{
    pos_x = x;
    return this -> get_dump();
}
std::string GameEntity::set_y(float y)
{
    pos_y = y;
    return this -> get_dump();
}

std::string GameEntity::get_id(void)
{
    return id;
}

std::string GameEntity::get_type(void)
{
    return type;
}


#endif