#include <gtest/gtest.h>

#include <string>
#include <iostream>

#include "gameentity.hpp"
#include "gamemap.hpp"

//Test Entities
const std::string TEST_ENTITY_ID = "test_entity";
const std::string TEST_ENTITY_TYPE= "test";
GameEntity CreateTestEntity()
{
    return GameEntity(TEST_ENTITY_ID, TEST_ENTITY_TYPE,0,0);
}
TEST(EntityTest, SetX)
{
    GameEntity entity = CreateTestEntity();
    entity.set_x(4);
    EXPECT_EQ(entity.get_x(), 4);
}
TEST(EntityTest, SetY)
{
    GameEntity entity = CreateTestEntity();
    entity.set_y(4);
    EXPECT_EQ(entity.get_y(), 4);
}
TEST(EntityTest, GetId)
{
    GameEntity entity = CreateTestEntity();
    EXPECT_EQ(entity.get_id(), TEST_ENTITY_ID);
}
TEST(EntityTest, GetType)
{
    GameEntity entity = CreateTestEntity();
    EXPECT_EQ(entity.get_type(), TEST_ENTITY_TYPE);
}
TEST(EntityTest, GetBasicDump)
{
    GameEntity entity = CreateTestEntity();
    //Test intial location (0,0)
    EXPECT_EQ(entity.get_dump(), std::string("0,0," + TEST_ENTITY_TYPE + ":" + TEST_ENTITY_ID + "\n"));

    //Test non-intial location (1,2)
    entity.set_x(1);
    entity.set_y(2);
    EXPECT_EQ(entity.get_dump(), std::string("1,2," + TEST_ENTITY_TYPE + ":" + TEST_ENTITY_ID + "\n"));
}

//Test GameMap Object
GameMap CreateMapEntity(int x = 16, int y = 16)
{
    return GameMap(x,y);
}
TEST(GameMapTest, CheckMapSize)
{
    GameMap map = CreateMapEntity(8,8);
    EXPECT_EQ(map.get_size_x(), 8);
    EXPECT_EQ(map.get_size_y(), 8);

    map = CreateMapEntity(16, 4);
    EXPECT_EQ(map.get_size_x(), 16);
    EXPECT_EQ(map.get_size_y(), 4);
}
TEST(GameMapTest, CreateEntity)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE);
    
    EXPECT_TRUE(map.entity_exists(TEST_ENTITY_ID, TEST_ENTITY_TYPE));
}
TEST(GameMapTest, IntialEntityPos)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 0,0);
    
    EXPECT_EQ(map.get_entity_pos_x(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 0);
    EXPECT_EQ(map.get_entity_pos_y(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 0);
}
TEST(GameMapTest, MoveEntityFromZero)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 0,0);

    //Move entity
    map.move_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 2, 1);

    EXPECT_EQ(map.get_entity_pos_x(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 2);
    EXPECT_EQ(map.get_entity_pos_y(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 1);
}
TEST(GameMapTest, MoveEntityRelative)
{

    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 0,0);

    //Move entity
    map.move_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 2, 1);

    //Move entity again!
    map.move_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 1, 1);

    EXPECT_EQ(map.get_entity_pos_x(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 3);
    EXPECT_EQ(map.get_entity_pos_y(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 2);   
}
TEST(GameMapTest, MoveEntityNegativeFromZero)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 0, 0);
    
    //Move entity
    map.move_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, -1, -1);

    EXPECT_EQ(map.get_entity_pos_x(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 0);
    EXPECT_EQ(map.get_entity_pos_y(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 0);   
}
TEST(GameMapTest, MoveEntityNegativeFromNonZero)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 1, 1);
    
    //Move entity
    std::cout << std::string(map.move_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, -2, -2)) << std::endl;

    EXPECT_EQ(map.get_entity_pos_x(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 0);
    EXPECT_EQ(map.get_entity_pos_y(TEST_ENTITY_ID, TEST_ENTITY_TYPE), 0);   
}
TEST(GameMapTest, StopEntityFromMovingOutOfBounds)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, map.get_size_x()-1, map.get_size_y()-1);
    
    //Move entity
    map.move_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE, 2, 2);

    EXPECT_EQ(map.get_entity_pos_x(TEST_ENTITY_ID, TEST_ENTITY_TYPE), map.get_size_x() - 1);
    EXPECT_EQ(map.get_entity_pos_y(TEST_ENTITY_ID, TEST_ENTITY_TYPE), map.get_size_y() - 1);   
}
TEST(GameMapTest, RemoveMapEntity)
{
    GameMap map = CreateMapEntity();
    map.spawn_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE);
    EXPECT_TRUE(map.remove_entity(TEST_ENTITY_ID, TEST_ENTITY_TYPE));
    EXPECT_FALSE(map.entity_exists(TEST_ENTITY_ID, TEST_ENTITY_TYPE));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}