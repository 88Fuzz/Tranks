#ifndef __MOVEABLE_HPP__
#define __MOVEABLE_HPP__
#include "MySprite.hpp"
#include "SceneNode.hpp"
#include "Category.hpp"

class Moveable: public SceneNode
{
public:
    enum Direction
    {
        EAST = 90,
        SOUTH = 180,
        WEST = 270,
        //NORTH is 360 instead of 0 so that it can be used with angle % NORTH == 0 to check direction it is facing
        NORTH = 360
    };

    Moveable(Category::Type, int, int, int, Direction);
    virtual ~Moveable();

    void setMapWidth(int);
    int getMapWidth();
    void setMapHeight(int);
    int getMapHeight();
    void setTileSize(int);
    int getTileSize();

    Direction getForwardDirection();
protected:
    MySprite sprite;

    //Direction trank is facing
    float forward;

    //Map information
    //the width of the map tiles, used to set the appropriate position for the sprite
    int mapWidth;
    int mapHeight;
    int tileSize;

    sf::Vector2i tilePos;
};

#endif
