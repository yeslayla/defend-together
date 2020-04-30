#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <string>
#include <iostream>

class GameEntity
{
    public:
        GameEntity(std::string,std::string,int,int);
        std::string get_id(void);
        std::string get_type(void);
        int get_x(void);
        int get_y(void);
        std::string set_x(int);
        std::string set_y(int);
        std::string get_dump(void);
    private:
        int pos_x;
        int pos_y;
        std::string id;
        std::string type;

};

GameEntity::GameEntity(std::string entity_id, std::string entity_type, int x = 0, int y = 0)
{
    id = entity_id;
    type = entity_type;
    this->set_x(x);
    this->set_y(y);
    std::cout << "Entity created with id: '" << id << "' at " << x << "," << y << std::endl;
}


int GameEntity::get_x(void)
{
    return pos_x;
}

int GameEntity::get_y(void)
{
    return pos_y;
}

std::string GameEntity::get_dump(void)
{
    return std::to_string(pos_x) + "," + std::to_string(pos_y) + "," + type + ":" + id + '\n';
}

std::string GameEntity::set_x(int x)
{
    pos_x = x;
    return this -> get_dump();
}
std::string GameEntity::set_y(int y)
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