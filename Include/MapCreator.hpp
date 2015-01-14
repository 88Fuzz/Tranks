#ifndef __MAP_CREATOR_HPP__
#define __MAP_CREATOR_HPP__
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "BoardPiece.hpp"
#include "rapidxml/rapidxml.hpp"
#include "Player.hpp"
#include <string>

/*
 * MapCreator will parse an xml file and generate the map
 */
class MapCreator
{
public:
    static const int PLAYER_COUNT = 4;

    MapCreator(TextureHolder*);
    virtual ~MapCreator();
    void generate(std::string);
    BoardPiece* getMap();
    int getMapWidth();
    int getMapHeight();
    int getTileWidth();
    int getHeight();
    int getTileHeight();
    static int get1d(int, int, int);
    sf::Vector2i getPlayerSpawnPos(int);
    Moveable::Direction getPlayerSpawnFacing(int);

private:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    BoardPiece *map;
    TextureHolder *boardTextures;
    sf::Vector2i playerSpawns[PLAYER_COUNT];
    Moveable::Direction playerSpawnFacing[PLAYER_COUNT];

    void parseTree(rapidxml::xml_node<> *);
    void parseBoard(rapidxml::xml_node<> *);
    void parseBlock(rapidxml::xml_node<> *);
    void parseSpawn(rapidxml::xml_node<> *);
    void parseFlag(rapidxml::xml_node<> *);
    void parseDeflect(rapidxml::xml_node<> *);
    void setPlayerSpawnPos(int, sf::Vector2i);
    void setPlayerSpawnFacing(int, Moveable::Direction);
};

#endif
