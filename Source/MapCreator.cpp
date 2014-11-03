#include "MapCreator.hpp"
#include "SceneNode.hpp"
#include "BoardPiece.hpp"
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <string.h>

MapCreator::MapCreator(TextureHolder* textures) :
//MapCreator::MapCreator(const TextureHolder& textures)//:
                map(new BoardPiece(Category::Type::PARENT, textures, 0, 0)),
                boardTextures(textures),
                mapWidth(-1),
                mapHeight(-1),
                tileWidth(0),
                tileHeight(0)
{
//    attachChild(&map);
//    textures->get(Textures::BOARD);
}

MapCreator::~MapCreator()
{
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
    //TODO memory management yo!
}

/*
 * kick off the map parsing process
 *
 * std::string fileName, input file to parse
 */
void MapCreator::generate(std::string fileName)
{
    rapidxml::xml_document<> doc;
    std::ifstream file(fileName);
    std::stringstream buffer;

    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);
    rapidxml::xml_node<> *pRoot = doc.first_node();

    parseTree(pRoot->first_node());
}

/*
 * xml_node is a tree based parser with child/sibling relationships
 * pareTree looks at each "parent node" and decides how to handle the node
 */
void MapCreator::parseTree(rapidxml::xml_node<> *node)
{
    if(node == NULL)
    {
        return;
    }

    //Board should be the only tag in the map xml file
    if(strcmp(node->name(), "board") == 0)
    {
        parseBoard(node);
    }
    else
    {
        //TODO create logging class/macro
        std::cerr << "Warning: unknown node: " << node->name() << std::endl;
    }
    parseTree(node->next_sibling());
}

/*
 * Called when board tag appears in xml file
 */
void MapCreator::parseBoard(rapidxml::xml_node<> *node)
{
    rapidxml::xml_attribute<>* attr = node->first_attribute();
    rapidxml::xml_node<>* child = node->first_node();
    BoardPiece *newPiece;
    int j;
    int x = 0, y = 0;

    while(attr != NULL)
    {
        if(strcmp(attr->name(), "width") == 0)
        {
            mapWidth = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "height") == 0)
        {
            mapHeight = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "type") == 0 || strcmp(attr->name(), "mode") == 0)
        {
            //TODO Do something here
        }
        else
        {
            //TODO get these logs
            std::cout << "Unknown attribute " << attr->name() << std::endl;
        }
        attr = attr->next_attribute();
    }

    if(mapWidth == -1 || mapHeight == -1)
    {
        //TODO more logs
        std::cerr << "mapWidth: " << mapWidth << " and/or mapHeight: " << mapHeight
                << " have not been properly initialized\n";
        return;
    }

    //Generate all the pieces of the board with dummy empty tiles for all pieces
    for(j = 0; j < mapWidth * mapHeight; j++)
    {
        //piece is on the top edge
        if(j < mapWidth)
        {
            //top left corner
            if(j == 0)
            {
                newPiece = new BoardPiece(Category::Type::CORNER, boardTextures, x, y, -90.0);
                x += newPiece->getSpriteWidth();
                //TODO get this width and height better
                tileWidth = newPiece->getSpriteWidth();
                tileHeight = newPiece->getSpriteHeight();
            }
            //top right corner
            else if(j == mapWidth - 1)
            {
                newPiece = new BoardPiece(Category::Type::CORNER, boardTextures, x, y, 0);
                y += newPiece->getSpriteHeight();
                x = 0;
            }
            else
            {
                newPiece = new BoardPiece(Category::Type::BORDER, boardTextures, x, y, 0);
                x += newPiece->getSpriteWidth();
            }
            map->attachChild(newPiece);
        }
        //piece is on left edge
        else if(j % mapWidth == 0)
        {
            //bottom left corner
            if(j == mapWidth * (mapHeight - 1))
                newPiece = new BoardPiece(Category::Type::CORNER, boardTextures, x, y, -180.0);
            else
                newPiece = new BoardPiece(Category::Type::BORDER, boardTextures, x, y, -90.0);

            x += newPiece->getSpriteWidth();
            map->attachChild(newPiece);
        }
        //piece is on right edge
        else if((j + 1) % mapWidth == 0)
        {
            //bottom right corner
            if(j == mapWidth * mapHeight - 1)
                newPiece = new BoardPiece(Category::Type::CORNER, boardTextures, x, y, 90.0);
            else
                newPiece = new BoardPiece(Category::Type::BORDER, boardTextures, x, y, 90.0);
            y += newPiece->getSpriteHeight();
            x = 0;
            map->attachChild(newPiece);
        }
        //piece is on bottom edge
        else if(j >= mapWidth * (mapHeight - 1))
        {
            newPiece = new BoardPiece(Category::Type::BORDER, boardTextures, x, y, 180.0);
            x += newPiece->getSpriteHeight();
            map->attachChild(newPiece);
        }
        //else it's a normal tile
        else
        {
            newPiece = new BoardPiece(Category::Type::TILE, boardTextures, x, y, 0);
            x += newPiece->getSpriteHeight();
            map->attachChild(newPiece);
        }
    }

    //Parse children of board tag
    while(child != NULL)
    {
        if(strcmp(child->name(), "block") == 0)
        {
            parseBlock(child);
        }
        else if(strcmp(child->name(), "spawn") == 0)
        {
            parseSpawn(child);
        }
        else if(strcmp(child->name(), "flag") == 0)
        {
            parseFlag(child);
        }
        else if(strcmp(child->name(), "deflect") == 0)
        {
            parseDeflect(child);
        }
        child = child->next_sibling();
    }

}

void MapCreator::parseBlock(rapidxml::xml_node<> *node)
{
    int x = -1;
    int y = -1;
    rapidxml::xml_attribute<>* attr = node->first_attribute();

    while(attr != NULL)
    {
        if(strcmp(attr->name(), "x") == 0)
        {
            x = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "y") == 0)
        {
            y = atoi(attr->value());
        }
        else
        {
            //TODO more logging yo
            std::cout << "Unkown attribute: " << attr->name() << "\n";
        }
        attr = attr->next_attribute();
    }

    if(x != -1 && y != -1)
    {
        //TODO error check
        map->swapChildNode(new BoardPiece(Category::Type::BLOCK, boardTextures, x * tileWidth, y * tileHeight),
                MapCreator::get1d(x, y, mapWidth));
    }
    else
    {
        std::cerr << "no x and y value for block\n";
    }
}

void MapCreator::parseSpawn(rapidxml::xml_node<> *node)
{
    int player = -1;
    int x = -1;
    int y = -1;
    Player::Direction facing = Player::Direction::NORTH;

    rapidxml::xml_attribute<>* attr = node->first_attribute();
    while(attr != NULL)
    {
        if(strcmp(attr->name(), "player") == 0)
        {
            player = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "x") == 0)
        {
            x = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "y") == 0)
        {
            y = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "direction") == 0)
        {
            if(strcmp(attr->value(), "N") == 0)
                facing = Player::Direction::NORTH;
            else if(strcmp(attr->value(), "E") == 0)
                facing = Player::Direction::EAST;
            if(strcmp(attr->value(), "S") == 0)
                facing = Player::Direction::SOUTH;
            if(strcmp(attr->value(), "W") == 0)
                facing = Player::Direction::WEST;

        }
        else
        {
            //TODO more logging yo
            std::cout << "Unkown attribute: " << attr->name() << "\n";
        }
        attr = attr->next_attribute();
    }

    if(x != -1 && y != -1 && player != -1)
    {
        setPlayerSpawnPos(player - 1, sf::Vector2i(x, y));
        setPlayerSpawnFacing(player - 1, facing);
        switch(player)
        {
        case 1:
            //TODO error check
            map->layerChildNode(new BoardPiece(Category::Type::SPAWN_P1, boardTextures, x * tileWidth, y * tileHeight),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        case 2:
            //TODO error check
            map->layerChildNode(new BoardPiece(Category::Type::SPAWN_P2, boardTextures, x * tileWidth, y * tileHeight),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        case 3:
            //TODO error check
            map->layerChildNode(new BoardPiece(Category::Type::SPAWN_P3, boardTextures, x * tileWidth, y * tileHeight),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        case 4:
            //TODO error check
            map->layerChildNode(new BoardPiece(Category::Type::SPAWN_P4, boardTextures, x * tileWidth, y * tileHeight),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        default:
            //TODO LOGGGGGGGGS
            std::cerr << "Unkown player count\n";

        }
    }
    else
    {
        std::cerr << "no x and y and player value for spawn\n";
    }
}

void MapCreator::parseFlag(rapidxml::xml_node<> *node)
{
    int x = -1;
    int y = -1;
    rapidxml::xml_attribute<>* attr = node->first_attribute();

    while(attr != NULL)
    {
        if(strcmp(attr->name(), "x") == 0)
        {
            x = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "y") == 0)
        {
            y = atoi(attr->value());
        }
        else
        {
            //TODO more logging yo
            std::cout << "Unkown attribute: " << attr->name() << "\n";
        }
        attr = attr->next_attribute();
    }

    if(x != -1 && y != -1)
    {
        //TODO error check
        map->layerChildNode(new BoardPiece(Category::Type::FLAG, boardTextures, x * tileWidth, y * tileHeight),
                MapCreator::get1d(x, y, mapWidth));
    }
    else
    {
        std::cerr << "no x and y value for block\n";
    }

}

void MapCreator::parseDeflect(rapidxml::xml_node<> *node)
{
    char direction = -1;
    int x = -1;
    int y = -1;
    rapidxml::xml_attribute<>* attr = node->first_attribute();

    while(attr != NULL)
    {
        if(strcmp(attr->name(), "direction") == 0)
        {
            direction = *(attr->value());
        }
        else if(strcmp(attr->name(), "x") == 0)
        {
            x = atoi(attr->value());
        }
        else if(strcmp(attr->name(), "y") == 0)
        {
            y = atoi(attr->value());
        }
        else
        {
            //TODO more logging yo
            std::cout << "Unkown attribute: " << attr->name() << "\n";
        }
        attr = attr->next_attribute();
    }

    if(x != -1 && y != -1 && direction != -1)
    {
        switch(direction)
        {
        case 'N':
            //TODO error check
            std::cout << "WHAT THE FUCK: " << tileWidth << "\n";
            map->swapChildNode(
                    new BoardPiece(Category::Type::DEFLECTOR, boardTextures, x * tileWidth, y * tileHeight, 90.0),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        case 'S':
            //TODO error check
            map->swapChildNode(
                    new BoardPiece(Category::Type::DEFLECTOR, boardTextures, x * tileWidth, y * tileHeight, -90.0),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        case 'E':
            //TODO error check
            map->swapChildNode(
                    new BoardPiece(Category::Type::DEFLECTOR, boardTextures, x * tileWidth, y * tileHeight, 180.0),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        case 'W':
            //TODO error check
            map->swapChildNode(new BoardPiece(Category::Type::DEFLECTOR, boardTextures, x * tileWidth, y * tileHeight),
                    MapCreator::get1d(x, y, mapWidth));
            break;
        default:
            //TODO LOGGGGGGGGS
            std::cerr << "Unknown player count\n";

        }
    }
    else
    {
        std::cerr << "no x and y and dirextion value for spawn\n";
    }
}

void MapCreator::setPlayerSpawnFacing(int playerNum, Player::Direction direction)
{
    playerSpawnFacing[playerNum] = direction;
}

Player::Direction MapCreator::getPlayerSpawnFacing(int playerNum)
{
    return playerSpawnFacing[playerNum];
}

void MapCreator::setPlayerSpawnPos(int playerNum, sf::Vector2i pos)
{
    playerSpawns[playerNum].x = pos.x;
    playerSpawns[playerNum].y = pos.y;
}

sf::Vector2i MapCreator::getPlayerSpawnPos(int playerNum)
{
    return sf::Vector2i(playerSpawns[playerNum].x, playerSpawns[playerNum].y);
}

int MapCreator::getMapWidth()
{
    return mapWidth;
}

int MapCreator::getMapHeight()
{
    return mapHeight;
}

int MapCreator::getTileWidth()
{
    return tileWidth;
}

int MapCreator::getTileHeight()
{
    return tileHeight;
}

int MapCreator::getHeight()
{
    return mapHeight;
}

int MapCreator::get1d(int x, int y, int width)
{
    return width * y + x;
}

BoardPiece* MapCreator::getMap()
{
    return map;
}
