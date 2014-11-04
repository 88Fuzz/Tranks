#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include "MySprite.hpp"
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

/*
 * Each Trank is a player.
 */
class Player: public SceneNode
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

    enum Rotation
    {
        CLOCKWISE = 1,
        COUNTER_CLOCKWISE = -1
    };
    static const int SINGLE_ROTATION = 90;
    static const int DOUBLE_ROTATION = 180;
    static const int SINGLE_MOVE = 75;
    static const int DOUBLE_MOVE = 150;

    Player();
    Player(int, TextureHolder*);
    virtual ~Player();
    void respawn();
    void setMapHeight(int);
    void setMapWidth(int);
    void setTileWidth(int);
    void setSpawnPos(sf::Vector2i, Direction);
    int getSpawnPosX();
    int getSpawnPosY();
    sf::Vector2i getTilePos();
    sf::Vector2i getTilePos(int);
    Player::Direction getForwardDirection();
    void startRotation(Rotation, int);
    void startMovement(int);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time);

private:
    static constexpr float ROTATION_SPEED = 0.13;
    static constexpr float MOVEMENT_SPEED = 0.13;

    //Tile position of spawn
    sf::Vector2i spawnPos;
    //Current tile position, for fine grained position use sprite
    sf::Vector2i tilePos;
    sf::Vector2i Pos;

    //Map information
    //the width of the map tiles, used to set the appropriate position for the sprite
    int tileWidth;
    int mapWidth;
    int mapHeight;

    //Direction trank is facing at spawn
    Direction spawnFacing;

    //Direction trank is facing
    float forward;

    //Rotation variables
    Rotation rotationDir;
    int rotationLimit;
    bool rotating;

    //Movement variables
    int moveLimit;
    bool moving;

    MySprite sprite;

    bool checkValidMove(int);
};

#endif
