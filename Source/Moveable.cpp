#include "Moveable.hpp"
#include "Utils.hpp"
#include <iostream>

Moveable::Moveable(Category::Type category, int mapWidth, int mapHeight, int tileSize, Direction forward) :
        SceneNode(category), tileSize(tileSize), forward(forward), mapWidth(mapWidth), mapHeight(mapHeight)
{
}

Moveable::~Moveable()
{
    destroy();
}

void Moveable::destroy()
{
    SceneNode::destroy();
}

void Moveable::setMapWidth(int width)
{
    mapWidth = width;
}

int Moveable::getMapWidth()
{
    return mapWidth;
}

void Moveable::setMapHeight(int height)
{
    mapHeight = height;
}

int Moveable::getMapHeight()
{
    return mapHeight;
}

void Moveable::setTileSize(int size)
{
    tileSize = size;
}

int Moveable::getTileSize()
{
    return tileSize;
}

Moveable::Direction Moveable::getForwardDirection()
{
    if(mod(forward, Direction::NORTH) == 0)
        return Direction::NORTH;
    else if(mod(forward, Direction::SOUTH) == 0)
        return Direction::SOUTH;
    else if(mod(forward, Direction::WEST) == 0)
        return Direction::WEST;
    return Direction::EAST;
}
